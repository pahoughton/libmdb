#ifndef _MapFile_hh_
#define _MapFile_hh_
//
// File:        MapFile.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/11/94 06:46
//
// Revision History:
//
// 
// $Log$
// Revision 2.3  1997/03/03 14:32:22  houghton
// Moved construtors to .C from .hh (no longer inline).
// Added virtual destructor.
//
// Revision 2.2  1996/02/29 19:09:49  houghton
// *** empty log message ***
//
// Revision 2.1  1995/11/10 12:42:28  houghton
// Change to Version 2
//
// Revision 1.4  1995/11/05  16:32:33  houghton
// Revised
//
// Revision 1.2  1995/03/02  16:35:35  houghton
// Linux ports & new Classes
//
// Revision 1.1  1995/02/13  16:08:48  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <ClueConfig.hh>

#include <FileStat.hh>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream.h>
#include <errno.h>

class MapFile
{

public:

  // use this constructor to create a new file
  MapFile( const char *     fileName,
	   size_t   	    size,
 	   caddr_t   	    baseAddr = 0,
	   unsigned short   permMask = 0);

  // use this constructor to open an existing file
  MapFile( const char *     fileName,
	   caddr_t   	    baseAddr = 0,
	   ios::open_mode   mode = ios::in );

  MapFile();
  
  virtual ~MapFile( void );

  size_t    	    map( const char * 	fileName,
			 caddr_t      	baseAddr = 0,
			 ios::open_mode	mode = ios::in );

  inline void	    unmap( void );
  
  size_t	    setSize( size_t amount, caddr_t baseAddr );
  size_t    	    grow( size_t amount, caddr_t baseAddr );
  size_t    	    shrink( size_t amount, caddr_t baseAddr );

  const char *	    getFileName( void ) const;
  const char * 	    getAccess( void ) const;
  size_t    	    getSize( void ) const;
  caddr_t    	    getBase( void ) const;
  caddr_t    	    getEnd( void ) const;
  size_t    	    getPageSize( void  ) const;

  virtual ostream & 	getStats( ostream & dest ) const;

  virtual const char * 	getClassName( void ) const { return "MapFile"; };
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;

  friend inline ostream & operator<<( ostream & dest, const MapFile & map );
  
protected:

private:

  MapFile( const MapFile & copyFrom );
  MapFile & operator=( const MapFile & assignFrom );

  FileStat  	    fileStat;

  int	    	    mapFd;
  ios::open_mode    mapMode;
  size_t    	    mapSize;
  caddr_t    	    mapBase;

  size_t    	    pageSize;

  int	    	    osErrno;

};


//
// Inline methods
//

inline
void
MapFile::unmap( void )
{
  
  if( mapBase != 0 && mapSize != 0 )
    {
      munmap( mapBase, mapSize );
    }

  if( mapFd != 0 )
    {
      close( mapFd );
    }
}


inline
size_t
MapFile::grow(
  size_t    amount,
  caddr_t   baseAddr
  )
{
  return( setSize( mapSize + amount, baseAddr ) ); 
}

inline
size_t
MapFile::shrink(
  size_t    amount,
  caddr_t   baseAddr
  )
{
  return( setSize( mapSize - amount, baseAddr ) );
}


inline
const char *
MapFile::getFileName( void ) const
{
  return( fileStat.getName() );
}

inline
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

inline
size_t
MapFile::getSize( void ) const
{
  return( mapSize );
}

inline
caddr_t
MapFile::getBase( void ) const
{
  return( mapBase );
}

inline
caddr_t
MapFile::getEnd( void ) const
{
  return( ((char *)mapBase) + mapSize );
}

inline
size_t
MapFile::getPageSize( void ) const
{
  return( pageSize );
}

inline
bool
MapFile::good( void ) const
{
  return( osErrno == 0 );
}

inline
ostream &
operator<<( ostream & dest, const MapFile & map )
{
  return( map.getStats( dest ) );
}

#endif // ! def _MapFile_hh_ 
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
