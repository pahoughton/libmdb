// 1997-07-21 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/DRBTree.hpp>
#include <mdb/MdbIdent.hpp>
#include <mdb/MapMemDynamicDynamic.hpp>
#include <mdb/MultiMemOffsetMapDynamic.hpp>
#include <clue/Clue.hpp>
#include <valid/verify.hpp>

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
v_DRBTree03( void )
{
  static VVDESC( "mdb::DRBTree03" );

  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDRBTree03.drbt",
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
      Rec r;
      r.k = 4;

      TEST( t.erase( r, 25 ) );

      Tree::iterator it = t.find( r, 25 );

      TEST( it == t.end() );

      it = t.find( r, 24 );

      TEST( it != t.end() );
      TEST( (*it).k == 4 );
      TEST( (*it).v == (r.k * 100) + 20 );

      it = t.begin();

      TEST( it != t.end() );
      TEST( (*it).k == 4 );
      TEST( (*it).v == (r.k * 100) + 20 );
    }

    {
      Rec r;
      r.k = 18;
      TEST( t.erase( r, 0 ) );

      Tree::iterator it = t.find( r, 0 );

      TEST( it == t.end() );

      it = t.find( r, 5 );

      TEST( (*it).k == 18 );
      TEST( (*it).v == ( 18 * 100 ) + 5 );

      it = t.end();
      -- it;

      TEST( (*it).k == 18 );
      TEST( (*it).v == ( 18 * 100 ) + 5 );
    }

    {
      Rec r;
      r.k = 10;
      TEST( t.erase( r, 15 ) );

      Tree::iterator it;

      for( Tree::EffDate e = 15; e < 20; ++ e )
	{
	  TEST( t.find( r, e ) == t.end() );
	}

      it = t.find( r, 14 );
      TEST( it != t.end() );
      TEST( (*it).k == r.k );
      TEST( (*it).v == (r.k * 100) + 10 );

      it = t.find( r, 20 );
      TEST( it != t.end() );
      TEST( (*it).k == r.k );
      TEST( (*it).v == (r.k * 100) + 20 );
    }

    {
      Rec r;
      r.k = 8;
      for( Tree::EffDate e = 0; e < 35; e += 5 )
	{
	  TEST( t.erase( r, e ) );
	}

    }

    {
      Rec r;
      r.k = 16;
      for( Tree::EffDate e = 25; e > -1 ; e -= 5 )
	{
	  TEST( t.erase( r, e ) );
	}
    }

    {
      Rec r;

      for( long k = 1; k < 25; k += 2 )
	{
	  r.k = k;
	  TEST( ! t.erase( r, 10 ) );
	}
    }

    {
      Tree::const_iterator  it;

      Rec r;
      Str errDesc;
      for( long k = 0; k < 25; ++ k )
	{
	  for( long e = 0; e < 50; ++ e )
	    {
	      r.k = k;
	      it = t.find( r, e );

	      if( ( k == 4 && ( e >= 5 && e < 25 ) ) ||
		  ( k == 10 && ( ( e >= 5 && e < 15 ) ||
				 ( e >= 20 ) ) ) ||
		  ( k > 4 && k < 20 && ( k % 2 ) == 0 &&
		    k != 8 && k != 10 && k != 16 && e >= 5 ) )
		{
		  errDesc = "not found: ";
		  errDesc << k << " " << e;

		  TEST( it != t.end() );
		  TEST( (*it).k == k );
		  TEST( (*it).v == (k * 100) + RoundDown( std::min( e, 25L ), 5 ) );
		}
	      else
		{
		  errDesc = "found: ";
		  errDesc << k << " " << e;

		  TEST( it == t.end() );
		}
	    }
	}
    }

    {
      Tree::const_iterator them = t.begin();
      long	     k = 4;
      long	     e = 20;
      Str errDesc;

      // std::cout << '\n';
      for( ; them != t.end(); ++ them )
	{
	  errDesc = "match: ";
	  errDesc << "k: " << k << " tk: " << (*them).k
		  << " e: " << e << " te: " << t.effective( them );

	  TEST( (*them).k == k );
	  TEST( t.effective( them ) == e );
	  TEST( (*them).v == (k * 100) + e );

	  e -= 5;

	  if( k == 10 && e == 15 ) {
	    e -= 5;
	  }

	  if( e == 0 ) {
	    e = 25;
	    k += 2;

	    if( k == 8 || k == 16 )
	      k += 2;
	  }
	}
      TEST( them == t.end() );
      TEST( k == 20 );
      TEST( e == 25 );
    }

  }
  return( VALID_VALIDATOR );
}
