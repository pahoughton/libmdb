//
// File:        tRBTree03.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tRBTree03
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/11/97 07:41
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <RBTree.hh>
#include <MdbUtils.hh>
#include <MapMemDynamicFixed.hh>
#include <MultiMemOffsetMapDynamic.hh>

#include <LibTest.hh>
#include <vector>
#include <functional>
#include <set>

struct Rec
{
  long	    k;
  long	    v;
};

inline
bool
operator < ( const Rec & a, const Rec & b )
{
  return( a.k < b.k );
}

typedef RBTree< Rec, Rec, MdbIdent< Rec, Rec >, less< Rec > >   Tree;

bool
tRBTree03( LibTest & tester )
{
  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree03.rbt",
				  (ios::open_mode)(ios::in|ios::out),
				  true,
				  Tree::getNodeSize(),
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0, true );

    TESTR( t.error(), t.good() );

    Rec	  i;

    {
      for( long k = 0; k < 5000; ++ k )
	{
	  i.k = k;
	  TEST( t.insert( i ).second );
	}
    }

    TEST( t.erase( t.begin(), t.end() ) );

    TEST( t.empty() );
  }

  return( true );
}


    
    
// Revision Log:
//
// $Log$
// Revision 2.1  1997/07/11 17:39:42  houghton
// Initial Version.
//
//
