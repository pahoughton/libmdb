//
// File:        main.C
// Project:	Mdb
// Desc:        
//
//  Application entry point.
//
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     05/05/99 06:11
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include "MdbConfig.hh"
#include "AppParam.hh"

#include <MapMemDynamicDynamic.hh>
#include <MapMemDynamicFixed.hh>

#if defined( HAVE_MMF )
#include <MapMemFixedDynamic.hh>
#include <MapMemFixedFixed.hh>
#endif

#include <iostream>

AppParam * App = 0;

MDB_FUNCT_VERSION(
  main,
  "$Id$");

int
main( int argc, char * argv[] )
{

  if( (App = new AppParam( argc, argv, VERID_main ) ) == 0 )
    {
      cerr << "Can't new AppParam." << endl;
      exit( 1 );
    }

  if( ! App->good() || App->help() )
    App->abort( 1, true, __FILE__, __LINE__ );

  bool status( true );
  
  for( Param::Args::const_iterator	args = (*App).begin();
       args != (*App).end();
       ++ args )
    {
      const MapFile	mapFile( *args );

      if( ! mapFile.good() )
	{
	  AppError << mapFile.error() << endl;
	  status = false;
	  continue;
	}

      const MapFile::MapAddr	base( mapFile.getBase() );

      if( ! base )
	{
	  AppError << "base address is 0!" << endl;
	  status = false;
	  continue;
	}

      const MapMem::MapInfo *	mapInfo( (const MapMem::MapInfo *)base );

      switch( (*mapInfo).type )
	{
	case MapMem::MM_FIXED:
	  {
#if defined( HAVE_MMF )
	    if( (*mapInfo).base != 0 )
	      {
		const MapMemFixedFixed mff( *args );
		AppInfo << "The map is a MapMemFixedFixed:\n";
		mff.dumpInfo( (*App).log() );
	      }
	    else
	      {
		const MapMemFixedDynamic mfd( *args );
		AppInfo << "The map is a MapMemFixedDynamic:\n";
		mfd.dumpInfo( (*App).log() );
	      }
#endif
	    break;
	  }

	case MapMem::MM_DYNAMIC:
	  {
	    if( (*mapInfo).base != 0 )
	      {
		const MapMemDynamicFixed mdf( *args );
		AppInfo << "The map is a MapMemDynamicFixed:\n";
		mdf.dumpInfo( (*App).log() );
	      }
	    else
	      {
		const MapMemDynamicDynamic mdd( *args );
		AppInfo << "The map is a MapMemDynamicDynamic:\n";
		mdd.dumpInfo( (*App).log() );
	      }
	    break;
	  }
	default:
	  AppError << "unknown map type '" << (*mapInfo).type << "'."
		   << endl;
	  status = false;
	  break;
	}
    }

  delete App;
  
  return( status ? 0 : 1 );
}
//
// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:42  houghton
// Change Major Version to 4
//
// Revision 1.1  1999/06/10 11:47:09  houghton
// Initial Version.
//
//

