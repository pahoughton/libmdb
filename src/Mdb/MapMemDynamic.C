//
// File:        MapMemDynamic.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for MapMemDynamic
//  
// Author:      Paul A. Houghton - (paul.houghton@mci.com)
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
  MapVersion	    mapVersion,
  ios::open_mode    mode,
  bool		    create,
  size_type	    size,
  MapMask	    permMask
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
  const char *	    fileName,
  MapVersion	    mapVersion,
  ios::open_mode    mode,
  bool		    overrideOwner
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
  static Str errStr;

  errStr = MapMemDynamic::getClassName();

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
  pre << MapMem::getClassName() << "::" ;

  MapMem::dumpInfo( dest, pre, false );

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
  
// Revision Log:
//
// $Log$
// Revision 4.2  2003/08/09 12:43:24  houghton
// Changed ver strings.
//
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.6  1997/07/19 10:26:55  houghton
// Port(Sun5): renamed local variables to eliminate compiler warnings.
//
// Revision 2.5  1997/07/13 11:19:38  houghton
// Cleanup
// Removed owner (now in MapMem).
//
// Revision 2.4  1997/06/27 12:15:12  houghton
// Cleanup dumpInfo output.
//
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
