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
  0
};

const char * MapMem::TypeStrings[] =
{
  "MapFixed",
  "MapDynamic",
  0
};


//
// Constructor for creating a new map or opening an existing one.
//
MapMem::MapMem(
  const char *	    fileName,
  MapType	    type,
  MapVersion	    version,
  ios::open_mode    mode,
  bool		    create,
  MapAddr	    baseAddr,
  size_type	    size,
  MapMask	    permMask
  )
  : MapFile( fileName, baseAddr, mode, create, size, permMask ),
    mapError( E_OK ),
    osErrno( 0 )
{
  if( create )
    createMapMem( type, version, baseAddr );
  else
    openMapMem( fileName, type, version, mode );
}

  
//
// Constructor for creating a new map
//
MapMem::MapMem(
  const char * 	    fileName,
  MapAddr	    baseAddr,
  MapType   	    type,
  MapVersion	    version,
  size_type	    size,
  MapMask	    permMask
  )
  : MapFile( fileName, size, baseAddr, permMask )
{
  createMapMem( type, version, baseAddr );
}


// constructor for opening an existing map
MapMem::MapMem(
  const char *	    fileName,
  MapType   	    type,
  MapVersion	    version,
  ios::open_mode    mode
  )
  : MapFile( fileName, 0, mode )
{
  openMapMem( fileName, type, version, mode );
}

MapMem::~MapMem( void )
{
}

unsigned long
MapMem::getMapVersion( void ) const
{
  return( (getMapInfo()) ? getMapInfo()->version : 0 );
}
	
MapMem::MapType
MapMem::getType( void  ) const
{
  return( (getMapInfo()) ? getMapInfo()->type : MM_UNDEFINED );
}

const char *
MapMem::getTypeName( void ) const
{
  return( TypeStrings[ getType() ]  );
}

bool
MapMem::good( void ) const
{
  return( mapError == E_OK && MapFile::good() );
}


const char *
MapMem::error( void ) const
{
  static Str errStr;
  errStr.reset();
  
  errStr << MapMem::getClassName();
  
  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {
      int errCnt = 0;
      
      if( mapError > E_MAPFILE && mapError < E_UNDEFINED )
	{
	  errCnt++;
	  errStr << ErrorStrings[ mapError ];
	}
	  
      if( ! MapFile::good() )
	{
	  errCnt++;
	  errStr << ": " << MapFile::error();
	}

      if( ! errCnt )
	{
	  errStr << ": unknown error";
	}
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

  if( getMapInfo() != 0 )
    {
      dest << prefix << "map Ver:  " << getVersion() << '\n'
	   << prefix << "type:     " << getTypeName() << '\n'
	   << prefix << "map to:   " << getMapToAddr() << '\n'
	   << prefix << "map Size: " << getMapSize() << '\n'
	;
    }
  else
    {
      dest << prefix << "No Map info available!" << '\n';
    }

  return( dest );
}

void
MapMem::createMapMem( MapType type, MapVersion version, MapAddr baseAddr )
{
  osErrno = 0;
  
  if( getMapInfo() != 0 )
    {
      getMapInfo()->type	= type;
      getMapInfo()->version	= version;
      getMapInfo()->base	= (unsigned long)baseAddr;      
      getMapInfo()->size	= getSize();
      mapError = E_OK;
    }
  else
    {
      mapError = E_MAPFILE;
    }
}

void
MapMem::openMapMem(
  const char *	    fileName,
  MapType   	    type,
  MapVersion	    version,
  ios::open_mode    mode
  )
{
  if( ! MapFile::good() )
    return;

  if( getSize() < sizeof( MapInfo ) )
    {
      mapError = E_FILESIZE;
      return;
    }

  const MapInfo * info = getMapInfo();

  if( ! info )
    return;

  if( info->type != type )
    {
      mapError = E_BADTYPE;
      return;
    }

  if( info->version != version )
    {
      mapError = E_VERSION;
      return;
    }

  MapAddr   mapToAddr = (MapAddr)info->base;
  
  unmap();

  map( fileName, mapToAddr, mode );
      
  mapError = E_OK;

}

// Revision Log:
//
// $Log$
// Revision 2.6  1997/06/09 11:57:50  houghton
// Bug-Fix: have to grab the base before I unmap.
//
// Revision 2.5  1997/06/05 11:23:36  houghton
// Cleanup.
// Change to be part of libMdb (vs Clue1).
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
