// 1997-07-22 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/DashSet.hpp>
#include <mdb/MapMemDynamicDynamic.hpp>
#include <mdb/MultiMemOffsetMapDynamic.hpp>
#include <valid/verify.hpp>

#include <vector>
#include <functional>


#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;


class LongHash
{
public:
  HashTableBase::HashValue operator () ( const long k ) const {
    return( k );
  };
};

typedef DashSet< long, LongHash, std::less< long > >   Table;

valid::verify &
v_DashSet01( void )
{
  static VVDESC( "mdb::DashSet01" );
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDashSet01.data",
				  std::ios::in|std::ios::out,
				  true,
				  1,
				  1,
				  02 );

    TEST( mmdd.good() );

    MultiMemOffsetMapDynamic	mmo( &mmdd, false );

    TEST( mmo.good() );

    Table    t( &mmo, TEST_DATA_DIR "/tDash01.dash",
		std::ios::in|std::ios::out,
		true,
		02 );

    TEST( t.good() );

    {
      Table::pair_iterator_bool	ins;

      for( long k = 4; k < 20; k += 2 )
	{
	  ins = t.insert( k, 0 );
	  TEST( ins.second );
	  TEST( (*ins.first) == k );
	}
    }

    {
      Table::const_iterator  it;

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
      Table::iterator them = t.begin();
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
