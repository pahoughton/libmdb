//
// File:        MapMem.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for MapMem
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     11/21/94 10:59 
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "MapMem.hh"
#include "Str.hh"

#include <unistd.h>

#ifdef Linux
#include <fcntl.h>
#endif

#if defined( MDB_DEBUG )
#include "MapMem.ii"
#endif

MDB_VERSION(
  MapMem,
  "$Id$");


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
  const char *	    fileName,
  MapType	    type,
  MapVersion	    mapVersion,
  ios::open_mode    mode,
  bool		    create,
  MapAddr	    baseAddr,
  size_type	    size,
  MapMask	    permMask
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
  const char *	    fileName,
  MapType   	    type,
  MapVersion	    mapVersion,
  ios::open_mode    mode,
  bool		    overrideOwner
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

unsigned long
MapMem::getMapVersion( void ) const
{
  return( (mapInfo()) ? mapInfo()->version : 0 );
}
	
MapMem::MapType
MapMem::getType( void  ) const
{
  return( (mapInfo()) ? mapInfo()->type : MM_UNDEFINED );
}

long
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
  static Str errStr;
  errStr.reset();
  
  errStr << MapMem::getClassName();
  
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

const char *
MapMem::getClassName( void ) const
{
  return( "MapMem" );
}

const char *
MapMem::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}
  
ostream &
MapMem::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << MapMem::getClassName() << ":\n"
	 << MapMem::getVersion() << '\n';
      
  if( ! MapMem::good() )
    dest << prefix << "Error: " << MapMem::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << MapFile::getClassName() << "::";

  MapFile::dumpInfo( dest, pre, false );

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
      mapInfo()->base	    = (unsigned long)baseAddr;      
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
  const char *	    fileName,
  MapType   	    type,
  MapVersion	    mapVersion,
  ios::open_mode    mode,
  bool		    overrideOwner
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
  errorNum = E_OK;

}

// Revision Log:
//
// $Log$
// Revision 2.10  1997/09/17 16:56:02  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.9  1997/07/19 10:26:35  houghton
// Port(Sun5): renamed local variables to eliminate compiler warnings.
//
// Revision 2.8  1997/07/13 11:17:48  houghton
// Cleanup
// Added owner.
//
// Revision 2.7  1997/06/27 12:15:06  houghton
// Cleanup dumpInfo output.
//
// Revision 2.6  1997/06/09 11:57:50  houghton
// Bug-Fix: have to grab the base before I unmap.
//
// Revision 2.5  1997/06/05 11:23:36  houghton
// Cleanup.
// Change to be part of libMdb (vs StlUtils1).
// Added constructor that can create or open existing.
// Changed to use new MapFile types.
// Added createMapMem and openMapMemMehtods.
//
// Revision 2.4  1997/04/04 20:49:11  houghton
// Cleanup.
//
// Revision 2.3  1997/03/07 11:49:12  houghton
// Add dumpInfo.
//
// Revision 2.2  1997/03/03 14:32:31  houghton
// Added virtual destructor.
//
// Revision 2.1  1995/11/10 12:42:29  houghton
// Change to Version 2
//
// Revision 1.4  1995/11/05  16:32:33  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:49  houghton
// New Style Avl an memory management. Many New Classes
//

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
