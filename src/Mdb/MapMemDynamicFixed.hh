#ifndef _MapMemFixedDynamic_hh_
#define _MapMemFixedDynamic_hh_
//
// File:        MapMemFixedDynamic.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/18/94 10:09
//
// Revision History:
//
// 
// $Log$
// Revision 1.1  1995/02/13 16:08:50  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <MapMem.hh>
#include <iostream.h>

#define MMF_VERSION 0x4d4d4601	// 'MMF1'

#define NUM_KEYS    16

class MapMemFixedDynamic : public MapMem
{

public:

  enum MapFixedDynamicError
  {
    E_OK,
    E_MAPMEM,
    E_BADSIZE,
    E_UNDEFINED
  };
    
  // use this constructor to create a new map file  
  MapMemFixedDynamic( const char * 	fileName,
		      size_t		recSize,
		      unsigned long	numRecs = 0,
		      unsigned short	permMask = 0 );

  // use this constructor to access an existing map file  
  MapMemFixedDynamic( const char * 	fileName,
	       ios::open_mode	mode = (ios::open_mode)(ios::in|ios::out) );

  
  off_t	    	    getMem( size_t size = 0 );	// returns offset not addr!
  void	    	    freeMem( off_t  offset ); 	// needs offset not addr!
  
  void *    	    getAddr( off_t offset ) const;
  off_t	    	    getOffset( void * addr ) const;

  size_t	    getRecSize( void ) const;
  size_t	    getChunkSize( void ) const;
  unsigned long	    getRecCount( void ) const;
  unsigned long	    getFreeRecCount( void ) const;
  
  long	    	    setKey( long value, unsigned short key = 0 );
  long	    	    getKey( unsigned short key = 0) const;
  
  void 	    	    expand( void );
  
  virtual ostream & 	getStats( ostream & dest ) const;

  virtual const char * 	getClassName( void ) const;
  virtual Bool	    	good( void ) const;
  virtual const char *	error( void ) const;
  
  friend ostream & operator<<( ostream & dest, const MapMemFixedDynamic & mmf );
  
  struct FreeList
  {
    unsigned long next;
    unsigned long prev;
  };

protected:

private:

  static const char * ErrorStrings[];
  
  MapMemFixedDynamic( const MapMemFixedDynamic & copyFrom );
  MapMemFixedDynamic & operator=( const MapMemFixedDynamic & assignFrom );
  
  struct MapFixedDynamicInfo : MapInfo
  {
    unsigned long   recSize;	// record size
    unsigned long   chunkSize;	// records to allocate at a time
    unsigned long   recCount;	// allocated records
    unsigned long   freeCount;	// available records
    long    	    keys[NUM_KEYS];	// general purpose values
    struct FreeList freeList;	// head to list of free records
  };

  MapFixedDynamicError	    	mapFixedDynamicError;
  struct MapFixedDynamicInfo * base;  
  
};


//
// Inline methods
//


inline
MapMemFixedDynamic::MapMemFixedDynamic(
  const char * 	    fileName,
  ios::open_mode    mode
  )
  : MapMem( fileName, MM_FIXED, MMF_VERSION, mode )
{
  base = (MapFixedDynamicInfo *)MapMem::getMapInfo();
  
  if( base == 0 || ! MapMem::good() )
    {
      mapFixedDynamicError = E_MAPMEM;
    }
  else
    {      
      mapFixedDynamicError = E_OK;
    }
}

inline
size_t
MapMemFixedDynamic::getRecSize( void ) const
{
  return( base->recSize );
}

inline
size_t
MapMemFixedDynamic::getChunkSize( void ) const
{
  return( base->chunkSize );
}

inline
unsigned long
MapMemFixedDynamic::getRecCount( void ) const
{
  return( base->recCount );
}

inline
unsigned long
MapMemFixedDynamic::getFreeRecCount( void ) const
{
  return( base->freeCount );
}

    
inline
off_t
MapMemFixedDynamic::getOffset( void * addr ) const
{
  return( (off_t)addr - (off_t)base );
}


inline
void *
MapMemFixedDynamic::getAddr( off_t offset ) const
{
  return( (void *) ((caddr_t)base + offset ));
}

inline
long
MapMemFixedDynamic::getKey( unsigned short key ) const
{
  if( base && key < NUM_KEYS )
    {
      return( base->keys[key] );
    }
  else
    {
      return( 0 );
    }
}

inline
long
MapMemFixedDynamic::setKey(
  long	    	 value,
  unsigned short key
  )
{
  if( key >= NUM_KEYS )
    {
      return(0);
    }
  
  long old = getKey( key );
  
  base->keys[key] = value;

  return( old );
}

inline
const char *
MapMemFixedDynamic::getClassName( void ) const
{
  return( "MapMemFixedDynamic" );
}

inline
Bool
MapMemFixedDynamic::good( void ) const
{
  return( base != 0 &&
	  mapFixedDynamicError == E_OK &&
	  MapMem::good() );
}


#endif // ! def _MapMemFixedDynamic_hh_ 
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
