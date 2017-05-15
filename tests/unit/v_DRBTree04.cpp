// 1997-07-22 (cc) Paul Houghton <paul4hough@gmail.com>

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
v_DRBTree04( void )
{
  static VVDESC( "mdb::DRBTree01" );
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDRBTree04.drbt",
				  std::ios::in|std::ios::out,
				  true,
				  1,
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdd, false );

    TEST( mmo.good() );

    Tree    t( &mmo, 0, true );

    TEST( t.good() );

    Rec r;
    Tree::size_type age_20_cnt = 0;

    {
      Tree::pair_iterator_bool	ins;

      Tree::size_type insCnt = 0;

      for( long k = 4; k < 50; ++ k )
	{
	  for( Tree::EffDate e = 5;
	       ( ( (k % 2) && e < 25) ||
		 ( ((k % 2) == 0) && e < 50 ) );
	       e += 5 )
	    {
	      r.k = k;
	      r.v = (k * 100) + e;

	      ++ insCnt;
	      if( e < 20 )
		++ age_20_cnt;

	      ins = t.insert( r, e );
	      TEST( ins.second );
	      TEST( (*ins.first).k == r.k );
	      TEST( (*ins.first).v == r.v );
	    }
	}
      TEST( t.size() == insCnt );
    }

    {
      Tree::size_type  origSize = t.size();
      TEST( t.trim( 20 ) );

      TEST( t.size() == (origSize - age_20_cnt) );

      Tree::const_iterator  it;

      for( long k = 0; k < 75; ++ k )
	{
	  for( long e = 0; e < 75; ++ e )
	    {
	      r.k = k;
	      it = t.find( r, e );

	      if( k >= 4 && k < 50 && e >= 20 )
		{
		  TEST( it != t.end() );
		}
	      else
		{
		  TEST( it == t.end() );
		}
	    }
	}
    }

    {
      Tree::size_type  erase_30_cnt = 0;
      {
	Tree::size_type  origSize = t.size();
	for( long k = 5; k < 50; k += 2 )
	  {
	    ++ erase_30_cnt;
	    r.k = k;
	    TEST( t.erase( r, 30 ) );
	  }

	TEST( t.size() == origSize + erase_30_cnt );
      }

      {
	Tree::const_iterator  it;

	for( long k = 0; k < 50; ++ k )
	  {
	    for( long e = 0; e < 75; ++ e )
	      {
		r.k = k;
		it = t.find( r, e );

		if( k >= 4 && k < 50 && e >= 20 &&
		    ( ( ( k % 2) && e < 30 ) ||
		      ( ( k % 2) == 0 ) ) )
		  {
		    TEST( it != t.end() );
		  }
		else
		  {
		    TEST( it == t.end() );
		  }
	      }
	  }
      }

      {
	TEST( t.size() == 184 );
	TEST( t.trim( 35 ) );
	TEST( t.size() == 46 );
      }
    }

    {
      Tree::const_iterator them = t.begin();
      long	     k = 4;
      long	     e = 45;
      Str errDesc;

      for( ; them != t.end(); ++ them )
	{
	  errDesc = "match: ";
	  errDesc << "k: " << k << " tk: " << (*them).k
		  << " e: " << e << " te: " << t.effective( them );


	  TEST( (*them).k == k );
	  TEST( t.effective( them ) == e );
	  TEST( (*them).v == (k * 100) + e );

	  e -= 5;

	  if( e == 30 )
	    {
	      e = 45;
	      k += 2;

	    }
	}
      TEST( them == t.end() );
      TEST( k == 50 );
      TEST( e == 45 );
    }
  }
  return( VALID_VALIDATOR );
}
