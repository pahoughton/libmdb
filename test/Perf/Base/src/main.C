//
// File:        main.C
// Project:	Mdb
// Desc:        
//
//  Application entry point.
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/10/97 06:03
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
#include <cstdio>

AppParam * App = 0;

MDB_FUNCT_VERSION(
  main,
  "$Id$");

bool
pAvlTreeOffset(
 const char *	fileName,
 long		recSize,
 size_t		initAllocNumRecs,
 long		quantity,
 ostream &	perfLog
 );

bool
pRBSet(
 const char *	fileName,
 long		recSize,
 size_t		initAllocNumRecs,
 long		quantity,
 ostream &	perfLog
 );

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


  ofstream	perfLog( App->perfLogFn() );

  if( ! perfLog.good() )
    {
      AppError << "Perf log: '" << App->perfLogFn() << "' - "
	       << strerror( errno )
	       << endl;
      exit( 1 );
    }

  static long RecSize[] = { 4, 128, 512, 1024, -1 };
  static long Quantity[] = { 1024, 10000, 100000, 500000, -1 };
  
  for( long qty = 0; Quantity[qty] > 0; ++ qty )
    {
      for( long rs = 0; RecSize[rs] > 0; ++ rs )
	{
	  pAvlTreeOffset( "../data/perf.test",
			  RecSize[rs],
			  1,
			  Quantity[qty],
			  perfLog );
	  
	  pAvlTreeOffset( "../data/perf.test",
			  RecSize[rs],
			  Quantity[qty] / 2,
			  Quantity[qty],
			  perfLog );
  
	  pRBSet( "../data/perf.test",
		  RecSize[rs],
		  1,
		  Quantity[qty],
		  perfLog );

	  pRBSet( "../data/perf.test",
		  RecSize[rs],
		  Quantity[qty] / 2,
		  Quantity[qty],
		  perfLog );
	}
    }
  
  
  return( 0 );
}
//
// Revision Log:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:05  houghton
// Changed version to 2
//
// Revision 1.4  1997/07/19 19:55:14  houghton
// Reorederd loops.
//
// Revision 1.3  1997/07/16 19:20:51  houghton
// *** empty log message ***
//
// Revision 1.2  1997/07/14 10:52:29  houghton
// Reworked test order and put into a loop. (Still needs more work)
//
// Revision 1.1  1997/07/13 11:36:40  houghton
// Initial Version.
//
//

