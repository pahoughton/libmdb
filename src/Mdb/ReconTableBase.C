//
// File:        ReconTableBase.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for ReconTableBase
//  
// Author:      Paul Houghton 719-527-7834 - (paul.houghton@mci.com)
// Created:     07/31/00 06:13
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "ReconTableBase.hh"
#include <Str.hh>
#include <fstream>

#if defined( MDB_DEBUG )
#include "ReconTableBase.ii"
#endif

MDB_VERSION(
  ReconTableBase,
  "$Id$");

const ReconTableBase::RecordNum	    ReconTableBase::BadRec( -1 );

ReconTableBase::ReconTableBase(
  const FilePath &   tableFn,
  ios::open_mode     mode,
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
	  if( lock.lockwrite( 0, ios::beg, 0, false ) )
	    {
	      header().version  = Host2Net( tableVersion );
	      header().count	= Host2Net( 0 );
	      header().mapSize	= Host2Net( map.getSize() );
	      header().recSize	= Host2Net( recSize );
	      errorNum = E_OK;
	    }
	  lock.unlock();
	}
    }
  else
    {
      if( map.good() && lock.good() )
	{
	  if( ! lock.lockread( 0, ios::beg, sizeof( Header ) ) )
	    return;
	  
	  if( Net2Host( header().version ) != tableVersion )
	    errorNum = E_VERSION;
	  if( Net2Host( header().recSize ) != recSize )
	    errorNum = E_RECSIZE;
	  
	  lock.unlock( 0, ios::beg, sizeof( Header ) );
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

  if( ! lock.lockwrite( 0, ios::beg, sizeof( Header ) ) )
    return( false );

  if( Net2Host( header().mapSize ) != map.getSize() )
    {
      // someone else changed the map size, remap it
      if( ! map.map( map.getFileName(), 0, map.getMode() ) )
	{
	  lock.unlock( 0, ios::beg, sizeof( Header ) );
	  return( false );
	}
    }
  
  if( Net2Host( header().mapSize ) < ( sizeof(Header)
				    + (Net2Host( header().recSize ) *
				       ( Net2Host( header().count ) + 1))) )
    {
      if( ! map.grow( Net2Host( header().recSize ) * 128, 0 ) )
	{
	  lock.unlock();
	  return( false );
	}
      
      header().mapSize = Host2Net( map.getSize() );
    }

  if( ! lock.lockwrite( recpos( Net2Host( header().count ) ),
			ios::beg,
			Net2Host( header().recSize ) ) )
    {
      lock.unlock( 0, ios::beg, sizeof( Header ) );
      return( false );
    }
  
  memcpy( recptr( Net2Host( header().count ) ),
	  rec, 
	  Net2Host( header().recSize ) );
  
  lock.unlock( recpos( Net2Host( header().count ) ),
	       ios::beg,
	       Net2Host( header().recSize ) );

  header().count = Host2Net( Net2Host( header().count ) + 1 );
  lock.unlock( 0, ios::beg, sizeof( Header ) );
  
  return( good() );
}

bool
ReconTableBase::update( RecordNum r, const void * rec )
{
  if( r < Net2Host( header().count ) )
    {
      if( ! lock.lockwrite( recpos( r ),
			    ios::beg,
			    Net2Host( header().recSize ) ) )
	return( false );
      
      memcpy( recptr( r ), rec, Net2Host( header().recSize ) );
      
      lock.unlock( recpos( r ), ios::beg, Net2Host( header().recSize ) );
      
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
  static Str errStr;

  errStr = ReconTableBase::getClassName();

  if( good() )
    {
      errStr << ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! map.good() )
	errStr << ": " << map.error() << endl;

      if( ! lock.good() )
	errStr << ": " << lock.error() << endl;

      if( errorNum != E_OK )
	{
	  switch( errorNum )
	    {
	    case E_VERSION:
	      if( map.good() )
		{
		  errStr << ": wrong version '" << Net2Host( header().version )
			 << "' expected '" << tableVer
			 << '\''
		    ;
		}
	      break;

	    case E_RECSIZE:
	      if( map.good() )
		{
		  errStr << ": wrong rec size '"
			 << Net2Host( header().recSize )
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

const char *
ReconTableBase::getClassName( void ) const
{
  return( "ReconTableBase" );
}

const char *
ReconTableBase::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
ReconTableBase::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << ReconTableBase::getClassName() << ":\n"
	 << ReconTableBase::getVersion() << '\n';

  if( ! ReconTableBase::good() )
    dest << prefix << "Error: " << ReconTableBase::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << "map:";
  map.dumpInfo( dest, pre, false );

  pre = prefix;
  pre << "lock:";
  lock.dumpInfo( dest, pre, false );

  if( map.good() )
    {
      // make a copy so it doesn't change on me.
      Header hdr( header() );

      dest << "version:      " << Net2Host( hdr.version ) << '\n'
	   << "count:        " << Net2Host( hdr.count ) << '\n'
	   << "mapSize:      " << Net2Host( hdr.mapSize ) << '\n'
	   << "recSize:      " << Net2Host( hdr.recSize ) << '\n'
	;
    }
  

  return( dest );
}

ReconTableBase::VersionNum
ReconTableBase::getFileVersion( const FilePath & fn )
{
  
  VersionNum	ver( 0 );
  FileLock	lock( fn, ios::in );

  if( ! lock.lockread( 0, ios::beg, sizeof( Header ) ) )
    return( ver );

  ifstream  in( fn );

  ReconTableBase::Header	hdr;
  
  if( in.good() && in.read( (char *)&hdr, sizeof( hdr ) ).good() )
    {
      ver = Net2Host( hdr.version );
    }

  lock.unlock( 0, ios::beg, sizeof( Header ) );
  
  return( ver );
}


// Revision Log:
//
// $Log$
// Revision 4.2  2003/08/09 12:43:24  houghton
// Changed ver strings.
//
// Revision 4.1  2001/07/27 00:57:44  houghton
// Change Major Version to 4
//
// Revision 1.1  2000/08/02 11:03:47  houghton
// Initial Version.
//
//
