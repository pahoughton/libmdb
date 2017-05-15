// 1995-01-28 (cc) Paul Houghton <paul4hough@gmail.com>

#include "MultiMemOffsetMapFixed.hpp"
#include <clue/Str.hpp>

namespace mdb {

MultiMemOffsetMapFixed::MultiMemOffsetMapFixed(
  const char * 	    fileName,
  std::ios::openmode    mode
  )
  : mem( fileName, mode )
{
}

MultiMemOffsetMapFixed::MultiMemOffsetMapFixed(
  const char * 	    fileName,
  size_type    	    recSize,
  size_type    	    numRecs,
  MapMask	    permMask
  )
  : mem( fileName, recSize, numRecs, permMask )
{
}

MultiMemOffsetMapFixed::~MultiMemOffsetMapFixed( void )
{
}

bool
MultiMemOffsetMapFixed::good( void ) const
{
  return( mem.good() );
}

const char *
MultiMemOffsetMapFixed::error( void ) const
{
  static clue::Str errStr;

  errStr = "MultiMemOffsetMapFixed";

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! mem.good() )
	errStr << ": " << mem.error();

      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

std::ostream &
MultiMemOffsetMapFixed::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! MultiMemOffsetMapFixed::good() )
    dest << prefix << "Error: " << MultiMemOffsetMapFixed::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  clue::Str pre;
  pre = prefix;
  pre << "mem:";

  mem.dumpInfo( dest, pre );

  return( dest );
}

}; // namespace mdb
