//
// File:        tDRBTree01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDRBTree01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 05:49
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <DRBTree.hh>
#include <MdbUtils.hh>
#include <MapMemDynamicDynamic.hh>
#include <MultiMemOffsetMapDynamic.hh>

#include <LibTest.hh>
#include <vector>
#include <functional>


typedef DRBTree< long, long, MdbIdent< long, long >, less< long > >   Tree;

bool
tDRBTree01( LibTest & tester )
{
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDRBTree01.drbt",
				  (ios::open_mode)(ios::in|ios::out),
				  true,
				  1,
				  1,
				  02 );

    TESTR( mmdd.error(), mmdd.good() );
    
    MultiMemOffsetMapDynamic	mmo( &mmdd, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0, true );

    TESTR( t.error(), t.good() );

    {
      Tree::pair_iterator_bool	ins;
      
      for( long k = 4; k < 20; k += 2 )
	{
	  ins = t.insert( k, 0 );
	  TEST( ins.second );
	  TEST( (*ins.first) == k );
	}
    }

    {
      Tree::const_iterator  it;
      
      for( long k = 0; k < 25; ++ k )
	{
	  it = t.find( k, 0 );
	  if( k >= 4 && k < 20 && (k % 2) == 0 )
	    {
	      TEST( it != t.end() );
	      TEST( (*it) == k );
	    }
	  else
	    {
	      TEST( it == t.end() );
	    }
	}
    }

    {
      Tree::iterator them = t.begin();
      long	     k = 4;
      for( ; them != t.end(); ++ them )
	{
	  TEST( *them == k );
	  k += 2;
	}
      TEST( them == t.end() );      
      TEST( k == 20 );
    }
       
  }
  return( true );
}

// Revision Log:
//
// $Log$
// Revision 2.2  1997/07/19 10:37:34  houghton
// Bug-Fix: forgot to declare return type.
//
// Revision 2.1  1997/07/16 16:38:54  houghton
// Initial Version (work in progress).
//
//
