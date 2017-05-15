// 1997-06-30 (cc) Paul Houghton <paul4hough@gmail.com>

#include "MultiMemOffsetMapDynamic.hpp"
#include "MapMemDynamic.hpp"
#include <clue/Str.hpp>

namespace mdb {

MultiMemOffsetMapDynamic::MultiMemOffsetMapDynamic(
  MapMemDynamic *   mapMemMgr,
  bool		    delMemMgr
  )
  : mem( mapMemMgr ),
    delMem( delMemMgr )
{
}

MultiMemOffsetMapDynamic::~MultiMemOffsetMapDynamic( void )
{
  if( delMem )
    delete mem;
}

MultiMemOffsetMapDynamic::Loc
MultiMemOffsetMapDynamic::allocate( size_type size )
{
  return( mem->allocate( size ) );
}

void
MultiMemOffsetMapDynamic::release( Loc loc )
{
  mem->release( loc );
}

MultiMemOffsetMapDynamic::Addr
MultiMemOffsetMapDynamic::getBase( void )
{
  return( mem->getBase() );
}

const MultiMemOffsetMapDynamic::Addr
MultiMemOffsetMapDynamic::getBase( void ) const
{
  return( mem->getBase() );
}

MultiMemOffsetMapDynamic::KeyValue
MultiMemOffsetMapDynamic::getKey( unsigned short key ) const
{
  return( mem->getKey( key ) );
}

bool
MultiMemOffsetMapDynamic::reserveKey( unsigned short  key )
{
  return( mem->reserveKey( key ) );
}

bool
MultiMemOffsetMapDynamic::setNewKey(
  unsigned short    key,
  KeyValue	    value
  )
{
  return( mem->setNewKey( key, value ) );
}

bool
MultiMemOffsetMapDynamic::setKey(
  unsigned short    key,
  KeyValue	    value
  )
{
  return( mem->setKey( key, value ) );
}


bool
MultiMemOffsetMapDynamic::good( void ) const
{
  return( mem && mem->good() );
}

bool
MultiMemOffsetMapDynamic::allocCopyStr( Loc & loc, const clue::Str & src )
{
  bool	status( true );

  if( src.size() )
    {
      if( (loc = allocate( src.size() + 5 )) != 0 )
	{
	  char * tmp( address( loc ) );

	  src.copy( tmp, clue::Str::npos );
	}
      else
	{
	  status = false;
	}
    }
  else
    {
      loc = 0;
    }

  return( status );
}
const char *
MultiMemOffsetMapDynamic::error( void ) const
{
  static clue::Str errStr;

  errStr = "MultiMemOffsetMapDynamic";

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! mem )
	{
	  errStr << ": no mem mgr.";
	}
      else
	{
	  if( ! mem->good() )
	    errStr << ": " << mem->error();
	}

      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}


std::ostream &
MultiMemOffsetMapDynamic::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! MultiMemOffsetMapDynamic::good() )
    dest << prefix << "Error: " << MultiMemOffsetMapDynamic::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  if( mem )
    {
      clue::Str pre;
      pre = prefix;
      pre << "mem::";
      mem->dumpInfo( dest, pre );
    }
  else
    {
      dest << prefix << "mem:      (NONE)" << '\n';
    }

  return( dest );
}

}; // namespace mdb
