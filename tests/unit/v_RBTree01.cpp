// 1997-07-03 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/RBTree.hpp>
#include <mdb/MdbIdent.hpp>
#include <mdb/MapMemDynamicFixed.hpp>
#include <mdb/MultiMemOffsetMapDynamic.hpp>
#include <valid/verify.hpp>


#include <vector>
#include <functional>
#include <algorithm>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;


typedef RBTree< long, long, MdbIdent< long, long >, std::less< long > >   Tree;
typedef std::vector< long >	List;

valid::verify &
v_RBTree01( void )
{
  static VVDESC( "mdb::RBTree01" );
  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree01.rbt",
				  std::ios::in|std::ios::out,
				  true,
				  Tree::getNodeSize(),
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TEST( mmo.good() );

    Tree    t( &mmo, 0, true );

    TEST( t.good() );

    //t.dumpInfo( tester.getDump() );
    //t.dumpTree( tester.getDump() );
    //t.testTree( tester.getDump() );

    TEST( t.insert( 15 ).second );

    //t.dumpTree( tester.getDump() );
    //t.dumpInfo( tester.getDump() );
    //t.testTree( tester.getDump() );

    TEST( t.insert( 11 ).second );

    TEST( t.testTree( std::cout ) );
    //t.dumpTree( tester.getDump() );

    {
      for( long k = 10; k < 20; ++ k )
	{
	  if( k == 11 || k == 15 )
	    {
	      TEST( t.insert( k ).second == false );
	    }
	  else
	    {
	      TEST( t.insert( k ).second );
	      TEST( t.testTree( std::cout ) );
	      // t.dumpTree( tester.getDump() ) << '\n';
	    }
	}
    }

    {
      for( long k = 9; k >= 0; -- k )
	{
	  TEST( t.insert( k ).second );
	  TEST( t.testTree( std::cout ) );
	  //t.dumpTree( tester.getDump() ) << '\n';
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
	  TEST( t.insert( *them ).second );
	  TEST( t.testTree( std::cout ) );
	  //t.dumpTree( tester.getDump() ) << endl;
	}
    }

    //t.dumpInfo( tester.getDump() );

    {
      long k = 0;
      for( Tree::iterator them = t.begin();
	   them != t.end();
	   ++ them, ++ k )
	{
	  if( k == 20 )
	    k = 50;

	  TEST( k == *them );
	}

      TEST( k == 100 );
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

      TEST( k == 101 );

    }

  }

  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree01.rbt",
				  std::ios::in|std::ios::out,
				  false );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TEST( mmo.good() );

    Tree    t( &mmo, 0 );

    TEST( t.good() );

    // tester.getDump() << "Erase Tests: " << endl;

    {
      long e;

      e = 99;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 0;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 13;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 79;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 91;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 58;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 1;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 98;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 80;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 88;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 86;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 87;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 85;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 84;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 83;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 5;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 6;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 7;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 8;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

      e = 2;
      TEST( t.erase( e ) );
      TEST( t.testTree( std::cout ) );
      //t.dumpTree( tester.getDump() ) << endl;

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
	    TEST( t.erase( *erThem ) );
	    //t.dumpTree( tester.getDump() ) << endl;
	    TEST( t.testTree( std::cout ) );
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

	  TEST( t.erase( randKeys.back() ) );

	  //t.dumpTree( tester.getDump() ) << endl;

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
	  TEST( t.erase( k ) );
	  //t.dumpTree( tester.getDump() ) << endl;
	}
    }

    {
      for( long k = 19; k > 9; -- k )
	{
	  TEST( t.erase( k ) );
	  //t.dumpTree( tester.getDump() ) << endl;
	}
    }

    if( false )
    {
      List recList;

      for( long k = 0; k < 256; ++ k )
	{
	  recList.push_back( k );
	  TEST( t.insert( k ).second );
	}

      for( List::reverse_iterator them = recList.rbegin();
	   them != recList.rend();
	   ++ them )
	{
	  TEST( t.erase( *them ) );
	}

      // random_shuffle( recList.begin(), recList.end() );
    }
#endif
    //t.dumpInfo( tester.getDump() );
    // TEST( t.allTested() );

  }
  return( VALID_VALIDATOR );

}
