#ifndef _MapMem_hh_
#define _MapMem_hh_
//
// File:        MapMem.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/18/94 10:06
//
// Revision History:
//
// 
// $Log$
// Revision 1.1  1995/02/13 16:08:49  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <MapFile.hh>
  
class MapMem : public MapFile
{

public:

  enum MapType
  {
    MM_FIXED,
    MM_DYNAMIC,
    MM_UNDEFINED
  };

  enum MapError
  {
    E_OK,
    E_OSERROR,
    E_MAPFILE,
    E_BADFILE,
    E_BADVER,
    E_BADTYPE,
    E_UNDEFINED
  };

  struct MapInfo
  {
    MapType 	    type;
    unsigned long   version;
    unsigned long   base;
    unsigned long   size;
  };

  // use this to create a new map
  MapMem( const char * 	    fileName,
	  caddr_t    	    baseAddr,
	  MapType   	    type,
	  unsigned long	    version,
	  unsigned long	    size,
	  unsigned short    permMask = 0 );

  // use this to open an existing map
  MapMem( const char * 	    fileName,
	  MapType   	    type,
	  unsigned long	    version,
	  ios::open_mode    mode = ios::in );

  
  unsigned long	    getVersion( void ) const;
  void *    	    getMapToAddr( void ) const;
  MapType   	    getType( void ) const;
  const char *	    getTypeName( void ) const;
  unsigned long	    getMapSize( void ) const;

  virtual ostream & 	getStats( ostream & dest ) const;

  virtual const char *	getClassName( void ) const { return "MapMem"; };
  virtual Bool	    	good( void ) const;
  virtual const char * 	error( void ) const;

  friend inline ostream & operator<<( ostream & dest, const MapMem & mm );
  
protected:

  MapInfo * 	getMapInfo( void );
  
private:

  static const char * ErrorStrings[];
  static const char * TypeStrings[];
  
  MapMem( const MapMem & copyFrom );
  MapMem & operator=( const MapMem & assignFrom );

  const char *  mapFileName;
  MapInfo * 	mapInfo;

  int	    	osErrno;
  MapError  	mapError;
};


//
// Inline methods
//

//
// Constructor for creating a new map
//
inline
MapMem::MapMem(
  const char * 	    fileName,
  caddr_t	    baseAddr,
  MapType   	    type,
  unsigned long	    version,
  unsigned long	    size,
  unsigned short    permMask
  )
  : MapFile( fileName, size, baseAddr, permMask )
{
  osErrno = 0;
  mapFileName = fileName;
  
  mapInfo = (MapInfo *)MapFile::getBase();
  
  if( mapInfo != 0 )
    {
      mapInfo->type     = type;
      mapInfo->version  = version;
      mapInfo->base	= (unsigned long)baseAddr;      
      mapInfo->size	= getSize();
      mapError = E_OK;
    }
  else
    {
      mapError = E_MAPFILE;
    }
}

inline
unsigned long
MapMem::getVersion( void ) const
{
  return( (mapInfo) ? mapInfo->version : 0 );
}
	
inline
void *
MapMem::getMapToAddr( void ) const
{
  return( (mapInfo) ? (void*)mapInfo->base : 0 );
}

inline
MapMem::MapType
MapMem::getType( void  ) const
{
  return( (mapInfo) ? mapInfo->type : MM_UNDEFINED );
}

inline
const char *
MapMem::getTypeName( void ) const
{
  return( TypeStrings[ getType() ]  );
}

inline
unsigned long
MapMem::getMapSize( void ) const
{
  return( (mapInfo) ? mapInfo->size : 0 );
}


inline
Bool
MapMem::good( void ) const
{
  return( mapError == E_OK && MapFile::good() );
}

inline
MapMem::MapInfo *
MapMem::getMapInfo( void )
{
  mapInfo = (MapInfo *)MapFile::getBase();
  return( mapInfo );
}

inline
ostream &
operator<<( ostream & dest, const MapMem & mm )
{
  return( mm.getStats( dest ) );
}

#endif // ! def _MapMem_hh_ 
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
