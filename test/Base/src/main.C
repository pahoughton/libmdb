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
// #include <TestConfig.hh>
#include <LibTest.hh>
#include "TestFunctProto.hh"

#include <iostream>

MDB_FUNCT_VERSION(
  main,
  "$Id$");


LibTest::TestItem   TestList[] =
{
  { "HashTable", tHashTable },
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
// Revision 1.1  1997/06/05 11:30:11  houghton
// Initial Version.
//
//

