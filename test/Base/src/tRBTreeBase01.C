//
// File:        tRBTreeBase01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tRBTreeBase01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/09/97 04:53
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <LibTest.hh>
#include <RBTree.hh>

typedef RBTree< long, long > Tree;

bool
tRBTreeBase01( LibTest & tester )
{

  {
    const char * TestFn = TEST_DATA_DIR "rbt01.rbt";
    
    MapMemDynamicDynamic    map( TestFn, 16 );

    TESTR( map.error(), map.good() );
    
    ChunkMgr		    chunkMgr( map );

    
				 
    Tree	    t( chunkMgr );
    Tree::Loc	    loc = 0;

    cout << endl;

    for( long i = 32; i > 0;-- i )
      {
	loc = t.insert( i, i );
	t.dumpTree( cout ) << endl;
      }    
  }
  
  return( true );
}


// Revision Log:
//
// $Log$
// Revision 1.1  1997/06/05 11:30:12  houghton
// Initial Version.
//
//
