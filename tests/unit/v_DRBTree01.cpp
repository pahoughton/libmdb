// 1997-07-16 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/DRBTree.hpp>
#include <mdb/MdbIdent.hpp>
#include <mdb/MapMemDynamicDynamic.hpp>
#include <mdb/MultiMemOffsetMapDynamic.hpp>
#include <valid/verify.hpp>

#include <vector>
#include <functional>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;


typedef DRBTree< long, long, MdbIdent< long, long >, std::less< long > >   Tree;

valid::verify &
v_DRBTree01( void )
{
  static VVDESC( "mdb::DRBTree01" );
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDRBTree01.drbt",
				  std::ios::in|std::ios::out,
				  true,
				  1,
				  1,
				  02 );

    TEST( mmdd.good() );

    MultiMemOffsetMapDynamic	mmo( &mmdd, false );

    TEST( mmo.good() );

    Tree    t( &mmo, 0, true );

    TEST( t.good() );

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
  return( VALID_VALIDATOR );
}
