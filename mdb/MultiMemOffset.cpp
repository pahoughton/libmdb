// 1995-01-28 (cc) Paul Houghton <paul4hough@gmail.com>

#include "MultiMemOffset.hpp"
#include <clue/Str.hpp>

namespace mdb {

const MultiMemOffset::Loc MultiMemOffset::badLoc( 0 );

MultiMemOffset::MultiMemOffset( void )
{
}

MultiMemOffset::~MultiMemOffset( void )
{
}

bool
MultiMemOffset::good( void ) const
{
  return( true );
}

const char *
MultiMemOffset::error( void ) const
{
  static clue::Str errStr;

  errStr = "MultiMemOffset";

  if( good() )
    {
      errStr << ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}


std::ostream &
MultiMemOffset::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! MultiMemOffset::good() )
    dest << prefix << "Error: " << MultiMemOffset::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  return( dest );
}

}; // namespace mdb
