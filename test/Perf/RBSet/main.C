//
// File:        main.C
// Project:	Mdb
// Desc:        
//
//  Application entry point.
//
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     07/20/97 09:18
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
#include "PerfRBSet.hh"
#include <MapMemDynamicDynamic.hh>
#include <MapMemDynamicFixed.hh>
#include <MultiMemOffsetMapDynamic.hh>
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

  if( ! App->good() || App->help() || ! App->allArgs() )
    App->abort( 1, true, __FILE__, __LINE__ );

  ofstream  perfDest( App->perfInfoFn(), ios::out|ios::app, 0664 );
  
  MapMemDynamic	* mmd = 0;
  
  if( App->mapType() == "mdd" )
    mmd = new MapMemDynamicDynamic( App->tableFn(),
				    App->tableMode(),
				    App->create(),
				    Table_4::getNodeSize(),
				    App->allocSize(),
				    02 );

  if( App->mapType() == "mdf" )
    mmd = new MapMemDynamicFixed( App->tableFn(),
				  App->tableMode(),
				  App->create(),
				  Table_4::getNodeSize(),
				  App->allocSize(),
				  02 );

  if( ! mmd )
    {
      AppError << "bad map type: " << App->mapType() << endl;
      exit( 1 );
    }

  if( ! mmd->good() )
    {
      AppError << mmd->error() << endl;
      exit( 1 );
    }
  
  MultiMemOffsetMapDynamic  mmo( mmd, true );

  if( ! mmo.good() )
    {
      AppError << mmo.error() << endl;
      exit( 1 );
    }
  
  Table_4   table( &mmo, 0, App->create() );

  if( ! table.good() )
    {
      AppError << table.error() << endl;
      exit( 1 );
    }


  TimeIt    timer;
  
  if( App->insert() )
    {
      long first;
      long last;
      
      if( App->order() == "asc" )
	{
	  first = table.size();
	  last = first + App->qty();
	}
      else
	{
	  if( App->order() == "rev" )
	    {
	      last = table.size();
	      first = last + App->qty();
	    }
	  else
	    {
	      AppError << "unsupported order: " << App->order() << endl;
	      exit( 1 );
	    }
	}
      
      if( first > last )
	{
	  Rec_4 r;
	  timer.start();
	  for( long k = first; k < last ; ++ k )
	    {
	      r.k = k;
	      if( ! table.insert( r ).second )
		{
		  AppError << "insert '" << k << "' failed." << endl;
		  exit( 1 );
		}
	    }

	  timer.stop();

	  LogPerfData( perfDest,
		       App->mapType(),
		       table.getClassName(),
		       "insert",
		       App->order(),
		       App->allocSize(),
		       Table_4::getNodeSize(),
		       App->qty(),
		       timer );
	}
      else
	{
	  Rec_4 r;
	  timer.start();
	  for( long k = last - 1; k >= first ; -- k )
	    {
	      r.k = k;
	      if( ! table.insert( r ).second )
		{
		  AppError << "insert '" << k << "' failed." << endl;
		  exit( 1 );
		}
	    }

	  timer.stop();

	  LogPerfData( perfDest,
		       App->mapType(),
		       table.getClassName(),
		       "insert",
		       App->order(),
		       App->allocSize(),
		       Table_4::getNodeSize(),
		       App->qty(),
		       timer );
	}
    }

  if( App->find() )
    {
      long first = 0;
      long last = table.size();
      Rec_4 r;

      timer.start();
      
      for( long k = first; k < last ; ++ k )
	{
	  r.k = k;
	  if( table.find( r ) == table.end() )
	    {
	      AppError << "find '" << k << "' failed." << endl;
	      exit( 1 );
	    }
	}
      
      timer.stop();
      
      LogPerfData( perfDest,
		   App->mapType(),
		   table.getClassName(),
		   "find",
		   App->order(),
		   App->allocSize(),
		   Table_4::getNodeSize(),
		   table.size(),
		   timer );
    }

  AppDebug << table.dump() << endl;
  return( 0 );
}
//
// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/21 10:29:41  houghton
// Initial version.
//
//

