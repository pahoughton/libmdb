//
// File:        tMapBatch01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapBatch01
//  
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     06/29/00 10:01
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <MapBatch.hh>
#include "TestConfig.hh"
#include <LibTest.hh>
#include <Compare.hh>
#include <cstdio>

struct TestRec
{
  long	    one;
  short	    two;
  short	    three;
  char	    four[ 4 ];
  char	    padding[ 3 ];
  char	    eol[ 1 ];
};

bool
tMapBatch01( LibTest & tester )
{
  static const char * TestFn = TEST_DATA_DIR "/tMapBatch01.map";

  remove( TestFn );
  
  {
    typedef MapBatch< TestRec >	    Batch;

    Batch	t( TestFn );
    
  }

  return( true );
}
    

// Revision Log:
//
// $Log$
// Revision 1.1  2000/07/31 13:07:07  houghton
// Initial Version.
//
//
