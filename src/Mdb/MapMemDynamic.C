//
// File:        MapMemDynamic.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for MapMemDynamic
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/20/97 06:14
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "MapMemDynamic.hh"
#include <Str.hh>
#include <cstring>

#include <iomanip>

#if defined( MDB_DEBUG )
#include "MapMemDynamic.ii"
#endif

MDB_VERSION(
  MapMemDynamic,
  "$Id$");

const unsigned short MapMemDynamic::numKeys( MMD_NUM_KEYS );

MapMemDynamic::MapMemDynamic(
  const char *	    fileName,
  MapVersion	    version,
  ios::open_mode    mode,
  bool		    create,
  size_type	    size,
  MapMask	    permMask
  )
  : MapMem( fileName, MM_DYNAMIC, version, mode, create, 0, size, permMask ),
    errorNum( E_OK )
{
  if( create )
    createMapMemDynamic();
  else
    openMapMemDynamic( mode, false );
}

MapMemDynamic::MapMemDynamic(
  const char *	    fileName,
  MapVersion	    version,
  size_type	    size,
  MapMask	    permMask
  )
  : MapMem( fileName, 0, MM_DYNAMIC, version, size, permMask ),
    errorNum( E_OK )
{
  createMapMemDynamic();
}

MapMemDynamic::MapMemDynamic(
  const char *	    fileName,
  MapVersion	    version,
  ios::open_mode    mode,
  bool		    overrideOwner
  )
  : MapMem( fileName, MM_DYNAMIC, version, mode ),
    errorNum( E_OK )
{
  openMapMemDynamic( mode, overrideOwner );
}

MapMemDynamic::~MapMemDynamic( void )
{
  if( mapInfo() && mapInfo()->owner == getpid() )
    mapInfo()->owner = 0;
}

bool
MapMemDynamic::good( void ) const
{
  return( errorNum == E_OK && MapMem::good() );
}

const char *
MapMemDynamic::error( void ) const
{
  static Str errStr;

  errStr = MapMemDynamic::getClassName();

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! MapMem::good() )
	errStr << ": " << MapMem::error();

      switch( errorNum )
	{
	case E_OWNER:
	  errStr << ": already owned by "
		 << ( mapInfo() ? mapInfo()->owner : -1 );
	  break;
	  
	default:
	  break;
	}

      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
MapMemDynamic::getClassName( void ) const
{
  return( "MapMemDynamic" );
}

const char *
MapMemDynamic::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
MapMemDynamic::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << MapMemDynamic::getClassName() << ":\n"
	 << MapMemDynamic::getVersion() << '\n';

  if( ! MapMemDynamic::good() )
    dest << prefix << "Error: " << MapMemDynamic::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << "::" << MapMem::getClassName();

  MapMem::dumpInfo( dest, pre, false );

  if( mapInfo() )
    {
      dest << prefix << "owner:        " << mapInfo()->owner << '\n'
	   << prefix << "chunkCount:   " << mapInfo()->chunkCount << '\n'
	   << prefix << "freeCount:    " << mapInfo()->freeCount << '\n'
	;
      
      for( int k = 0; k < numKeys; k++ )
	{
	  if( getKey( k ) != 0 )
	    {
	      dest << prefix
		   << "key(" << setw(2) << k << "):      " << getKey(k) << '\n'
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
  mapInfo()->owner = getpid();
  mapInfo()->chunkCount = 0;
  mapInfo()->freeCount = 0;
  memset( mapInfo()->keys, 0, sizeof( mapInfo()->keys ) );
};

void
MapMemDynamic::openMapMemDynamic(
  ios::open_mode    mode,
  bool		    overrideOwner
  )
{
  
  if( mode & ios::out )
    {
      if( mapInfo()->owner && ! overrideOwner )
	{
	  errorNum = E_OWNER;
	  return;
	}
      mapInfo()->owner = getpid();
    }
}  
  
// Revision Log:
//
// $Log$
// Revision 2.3  1997/06/25 12:55:04  houghton
// Cleanup dumpInfo() output.
//
// Revision 2.2  1997/06/23 12:56:00  houghton
// Bug-Fix: forgot to initialize errorNum in costructor.
//
// Revision 2.1  1997/06/05 11:29:12  houghton
// Initial Version.
//
//
