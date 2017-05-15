// 1997-05-20 (cc) Paul Houghton <paul4hough@gmail.com>

#include "MapMemDynamic.hpp"
#include <clue/Str.hpp>
#include <cstring>

#include <iomanip>

namespace mdb {

const unsigned short MapMemDynamic::numKeys( MMD_NUM_KEYS );

MapMemDynamic::MapMemDynamic(
  const char *		fileName,
  MapVersion		mapVersion,
  std::ios::openmode    mode,
  bool			create,
  size_type		size,
  MapMask		permMask
  )
  : MapMem( fileName, MM_DYNAMIC, mapVersion, mode, create, 0, size, permMask )
{
  if( create )
    createMapMemDynamic();
  else
    openMapMemDynamic();
}

MapMemDynamic::MapMemDynamic(
  const char *	    fileName,
  MapVersion	    mapVersion,
  size_type	    size,
  MapMask	    permMask
  )
  : MapMem( fileName, 0, MM_DYNAMIC, mapVersion, size, permMask )
{
  createMapMemDynamic();
}

MapMemDynamic::MapMemDynamic(
  const char *		fileName,
  MapVersion		mapVersion,
  std::ios::openmode    mode,
  bool			overrideOwner
  )
  : MapMem( fileName, MM_DYNAMIC, mapVersion, mode, overrideOwner )
{
  openMapMemDynamic();
}

MapMemDynamic::~MapMemDynamic( void )
{
}

bool
MapMemDynamic::good( void ) const
{
  return( MapMem::good() );
}

const char *
MapMemDynamic::error( void ) const
{
  static clue::Str errStr;

  errStr = "MapMemDynamic";

  if( good() )
    {
      errStr << ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! MapMem::good() )
	errStr << ": " << MapMem::error();

      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

std::ostream &
MapMemDynamic::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{
  if( ! MapMemDynamic::good() )
    dest << prefix << "Error: " << MapMemDynamic::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  clue::Str pre;
  pre = prefix;
  pre << "MapMem::";

  MapMem::dumpInfo( dest, pre );

  if( mapInfo() )
    {
      dest << prefix << "allocCount:   " << mapInfo()->allocCount << '\n'
	   << prefix << "freeCount:    " << mapInfo()->freeCount << '\n'
	;

      for( int k = 0; k < numKeys; k++ )
	{
	  if( getKey( k ) != 0 )
	    {
	      dest << prefix
		   << "key(" << std::setw(2) << k
		   << "):      " << getKey(k) << '\n'
		;
	    }
	}
    }
  else
    {
      dest << prefix << "No Base Addr!\n";
    }


  return( dest );
}

void
MapMemDynamic::createMapMemDynamic( void )
{
  if( mapInfo() )
    {
      mapInfo()->allocCount = 0;
      mapInfo()->freeCount = 0;
      memset( mapInfo()->keys, 0, sizeof( mapInfo()->keys ) );
    }
}

void
MapMemDynamic::openMapMemDynamic( void )
{
}

}; // namespace mdb
