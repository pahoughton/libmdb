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
 size_t		initAllocNumRecs,
 long		quantity,
 ostream &	perfLog
 );

bool
pRBSet(
 const char *	fileName,
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

  
  pAvlTreeOffset( "../data/avl.avl",          1,   1024, cout );
  pAvlTreeOffset( "../data/avl.avl",          1,  10000, cout );
  pAvlTreeOffset( "../data/avl.avl",          1, 100000, cout );
  pAvlTreeOffset( "../data/avl.avl",          1, 500000, cout );
  pAvlTreeOffset( "../data/avl.avl",   1024 / 2,   1024, cout );
  pAvlTreeOffset( "../data/avl.avl",  10000 / 2,  10000, cout );
  pAvlTreeOffset( "../data/avl.avl", 100000 / 2, 100000, cout );
  pAvlTreeOffset( "../data/avl.avl", 500000 / 2, 500000, cout );
  // pAvlTreeOffset( "../data/avl.avl", 1, 10000, cout );
  remove( "../data/avl.avl" );

  pRBSet( "../data/rbset.rbt",          1,   1024, cout );
  pRBSet( "../data/rbset.rbt",          1,  10000, cout );
  pRBSet( "../data/rbset.rbt",          1, 100000, cout );
  pRBSet( "../data/rbset.rbt",          1, 500000, cout );
  pRBSet( "../data/rbset.rbt",   1024 / 2,   1024, cout );
  pRBSet( "../data/rbset.rbt",  10000 / 2,  10000, cout );
  pRBSet( "../data/rbset.rbt", 100000 / 2, 100000, cout );
  pRBSet( "../data/rbset.rbt", 500000 / 2, 500000, cout );
  return( 0 );
}
//
// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/13 11:36:40  houghton
// Initial Version.
//
//

