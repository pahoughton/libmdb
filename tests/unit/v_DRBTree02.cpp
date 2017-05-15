// 1997-07-16 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/DRBTree.hpp>
#include <mdb/MdbIdent.hpp>
#include <mdb/MapMemDynamicDynamic.hpp>
#include <mdb/MultiMemOffsetMapDynamic.hpp>
#include <valid/verify.hpp>

#include <clue/Clue.hpp>

#include <vector>
#include <functional>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;

struct Rec
{
  long	k;
  long	v;
};

inline
bool
operator < ( const Rec & one, const Rec & two )
{
  return( one.k < two.k );
}

typedef DRBTree< Rec, Rec, MdbIdent< Rec, Rec >, std::less< Rec > >   Tree;

valid::verify &
v_DRBTree02( void )
{
  static VVDESC( "mdb::DRBTree02" );
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDRBTree02.drbt",
				  std::ios::in|std::ios::out,
				  true,
				  1,
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdd, false );

    TEST( mmo.good() );

    Tree    t( &mmo, 0, true );

    TEST( t.good() );

    {
      Tree::pair_iterator_bool	ins;
      Rec r;

      for( long k = 4; k < 20; k += 2 )
	{
	  for( long e = 5; e < 30; e += 5 )
	    {
	      r.k = k;
	      r.v = (k * 100) + e;

	      ins = t.insert( r, e );
	      TEST( ins.second );
	      TEST( (*ins.first).k == r.k );
	      TEST( (*ins.first).v == r.v );
	    }
	}
    }

    {
      Tree::const_iterator  it;
      Rec r;
      for( long k = 0; k < 25; ++ k )
	{
	  for( long e = 0; e < 50; ++ e )
	    {
	      r.k = k;
	      it = t.find( r, e );

	      if( k >= 4 && k < 20 && (k % 2) == 0  && e >= 5 )
		{
		  TEST( it != t.end() );
		  TEST( (*it).k == k );
		  TEST( (*it).v == (k * 100) + RoundDown( std::min( e, 25L ), 5 ) );

		}
	      else
		{
		  TEST( it == t.end() );
		}
	    }
	}
    }

    {
      Tree::const_iterator them = t.begin();
      long	     k = 4;
      long	     e = 25;
      for( ; them != t.end(); ++ them )
	{
	  TEST( (*them).k == k );
	  TEST( (*them).v == (k * 100) + e );

	  e -= 5;
	  if( e == 0 )
	    {
	      k += 2;
	      e = 25;
	    }
	}
      TEST( them == t.end() );
      TEST( k == 20 );
      TEST( e == 25 );
    }
  }
  return( VALID_VALIDATOR );
}
