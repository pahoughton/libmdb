//
// File:        tRBTree01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tRBTree01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/03/97 08:42
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
#include <algorithm>

typedef RBTree< long, long, MdbIdent< long, long >, less< long > >   Tree;
typedef vector< long >	List;

bool
tRBTree01( LibTest & tester )
{
  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree01.rbt",
				  (ios::open_mode)(ios::in|ios::out),
				  true,
				  Tree::getNodeSize(),
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0, true );

    TESTR( t.error(), t.good() );

    t.dumpInfo( tester.getDump() );
    t.dumpTree( tester.getDump() );
    t.testTree( tester.getDump() );
    
    TESTR( t.error(), t.insert( 15 ).second );

    t.dumpTree( tester.getDump() );
    t.dumpInfo( tester.getDump() );
    t.testTree( tester.getDump() );

    TESTR( t.error(), t.insert( 11 ).second );
    
    TEST( t.testTree( tester.getDump() ) );
    t.dumpTree( tester.getDump() );

    {
      for( long k = 10; k < 20; ++ k )
	{
	  if( k == 11 || k == 15 )
	    {
	      TESTR( "ins dup", t.insert( k ).second == false );
	    }
	  else
	    {
	      TESTR( t.error(), t.insert( k ).second );
	      TEST( t.testTree( tester.getDump() ) );
	      t.dumpTree( tester.getDump() ) << '\n';
	    }
	}
    }

    {
      for( long k = 9; k >= 0; -- k )
	{
	  TESTR( t.error(), t.insert( k ).second );
	  TEST( t.testTree( tester.getDump() ) );
	  t.dumpTree( tester.getDump() ) << '\n';
	}
    }

    {
      List randKeys;
      
      for( long k = 50; k < 100; ++ k )
	{
	  randKeys.push_back( k );
	}

      random_shuffle( randKeys.begin(), randKeys.end() );

      for( List::iterator them = randKeys.begin();
	   them != randKeys.end();
	   ++ them )
	{
	  TESTR( t.error(), t.insert( *them ).second );
	  TEST( t.testTree( tester.getDump() ) );
	  t.dumpTree( tester.getDump() ) << endl;
	}
    }

    t.dumpInfo( tester.getDump() );
	      
    {
      long k = 0;
      for( Tree::iterator them = t.begin();
	   them != t.end();
	   ++ them, ++ k )
	{
	  if( k == 20 )
	    k = 50;

	  TESTR( "bad value", k == *them );
	}

      TESTP( true );
      TESTR( "bad value", k == 100 );
    }

    {
      Tree::iterator it;
      long k;
      
      for( k = -1; k < 101; ++ k )
	{
	  if( k < 0 ||
	      ( k >= 20 && k < 50 ) ||
	      k > 99 )
	    {
	      it = t.find( k );
	      TEST( it == t.end() );
	    }
	  else
	    {
	      it = t.find( k );
	      TEST( it != t.end() );
	      TEST( *it == k );
	    }
	}
      
      TESTP( k == 101 );
      
    }
    
  }

  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree01.rbt",
				  (ios::open_mode)(ios::in|ios::out),
				  false );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0 );

    TESTR( t.error(), t.good() );

    tester.getDump() << "Erase Tests: " << endl;

    {
      long e;

      e = 99;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 0;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 13;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 79;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 91;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 58;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 1;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 98;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 80;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 88;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 86;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 87;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 85;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 84;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;
      
      e = 83;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      e = 5;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;
      
      e = 6;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;
      
      e = 7;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;
      
      e = 8;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;
      
      e = 2;
      TESTR( t.error(), t.erase( e ) );
      TEST( t.testTree( tester.getDump() ) );
      t.dumpTree( tester.getDump() ) << endl;

      {
	List erList;

	for( Tree::iterator them = t.begin();
	     them != t.end();
	     ++ them )
	  erList.push_back( *them );

	
	for( List::reverse_iterator erThem = erList.rbegin();
	     erThem != erList.rend();
	     ++ erThem )
	  {
	    TESTR( t.error(), t.erase( *erThem ) );
	    t.dumpTree( tester.getDump() ) << endl;
	    TEST( t.testTree( tester.getDump() ) );
	  }
      }
	    
    }

#if defined( FIXME )
    {
      List randKeys;
      
      for( long k = 50; k < 100; ++ k )
	{
	  randKeys.push_back( k );
	}

      random_shuffle( randKeys.begin(), randKeys.end() );

      Tree::iterator it;
      while( randKeys.size() )
	{
	  
	  TESTR( t.error(), t.erase( randKeys.back() ) );

	  t.dumpTree( tester.getDump() ) << endl;

	  it = t.find( randKeys.back() );
	  TEST( it == t.end() );
	  
	  randKeys.pop_back();
	  
	  for( List::iterator them = randKeys.begin();
	       them != randKeys.end();
	       ++ them )
	    {
	      it = t.find( *them );
	      TEST( it != t.end() );
	      TEST( *it == *them );
	    }

	  for( long f = 0; f < 20 ; ++ f )
	    {
	      it = t.find( f );
	      TEST( it != t.end() );
	      TEST( *it == f );
	    }
	}
    }

    {
      for( long k = 0; k < 10; ++ k )
	{
	  TESTR( t.error(), t.erase( k ) );
	  t.dumpTree( tester.getDump() ) << endl;
	}
    }
    
    {
      for( long k = 19; k > 9; -- k )
	{
	  TESTR( t.error(), t.erase( k ) );
	  t.dumpTree( tester.getDump() ) << endl;
	}
    }
	  
    if( false )
    {
      List recList;
      
      for( long k = 0; k < 256; ++ k )
	{
	  recList.push_back( k );
	  TESTR( t.error(), t.insert( k ).second );
	}

      for( List::reverse_iterator them = recList.rbegin();
	   them != recList.rend();
	   ++ them )
	{
	  TESTR( t.error(), t.erase( *them ) );
	}
      
      // random_shuffle( recList.begin(), recList.end() );
    }
#endif
    t.dumpInfo( tester.getDump() );
    // TEST( t.allTested() );
    
  }
  
  return( true );
}
  

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.2  1997/07/19 10:40:32  houghton
// Bug-Fix: added include <algorithm>
//
// Revision 2.1  1997/07/11 17:39:34  houghton
// Initial Version.
//
//
