//
// File:        MapFile.C
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/18/94 11:13 
//
// Revision History:
//
// $Log$
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
static const char * RcsId =
"$Id$";

#include "MapFile.hh"

#include <Str.hh>

#include <unistd.h>

#ifdef Linux
#include <sys/stat.h>
#include <fcntl.h>

//
// linux has an else construct on MAP_FIXED, so it does
// not have a MAP_VARIABLE. I define it here as 0 so it will be ignored
// 
#define MAP_VARIABLE  0
#endif


size_t
MapFile::map(
  const char *	    fileName,
  caddr_t    	    baseAddr,
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

  mapBase = mmap( baseAddr, mapSize, mapProt, mapType, mapFd, 0 );

  if( mapBase == (caddr_t)-1 )
    {
      mapSize = 0;
      osErrno = errno;
    }
  
  return( mapSize );
}
  
  
  
size_t
MapFile::setSize(
  size_t    	size,
  caddr_t    	baseAddr
  )
{

  if( mapFd == 0 )
    {
      return( 0 );
    }

  if( (mapMode & ios::out ) == 0 )
    {
      osErrno = EPERM;
      return( 0 );
    }
      
  if( mapSize != 0 )
    {
      munmap( mapBase, mapSize );
    }

  
  if( size >= mapSize )
    {
      mapSize = size + (pageSize - (size % pageSize) );
      
      char	buf = 0;
      if( lseek( mapFd, mapSize, SEEK_SET ) < 0 ||
	  write( mapFd, &buf, 1 ) != 1 ) 
	{
	  osErrno = errno;
	  return( 0 );
	}
    }
  else
    {
      mapSize = size + (pageSize - (size % pageSize) );
      
      if( ftruncate( mapFd, mapSize + 1 ) != 0)
	{
	  osErrno = errno;
	  return( 0 );
	}
    }
	

  int mapType = MAP_SHARED;
  
  if( baseAddr == 0 )
    {
      mapType |= MAP_VARIABLE;
    }
  else
    {
      mapType |= MAP_FIXED;
    }

  int mapProt = PROT_READ;

  if( mapMode | ios::out )
    {
      mapProt |= PROT_WRITE;
    }
    
  mapBase = mmap( baseAddr, mapSize, mapProt, mapType, mapFd, 0 );

  if( mapBase == (caddr_t)-1 )
    {
      mapSize = 0;
      osErrno = errno;
    }

  return( mapSize );
}

  
const char *
MapFile::error( void ) const
{
  static Str errStr;
  errStr.reset();
  
  errStr << MapFile::getClassName();
  
  if( osErrno == 0 )
    {
      errStr << ": Ok";
    }
  else
    {
      errStr << ": " << fileStat.getName() << " - " << strerror( osErrno );
    }
  return( errStr.cstr() );
}

ostream &
MapFile::getStats( ostream & dest ) const
{
  dest << MapFile::getClassName() << ": stats" << '\n'
       << "    Status:        " << MapFile::error() << '\n'
       << "    Name:          " << fileStat.getName() << '\n'
       << "    Perm:          " << fileStat.getModeString() << '\n'
       << "    Owner/Group:   " << fileStat.getUserName() << '/'
       <<                          fileStat.getGroupName() << '\n'
       << "    Access:        " << getAccess() << '\n'
       << "    Map Size:      " << getSize() << '\n'
       << "    Base Addr:     " << (void *)getBase() << '\n'
       << "    End Addr:      " << (void *)getEnd() << '\n'
       << "    Page Size:     " << getPageSize() << endl
    ;

  return( dest );
}

  

    
  

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
