// 2000-07-31 (cc) Paul Houghton <paul4hough@gmail.com>

#include "ReconTableBase.hpp"
#include <clue/Str.hpp>
#include <fstream>

namespace mdb {

const ReconTableBase::RecordNum	    ReconTableBase::BadRec( -1 );

ReconTableBase::ReconTableBase(
  const char *	     tableFn,
  std::ios::openmode mode,
  bool               create,
  MapFile::MapMask   permMask,
  unsigned int	     tableVersion,
  unsigned int	     recordSize
  )
  : map( tableFn,
	 0,
	 mode,
	 create,
	 sizeof( Header ) + (recSize * 1024),
	 permMask ),
    lock( tableFn, mode ),
    tableVer( tableVersion ),
    recSize( recordSize ),
    errorNum( E_OK )
{
  if( create )
    {
      if( map.good() && map.getSize() > sizeof( Header ) )
	{
	  if( lock.lockwrite( 0, std::ios::beg, 0, false ) )
	    {
	      header().version  = tableVersion;
	      header().count	= 0;
	      header().mapSize	= map.getSize();
	      header().recSize	= recSize;
	      errorNum = E_OK;
	    }
	  lock.unlock();
	}
    }
  else
    {
      if( map.good() && lock.good() )
	{
	  if( ! lock.lockread( 0, std::ios::beg, sizeof( Header ) ) )
	    return;

	  if( header().version != tableVersion )
	    errorNum = E_VERSION;
	  if( header().recSize != recSize )
	    errorNum = E_RECSIZE;

	  lock.unlock( 0, std::ios::beg, sizeof( Header ) );
	}
    }
}

ReconTableBase::~ReconTableBase( void )
{
}

bool
ReconTableBase::append( const void * rec )
{
  if( ! good() )
    return( false );

  if( ! lock.lockwrite( 0, std::ios::beg, sizeof( Header ) ) )
    return( false );

  if( header().mapSize != map.getSize() )
    {
      // someone else changed the map size, remap it
      if( ! map.map( map.getFileName(), 0, map.getMode() ) )
	{
	  lock.unlock( 0, std::ios::beg, sizeof( Header ) );
	  return( false );
	}
    }

  if( header().mapSize < ( sizeof(Header)
			   + (header().recSize * ( header().count ) + 1)))
    {
      if( ! map.grow( header().recSize * 128, 0 ) )
	{
	  lock.unlock();
	  return( false );
	}

      header().mapSize = map.getSize();
    }

  if( ! lock.lockwrite( recpos( header().count ),
			std::ios::beg,
			header().recSize ) )
    {
      lock.unlock( 0, std::ios::beg, sizeof( Header ) );
      return( false );
    }

  memcpy( recptr( header().count ),
	  rec,
	  header().recSize );

  lock.unlock( recpos( header().count ),
	       std::ios::beg,
	       header().recSize );

  header().count = header().count + 1;
  lock.unlock( 0, std::ios::beg, sizeof( Header ) );

  return( good() );
}

bool
ReconTableBase::update( RecordNum r, const void * rec )
{
  if( r < header().count )
    {
      if( ! lock.lockwrite( recpos( r ),
			    std::ios::beg,
			    header().recSize ) )
	return( false );

      memcpy( recptr( r ), rec, header().recSize );

      lock.unlock( recpos( r ), std::ios::beg, header().recSize );

      return( true );
    }
  else
    {
      return( false );
    }
}

bool
ReconTableBase::good( void ) const
{
  return( map.good() && lock.good() && errorNum == E_OK );
}

const char *
ReconTableBase::error( void ) const
{
  static clue::Str errStr;

  errStr = "ReconTableBase";

  if( good() )
    {
      errStr << ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! map.good() )
	errStr << ": " << map.error() << std::endl;

      if( ! lock.good() )
	errStr << ": " << lock.error() << std::endl;

      if( errorNum != E_OK )
	{
	  switch( errorNum )
	    {
	    case E_VERSION:
	      if( map.good() )
		{
		  errStr << ": wrong version '" << header().version
			 << "' expected '" << tableVer
			 << '\''
		    ;
		}
	      break;

	    case E_RECSIZE:
	      if( map.good() )
		{
		  errStr << ": wrong rec size '"
			 << header().recSize
			 << "' expected '" << recSize
			 << '\''
		    ;
		}
	    default:
	      break;
	    }
	}
      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}


std::ostream &
ReconTableBase::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! ReconTableBase::good() )
    dest << prefix << "Error: " << ReconTableBase::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  clue::Str pre;
  pre = prefix;
  pre << "map:";
  map.dumpInfo( dest, pre );

  pre = prefix;
  pre << "lock:";
  lock.dumpInfo( dest, pre );

  if( map.good() )
    {
      // make a copy so it doesn't change on me.
      Header hdr( header() );

      dest << "version:      " << hdr.version << '\n'
	   << "count:        " << hdr.count << '\n'
	   << "mapSize:      " << hdr.mapSize << '\n'
	   << "recSize:      " << hdr.recSize << '\n'
	;
    }


  return( dest );
}

ReconTableBase::VersionNum
ReconTableBase::getFileVersion( const char * fn )
{

  VersionNum	    ver( 0 );
  clue::FileLock    lock( fn, std::ios::in );

  if( ! lock.lockread( 0, std::ios::beg, sizeof( Header ) ) )
    return( ver );

  std::ifstream  in( fn );

  ReconTableBase::Header	hdr;

  if( in.good() && in.read( (char *)&hdr, sizeof( hdr ) ).good() )
    {
      ver = hdr.version;
    }

  lock.unlock( 0, std::ios::beg, sizeof( Header ) );

  return( ver );
}

}; // namespace mdb
