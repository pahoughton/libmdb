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

#include <Str.hh>
#include <ClueUtils.hh>

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#ifdef Linux
#include <sys/stat.h>

//
// linux has an else construct on MAP_FIXED, so it does
// not have a MAP_VARIABLE. I define it here as 0 so it will be ignored
// 
#define MAP_VARIABLE  0
#endif

#if defined( Linux )
extern "C" size_t getpagesize( void );
#endif

#if defined( MDB_DEBUG )
#include "MapFile.ii"
#endif

MDB_VERSION(
  MapFile,
  "$Id$");


MapFile::MapFile()
  : mapFd( 0 ),
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
    mapFd( 0 ),
    mapMode( mode ),
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
    mapFd( 0 ),
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
    mapFd( 0 ),
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

  if( mapBase == (MapAddr)-1 )
    {
      mapSize = 0;
      osErrno = errno;
    }
  
  return( mapSize );
}
  
void
MapFile::unmap( void )
{
  if( mapBase != 0 && mapSize != 0 )
    {
      munmap( mapBase, mapSize );
      mapBase = 0;
      mapSize = 0;
    }
  
  if( mapFd != 0 )
    {
      close( mapFd );
      mapFd = 0;
    }
}
  
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
    munmap( mapBase, mapSize );
    

  
  if( size >= mapSize )
    {
      mapSize = Align( size, pageSize );
      
      char	buf = 0;
      if( lseek( mapFd, mapSize - 1, SEEK_SET ) < 0 ||
	  write( mapFd, &buf, 1 ) != 1 ) 
	{
	  osErrno = errno;
	  return( 0 );
	}
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

  if( mapBase == (MapAddr)-1 )
    {
      mapSize = 0;
      osErrno = errno;
    }

  return( mapSize );
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
      errStr << ": " << fileStat.getName() << " - " << strerror( osErrno );
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
       << prefix << "page size:     " << getPageSize() << endl
    ;

  return( dest );
}
  
void
MapFile::createMap(
  const char *	    fileName,
  MapAddr	    baseAddr,
  size_type	    size,
  MapMask	    permMask
  )
{
  unsigned short origMask = 0;

  if( permMask != 0777 )
    {
      origMask = umask( permMask );
    }

  unlink( fileName );

  if( (mapFd = open( fileName, O_RDWR | O_CREAT, 0666 ) ) < 0 )
    {
      osErrno = errno;
      return;
    }

  fileStat( mapFd, true );
  
  if( permMask != 0777 )
    {
      umask( origMask );
    }
  
  setSize( size, baseAddr );
  
}


// Revision Log:
//
// $Log$
// Revision 2.9  1997/07/15 20:21:37  houghton
// Bug-Fix: Reset values to 0 in unmap.
//
// Revision 2.8  1997/07/13 11:14:52  houghton
// Cleanup
//
// Revision 2.7  1997/06/05 11:19:10  houghton
// Cleanup.
// Change to be part of libMdb (vs Clue1).
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

