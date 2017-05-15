// 1997-07-11 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/RBTree.hpp>
#include <mdb/MdbIdent.hpp>
#include <mdb/MapMemDynamicFixed.hpp>
#include <mdb/MultiMemOffsetMapDynamic.hpp>
#include <valid/verify.hpp>

#include <vector>
#include <functional>
#include <set>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;

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

typedef RBTree< Rec, Rec, MdbIdent< Rec, Rec >, std::less< Rec > >   Tree;

valid::verify &
v_RBTree03( void )
{
  static VVDESC( "mdb::RBTree03" );
  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree03.rbt",
				  std::ios::in|std::ios::out,
				  true,
				  Tree::getNodeSize(),
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TEST( mmo.good() );

    Tree    t( &mmo, 0, true );

    TEST( t.good() );

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

  return( VALID_VALIDATOR );
}
