//
// File:        main.C
// Project:	Mdb
// Desc:        
//
//  Application entry point.
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/08/97 12:27
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <MdbConfig.hh>
#include <TestConfig.hh>
#include <LibTest.hh>
#include "TestFunctProto.hh"

#include <iostream>

MDB_FUNCT_VERSION(
  main,
  "$Id$");


LibTest::TestItem   TestList[] =
{
  { "MapFile", tMapFile },
  { "MapMemDynamicFixed", tMapMemDynamicFixed },
  { "MapMemDynamicDynamic", tMapMemDynamicDynamic },
  { "RBTree", tRBTree },
  { "RBSet", tRBSet },
  { "DRBTree", tDRBTree },
  { "HashTable", tHashTable },
  { "HashSet", tHashSet },
  { "DashSet", tDashSet },
  { "MapMemDlist", tMapMemDlist },
  { "AvlTreeOffset", tAvlTreeOffset },
  // { "DavlTreeOffset", tDavlTreeOffset }, NOT READY!
  { 0, 0 }
};

int
main( int argc, char * argv[] )
{

#if defined( TEST_DUMP_DEST )
  ofstream  dumpDest( TEST_DUMP_DEST );
#else
  ostream & dumpDest( cout );
#endif
  
  LibTest test( TestList, false, dumpDest, cout, cerr );

  return( test.run( argc, argv ) );

  return( 0 );
}
//
// Revision Log:
//
// $Log$
// Revision 2.2  1998/10/23 13:22:09  houghton
// Added MapMemDList test.
//
// Revision 2.1  1997/09/21 19:21:59  houghton
// Changed version to 2
//
// Revision 1.8  1997/07/25 13:49:31  houghton
// Changed: Dash was renamed to DashSet.
// Changed: Hash was renamed to HashSet.
//
// Revision 1.7  1997/07/22 19:45:51  houghton
// Added Dash test.
//
// Revision 1.6  1997/07/19 10:35:41  houghton
// Added HashTable test.
//
// Revision 1.5  1997/07/16 16:38:28  houghton
// Added MapFile & DRBTree tests.
//
// Revision 1.4  1997/07/14 10:44:42  houghton
// Changed order of tests.
//
// Revision 1.3  1997/07/13 11:35:10  houghton
// Added many test.
//
// Revision 1.2  1997/06/27 12:16:06  houghton
// Added tMapMemDynamicDynamic.
//
// Revision 1.1  1997/06/05 11:30:11  houghton
// Initial Version.
//
//

