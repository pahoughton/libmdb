//
// File:        MapFile.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for MapFile
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     11/18/94 11:13 
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "MapFile.hh"
#include "_MdbMemMap.hh"

#include <Str.hh>
#include <StlUtilsMisc.hh>

#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>


#if defined( MDB_DEBUG )
#include "MapFile.ii"
#endif

MDB_VERSION(
  MapFile,
  "$Id$");


const MapFile::size_type  MapFile::npos = NPOS;
MapFile::size_type	    MapFile::totalMapped( 0 );

MapFile::MapFile()
  : mapFd( -1 ),
    mapMode( ios::in ),
    mapSize( 0 ),
    mapBase( 0 ),
    refCount( 0 ),
    pageSize( getpagesize() ),
    osErrno( ENOENT )
{
}

MapFile::MapFile(
  const char *	    fileName,
  MapAddr	    baseAddr,
  ios::open_mode    mode,
  bool		    create,
  size_type	    size,
  MapMask	    permMask
  )
  : fileStat( fileName ),
    mapFd( -1 ),
    mapMode( create ? (ios::open_mode)( mode | ios::out) : mode ),
    mapSize( 0 ),
    mapBase( 0 ),
    refCount( 0 ),
    pageSize( getpagesize() ),
    osErrno( 0 )
{
  if( create )
    createMap( fileName, baseAddr, size, permMask );
  else
    map( fileName, baseAddr, mode );
}

MapFile::MapFile(
  const char * 	    fileName,
  MapAddr    	    baseAddr,
  ios::open_mode    mode
  )
  : fileStat( fileName ),
    mapFd( -1 ),
    mapMode( mode ),
    mapSize( 0 ),
    mapBase( 0 ),
    refCount( 0 ),
    pageSize( getpagesize() ),
    osErrno( 0 )
{
  map( fileName, baseAddr, mode );
}

MapFile::MapFile(
  const char * 	    fileName,
  size_type	    size,
  MapAddr    	    baseAddr,
  MapMask	    permMask
  )
  : fileStat( fileName ),
    mapFd( -1 ),
    mapMode( (ios::open_mode)(ios::in | ios::out) ),
    mapSize( 0 ),
    mapBase( 0 ),
    refCount( 0 ),
    pageSize( getpagesize() ),
    osErrno( 0 )
{
  createMap( fileName, baseAddr, size, permMask );
}

  
MapFile::~MapFile( void )
{
  unmap();
}


MapFile::size_type
MapFile::createMap(
  const char *	    fileName,
  MapAddr	    baseAddr,
  size_type	    size,
  MapMask	    permMask
  )
{

  if( mapFd >= 0 )
    unmap();
  
  unsigned short origMask = 0;

  if( permMask != 0777 )
    {
      origMask = umask( permMask );
    }

  if( ! (mapMode & ios::noreplace ) )
    unlink( fileName );

  if( (mapFd = open( fileName, OpenFlags( mapMode ), 0666 ) ) < 0 )
    {
      osErrno = errno;
      mapFd = -1;
      return( 0 );
    }

  fileStat( mapFd, true );
  
  if( permMask != 0777 )
    {
      umask( origMask );
    }
  
  return( setSize( size, baseAddr ) );
  
}

MapFile::size_type
MapFile::map(
  const char *	    fileName,
  MapAddr    	    baseAddr,
  ios::open_mode    mode
  )
{

  mapBase = 0;
  osErrno = 0;
  mapMode = mode;

  int mapProt = 0;
  int posixMode = 0;
  
  if( mapMode & ios::in )
    {
      if( mapMode & ios::out )
	{
	  mapProt = PROT_READ | PROT_WRITE;
	  posixMode = O_RDWR;
	}
      else
	{
	  mapProt = PROT_READ;
	  posixMode = O_RDONLY;
	}
    }
  else
    {
      if( mapMode & ios::out )
	{
	  mapProt = PROT_WRITE;
	  posixMode = O_WRONLY;
	}
      else
	{
	  osErrno = EPERM;
	  return(0);
	}
    }

  if( ! fileStat( fileName ).good() )
    {
      osErrno = fileStat.getSysError();
      return( 0 );
    }

  if( (mapFd = open( fileName, posixMode, 0 ) ) < 0 )
    {
      osErrno = errno;
      mapFd = -1;
      return(0);
    }

  mapSize = fileStat.getSize();

  int mapType = MAP_SHARED;
  
  if( baseAddr == 0 )
    {
      mapType |= MAP_VARIABLE;
    }
  else
    {
      mapType |= MAP_FIXED;
    }

  mapBase = (char *)mmap( baseAddr, mapSize, mapProt, mapType, mapFd, 0 );

  if( mapBase == MAP_FAILED )
    {
      mapSize = 0;
      osErrno = errno;
    }
  else
    {
      totalMapped += mapSize;
    }
  
  return( mapSize );
}
  
void
MapFile::unmap( void )
{
  if( mapBase != 0 && mapSize != 0 )
    {
      munmap( mapBase, mapSize );
      totalMapped -= mapSize;
      mapBase = 0;
      mapSize = 0;
    }
  
  if( mapFd >= 0 )
    {
      close( mapFd );
      mapFd = -1;
    }
}

bool
MapFile::sync(
  size_type	beg,
  size_type	len,
  bool		async
  )
{
  if( mapBase != 0 && mapSize != 0 )
    {
      if( beg >= mapSize )
	{
	  osErrno = EINVAL;
	  return( false );
	}
      
      // sync must begin and end on page boundries
      size_t	syncStart = RoundDown( beg, (int)pageSize );
      size_t	syncLen = 0;

      if( len == npos || (beg + len) >= mapSize )
	{
	  syncLen = Align( mapSize - syncStart, pageSize );
	}
      else
	{
	  syncLen = Align( syncStart - beg + len, pageSize );

	  if( syncStart + syncLen > mapSize )
	    {
	      syncLen = Align( mapSize - syncStart, pageSize );
	    }
	}
      
      if( msync( mapBase + syncStart, syncLen, (async ? MS_ASYNC : MS_SYNC ) ) )
	{
	  osErrno = errno;
	  return( false );
	}
      else
	{
	  return( true );
	}
    }
  else
    {
      return( false );
    }
}

#if defined( Sun5 ) && defined( STUPID )
static char sunWriteData[ 8192 * 8 ];
#endif

MapFile::size_type
MapFile::setSize(
  size_type    	size,
  MapAddr    	baseAddr
  )
{

  if( mapFd == 0 )
    return( 0 );
  

  if( (mapMode & ios::out ) == 0 )
    {
      osErrno = EPERM;
      return( 0 );
    }
      
  if( mapSize != 0 )
    {
      munmap( mapBase, mapSize );
      totalMapped -= mapSize;
    }
    

  
  if( size >= mapSize )
    {

#if defined( Sun5 ) && defined( STUPID )

      // THIS IS SLOW AND STUPID, BUT ...
      //
      // SunOS will randomly cause a SIGBUS when I use 'memory'
      // operations that write to a memory mapped file.
      //
      size_type	newSize = Align( size, pageSize );

      // seek to the current size
      if( lseek( mapFd, mapSize, SEEK_SET ) < 0 )
	{
	  osErrno = errno;
	  return( 0 );
	}

      size_type writeSize = newSize - mapSize;
      
      size_type written;

      while( writeSize > 0 )
	{
	  if( (written = write( mapFd,
				&sunWriteData,
				min( writeSize,
				     (size_type)sizeof( sunWriteData )) ))
	      < 0 )
	    {
	      if( errno != EINTR )
		{
		  osErrno = errno;
		  return( 0 );
		}
	    }
	  else
	    {
	      writeSize -= written;
	    }
	}
      
      mapSize = newSize;
      
#else
      mapSize = Align( size, pageSize );
      
      char	buf = 0;
      if( lseek( mapFd, mapSize - 1, SEEK_SET ) < 0 ||
	  write( mapFd, &buf, 1 ) != 1 ) 
	{
	  osErrno = errno;
	  return( 0 );
	}
#endif
    }
  else
    {
      mapSize = Align( size, pageSize );
      
      if( ftruncate( mapFd, mapSize ) != 0)
	{
	  osErrno = errno;
	  return( 0 );
	}
    }

  int mapType = ( MAP_SHARED | ( baseAddr == 0 ? MAP_VARIABLE : MAP_FIXED ) );
  int mapProt = ( PROT_READ | ( ( mapMode | ios::out ) ? PROT_WRITE : 0 ) );
    
  mapBase = (char *)mmap( baseAddr, mapSize, mapProt, mapType, mapFd, 0 );

  if( mapBase == MAP_FAILED )
    {
      mapSize = 0;
      osErrno = errno;
    }
  else
    {
      totalMapped += mapSize;
    }

  return( mapSize );
}

const FileStat &
MapFile::getFileStat( void ) const
{
  return( fileStat );
}

const char *
MapFile::getFileName( void ) const
{
  return( fileStat.getName() );
}

const char *
MapFile::getAccess( void  ) const
{
  if( mapMode & ios::in )
    {
      if( mapMode & ios::out )
	{
	  return( "RW" );
	}
      else
	{
	  return( "R" );
	}
    }
  else
    {
      return( "W" );
    }
}

MapFile::size_type
MapFile::getPageSize( void )
{
  return( getpagesize() );
}


MapFile::size_type
MapFile::getTotalMapped( void )
{
  return( totalMapped );
}


bool
MapFile::good( void ) const
{
  return( osErrno == 0 );
}

const char *
MapFile::error( void ) const
{
  static Str errStr;
  errStr.reset();
  
  errStr << MapFile::getClassName();
  
  if( osErrno == 0 )
    {
      errStr << ": ok";
    }
  else
    {
      errStr << ": '" << fileStat.getName() << "' - " << strerror( osErrno );
    }
  return( errStr.cstr() );
}

int
MapFile::getErrno( void ) const
{
  return( osErrno );
}

const char *
MapFile::getClassName( void ) const
{
  return( "MapFile" );
}

const char *
MapFile::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
MapFile::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << MapFile::getClassName() << ":\n"
	 << MapFile::getVersion() << '\n' ;
      
  if( ! MapFile::good() )
    dest << prefix << "Error: " << MapFile::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  dest << prefix << "name:          " << fileStat.getName() << '\n'
       << prefix << "perm:          " << fileStat.getModeString() << '\n'
       << prefix << "owner/group:   " << fileStat.getUserName() << '/'
       <<                                fileStat.getGroupName() << '\n'
       << prefix << "access:        " << getAccess() << '\n'
       << prefix << "map size:      " << getSize() << '\n'
       << prefix << "base addr:     " << (void *)getBase() << '\n'
       << prefix << "end addr:      " << (void *)getEnd() << '\n'
       << prefix << "total mapped:  " << totalMapped << '\n'
       << prefix << "page size:     " << getPageSize() << endl
    ;

  return( dest );
}
  


// Revision Log:
//
// $Log$
// Revision 4.2  2003/07/26 15:58:05  houghton
// Bug-Fix: use MAP_FAILED to detect mmap errros.
//
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.20  2000/07/31 13:06:05  houghton
// Change Sun5 don't prewrite map files (os bug fixed).
//
// Revision 2.19  1999/03/02 12:58:00  houghton
// Added totalMapped static variable.
//
// Revision 2.18  1998/03/18 06:41:36  houghton
// Changed for Sun5 - the kernel does not allocate file space
//     properly. I'm hoping 'writing' to the map file first will keep me
//     from getting random SIGBUSs.
//
// Revision 2.17  1998/01/09 10:42:41  houghton
// Bug-Fix: sync() was not handling default args (len == npos) correctly.
//
// Revision 2.16  1997/10/22 16:06:29  houghton
// Cleanup.
//
// Revision 2.15  1997/10/01 14:00:10  houghton
// Added sync().
//
// Revision 2.14  1997/09/17 16:56:00  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.13  1997/08/25 10:35:34  houghton
// Changed so the noreplace (and other) ios::open_modes can be used when
//     createding a map.
//
// Revision 2.12  1997/07/25 13:47:02  houghton
// Cleanup.
// Changed so an invalid mapFd value is -1.
//
// Revision 2.11  1997/07/19 10:25:50  houghton
// Port(Sun5): moved secializations to new _MdbMapMem.hh header.
//
// Revision 2.10  1997/07/16 16:37:38  houghton
// Bug-Fix: if create is true mode needs to be set to ios::in|ios::out.
//
// Revision 2.9  1997/07/15 20:21:37  houghton
// Bug-Fix: Reset values to 0 in unmap.
//
// Revision 2.8  1997/07/13 11:14:52  houghton
// Cleanup
//
// Revision 2.7  1997/06/05 11:19:10  houghton
// Cleanup.
// Change to be part of libMdb (vs StlUtils1).
// Added types size_type, MapAddr, MapMask and change to use them.
// Added constructor that can create or open existing.
// Added createMap and openMap methods.
//
// Revision 2.6  1997/04/21 12:11:59  houghton
// Added getErrno.
//
// Revision 2.5  1997/04/04 20:48:54  houghton
// Cleanup.
//
// Revision 2.4  1997/03/07 11:48:23  houghton
// Add dumpInfo.
//
// Revision 2.3  1997/03/03 14:32:06  houghton
// Moved construtors to .C from .hh (no longer inline).
//
// Revision 2.2  1996/02/29 19:09:25  houghton
// Added case to return from mmap call.
//
// Revision 2.1  1995/11/10 12:42:28  houghton
// Change to Version 2
//
// Revision 1.4  1995/11/05  16:32:32  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:48  houghton
// New Style Avl an memory management. Many New Classes
//
//
// Copyright:
//
//              This software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1994 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//

