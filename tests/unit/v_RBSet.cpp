/* 1997-07-23 (cc) Paul Houghton <paul4hough@gmail.com>

   This is a small simple test of the RBSet<> tempate class.
   Since RBSet gets all of it's functionallity from RBTree,
   most of the testing ins done on the RBTree class.
*/

#include <mdb/RBSet.hpp>
#include <mdb/MapMemDynamicFixed.hpp>
#include <mdb/MultiMemOffsetMapDynamic.hpp>
#include <valid/verify.hpp>
#include <clue/compare>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;

#define TEST_TABLE_FILENAME	TEST_DATA_DIR "/tRBSet.rbs"

struct Rec
{
  long	k;
  char	data[17];
};

inline
bool
operator < ( const Rec & one, const Rec & two )
{
  return( one.k < two.k );
}

typedef RBSet< Rec, std::less< Rec > >   Table;

valid::verify &
v_RBSet( void )
{
  static VVDESC( "mdb::RBSet" );

  {
    MapMemDynamicFixed	mmdf( TEST_TABLE_FILENAME,
			      std::ios::in|std::ios::out,
			      true,
			      Table::getNodeSize(),
			      1,
			      02 );

    MultiMemOffsetMapDynamic mmo( &mmdf, false );

    Table   t( &mmo, 0, true );

    TEST( t.good() );

    TEST( t.size() == 0 );
    TEST( t.empty() );

    Rec r;

    {
      // insert
      Table::pair_iterator_bool ins;

      for( long k = 5; k < 100; ++ k )
	{
	  r.k = k;
	  ins = t.insert( r );
	  TEST( ins.second );
	  TEST( (*ins.first).k == k );
	}
    }

    TEST( t.size() == (100 - 5) );
    TEST( ! t.empty() );

    {
      // find
      Table::iterator it;

      for( long k = 0; k < 150; ++ k )
	{
	  r.k = k;

	  it = t.find( r );
	  if( k >= 5 && k < 100 )
	    {
	      TEST( it != t.end() );
	      TEST( (*it).k == k );
	    }
	  else
	    {
	      TEST( it == t.end() );
	    }
	}
    }
    // destroy t, mmo & mmdf
  }

  {
    // construct ios::in|ios::out

    MapMemDynamicFixed	mmdf( TEST_TABLE_FILENAME,
			      std::ios::in|std::ios::out );

    MultiMemOffsetMapDynamic mmo( &mmdf, false );

    Table   t( &mmo, 0 );

    TEST( t.good() );

    Rec r;

    {
      r.k = 20;
      TEST( t.erase( r ) );

      r.k = 25;
      TEST( t.erase( r ) );
    }

    {
      Table::iterator it;

      r.k = 30;
      it = t.find( r );
      TEST( t.erase( it ) );

      r.k = 35;
      it = t.find( r );
      TEST( t.erase( it ) );
    }

    {
      Table::iterator first;
      Table::iterator last;

      r.k = 10;
      first = t.find( r );
      r.k = 16;
      last = t.find( r );
      TEST( t.erase( first, last ) );
    }

    {
      long k = 5;
      for( Table::iterator them = t.begin();
	   them != t.end();
	   ++ them )
	{
	  TEST( (*them).k == k );
	  ++ k;

	  if( k == 10 )
	    k = 16;

	  if( k == 20 || k == 25 || k == 30 || k == 35 )
	    ++ k;
	}
      TEST( k == 100 );
    }

    {
      long k = 99;
      for( Table::reverse_iterator them = t.rbegin();
	   them != t.rend();
	   ++ them )
	{
	  TEST( (*them).k == k );
	  -- k;
	  if( k == 15 )
	    k = 9;

	  if( k == 20 || k == 25 || k == 30 || k == 35)
	    --k;
	}

      TEST( k == 4 );
    }

  }

  {
    // const Table

    MapMemDynamicFixed	mmdf( TEST_TABLE_FILENAME,
			      std::ios::in );

    MultiMemOffsetMapDynamic mmo( &mmdf, false );

    const Table   t( &mmo );

    TEST( t.good() );

    Rec r;

    {
      // find const
      Table::const_iterator it;

      for( long k = 0; k < 150; ++ k )
	{
	  r.k = k;

	  it = t.find( r );
	  if( k >= 5 && k < 100 &&
	      ( k < 10 || k > 15 ) &&
	      k != 20 && k != 25 && k != 30 && k != 35 )
	    {
	      TEST( it != t.end() );
	      TEST( (*it).k == k );
	    }
	  else
	    {
	      TEST( it == t.end() );
	    }
	}
    }

    {
      long k = 5;
      for( Table::const_iterator them = t.begin();
	   them != t.end();
	   ++ them )
	{
	  TEST( (*them).k == k );
	  ++ k;

	  if( k == 10 )
	    k = 16;

	  if( k == 20 || k == 25 || k == 30 || k == 35 )
	    ++ k;
	}
      TEST( k == 100 );
    }

    {
      long k = 99;
      for( Table::const_reverse_iterator them = t.rbegin();
	   them != t.rend();
	   ++ them )
	{
	  TEST( (*them).k == k );
	  -- k;
	  if( k == 15 )
	    k = 9;

	  if( k == 20 || k == 25 || k == 30 || k == 35)
	    --k;
	}

      TEST( k == 4 );
    }

    // tester.getDump() << endl;
    // t.dumpInfo( tester.getDump() ) << endl;;
    // tester.getDump() << t.dump() << endl;

  }

  {
    // now just empty the entire tree

    MapMemDynamicFixed	mmdf( TEST_TABLE_FILENAME,
			      std::ios::in|std::ios::out );

    MultiMemOffsetMapDynamic mmo( &mmdf, false );

    Table   t( &mmo, 0 );

    TEST( t.good() );

    TEST( t.erase( t.begin(), t.end() ) );
    TEST( t.size() == 0 );
    TEST( t.empty() );
  }
  return( VALID_VALIDATOR );
}
