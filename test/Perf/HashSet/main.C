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
#include <iostream>

AppParam * App = 0;

MDB_FUNCT_VERSION(
  main,
  "$Id$");

bool PerfTable_4( ostream & perfDest );
bool PerfTable_128( ostream & perfDest );
bool PerfTable_512( ostream & perfDest );
bool PerfTable_1024( ostream & perfDest );

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

  switch( App->recSize() )
    {
    case 4:
      return( PerfTable_4( perfDest ) ? 0 : 1 );

    case 128:
      return( PerfTable_128( perfDest ) ? 0 : 1 );
      
    case 512:
      return( PerfTable_512( perfDest ) ? 0 : 1 );
      
    case 1024:
      return( PerfTable_1024( perfDest ) ? 0 : 1 );

    default:
      AppError << "Unsupported record size: " << App->recSize() << endl;
      return( 0 );
    }
  
      
#if defined( OLD_WAY )  
  if( ! table.good() )
    {
      AppError << table.error() << endl;
      exit( 1 );
    }


  TimeIt    timer;
  
  if( App->insert() )
    {

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

  return( 0 );
#endif
  
}
//
// Revision Log:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:18  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/25 13:41:48  houghton
// Initial Version.
//
//

