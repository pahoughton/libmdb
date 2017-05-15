// 1994-11-18 (cc) Paul Houghton <paul4hough@gmail.com>

#include "MapMem.hpp"
#include <clue/Str.hpp>

#include <unistd.h>
#include <fcntl.h>

namespace mdb {

const char * MapMem::ErrorStrings[] =
{
  ": ok",
  ": ",
  ": file to small",
  ": incorect version",
  ": map type mismatch",
  ": already owned by another process",
  0
};

const char * MapMem::TypeStrings[] =
{
  "Fixed addr",
  "Dynamic addr",
  0
};


//
// Constructor for creating a new map or opening an existing one.
//
MapMem::MapMem(
  const char *		fileName,
  MapType		type,
  MapVersion		mapVersion,
  std::ios::openmode    mode,
  bool			create,
  MapAddr		baseAddr,
  size_type		size,
  MapMask		permMask
  )
  : MapFile( fileName, baseAddr, mode, create, size, permMask ),
    errorNum( E_OK ),
    osErrno( 0 )
{
  if( create )
    createMapMem( type, mapVersion, baseAddr );
  else
    openMapMem( fileName, type, mapVersion, mode, false );
}


//
// Constructor for creating a new map
//
MapMem::MapMem(
  const char * 	    fileName,
  MapAddr	    baseAddr,
  MapType   	    type,
  MapVersion	    mapVersion,
  size_type	    size,
  MapMask	    permMask
  )
  : MapFile( fileName, size, baseAddr, permMask )
{
  createMapMem( type, mapVersion, baseAddr );
}


// constructor for opening an existing map
MapMem::MapMem(
  const char *		fileName,
  MapType		type,
  MapVersion		mapVersion,
  std::ios::openmode    mode,
  bool			overrideOwner
  )
  : MapFile( fileName, 0, mode )
{
  openMapMem( fileName, type, mapVersion, mode, overrideOwner );
}

MapMem::~MapMem( void )
{
  if( mapInfo() && mapInfo()->owner == getpid() )
    mapInfo()->owner = 0;
}

MapMem::MapVersion
MapMem::getMapVersion( void ) const
{
  return( (mapInfo()) ? mapInfo()->version : 0 );
}

MapMem::MapType
MapMem::getType( void  ) const
{
  return( (mapInfo()) ? (MapType)mapInfo()->type : MM_UNDEFINED );
}

MapMem::MapOwner
MapMem::getOwner( void ) const
{
  return( mapInfo() ? mapInfo()->owner : -1 );
}

const char *
MapMem::getTypeName( void ) const
{
  return( TypeStrings[ getType() ]  );
}

bool
MapMem::good( void ) const
{
  return( errorNum == E_OK && MapFile::good() );
}


const char *
MapMem::error( void ) const
{
  static clue::Str errStr;
  errStr.reset();

  errStr << "MapMem";

  if( good() )
    {
      errStr << ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! MapFile::good() )
	errStr << ": " << MapFile::error();

      switch( errorNum )
	{
	case E_OWNER:
	  errStr << ": already owned by "
		 << ( mapInfo() ? mapInfo()->owner : -1 );
	  break;

	default:
	  if( errorNum > E_MAPFILE && errorNum < E_UNDEFINED )
	    errStr << ErrorStrings[ errorNum ];
	  break;
	}

      if( eSize == errStr.size() )
        errStr << ": unknown error";

    }
  return( errStr.cstr() );
}


std::ostream &
MapMem::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! MapMem::good() )
    dest << prefix << "Error: " << MapMem::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  clue::Str pre;
  pre = prefix;
  pre << "MapFile::";

  MapFile::dumpInfo( dest, pre );

  if( mapInfo() != 0 )
    {
      dest << prefix << "version:  " << getMapVersion() << '\n'
	   << prefix << "type:     " << getTypeName() << '\n'
	   << prefix << "map to:   " << (void *)getMapToAddr() << '\n'
	   << prefix << "map size: " << getMapSize() << '\n'
	   << prefix << "owner:    " << mapInfo()->owner << '\n'
	;
    }
  else
    {
      dest << prefix << "No Map info available!" << '\n';
    }

  return( dest );
}

void
MapMem::createMapMem( MapType type, MapVersion mapVersion, MapAddr baseAddr )
{
  osErrno = 0;

  if( mapInfo() != 0 )
    {
      mapInfo()->type	    = type;
      mapInfo()->version    = mapVersion;
      mapInfo()->base	    = (MapBaseAddr)baseAddr;
      mapInfo()->size	    = getSize();
      mapInfo()->owner	    = getpid();
      errorNum = E_OK;
    }
  else
    {
      errorNum = E_MAPFILE;
    }
}

void
MapMem::openMapMem(
  const char *		fileName,
  MapType		type,
  MapVersion		mapVersion,
  std::ios::openmode    mode,
  bool			overrideOwner
  )
{
  if( ! MapFile::good() )
    return;

  if( getSize() < sizeof( MapInfo ) )
    {
      errorNum = E_FILESIZE;
      return;
    }

  const MapInfo * info = mapInfo();

  if( ! info )
    return;

  if( info->type != type )
    {
      errorNum = E_BADTYPE;
      unmap();
      return;
    }

  if( info->version != mapVersion )
    {
      errorNum = E_VERSION;
      unmap();
      return;
    }


  MapAddr   mapToAddr = (MapAddr)info->base;

  unmap();

  if( map( fileName, mapToAddr, mode ) )
    {
      if( mode & std::ios::out )
	{
	  if( mapInfo()->owner && ! overrideOwner )
	    {
	      errorNum = E_OWNER;
	      return;
	    }
	  mapInfo()->owner = getpid();
	}
    }
  errorNum = E_OK;

}

}; // namespace clue
