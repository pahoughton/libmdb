// 1997-06-27 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MapMemDynamicDynamic.hpp>
#include <valid/verify.hpp>

#include <algorithm>
#include <vector>

#include <cstring>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

#define MAX_VALUE_SIZE	256

// interesting Sun5 problem, the static.allocater in vector<>
// remembers 'Data' across sources, so it gets the size wrong.
using namespace mdb;
using namespace clue;


struct MMDD02Data
{
  MMDD02Data( MapMemDynamicDynamic::Loc l = 0, long s = 0) : loc(l), size(s) {};
  MapMemDynamicDynamic::Loc	loc;
  long				size;
  unsigned char			value[ MAX_VALUE_SIZE + 1];
};

typedef std::vector<MMDD02Data> DataList;

static const long  TotalChunks = 10000;

typedef	    std::vector< int >   DataIndex;

valid::verify &
v_MapMemDynamicDynamic02( void )
{
  static VVDESC( "mdb::MapMemDynamicDynamic02" );

  DataList	data;
  DataIndex	dataIndex;

  {
    MMDD02Data d;
    d.loc = 0;

    for( int i = 0; i < TotalChunks; ++ i )
      {
	dataIndex.push_back( i );
	d.size = (rand() % MAX_VALUE_SIZE);
	for( int c = 0; c < d.size; ++ c )
	  d.value[ c ] = (rand() % UCHAR_MAX);
	data.push_back( d );
      }
  }

  {
    MapMemDynamicDynamic    t( TEST_DATA_DIR "/tMMDD02.mdd",
			       1,
			       4096 );

    // allocate all
    for( DataList::iterator them = data.begin();
	 them != data.end();
	 ++ them )
      {
	(*them).loc = t.allocate( (*them).size );
	TEST( (*them).loc );
	memcpy( t.address( (*them).loc ), (*them).value, (*them).size );
      }

    // random release all
    {
      DataIndex	    randRelIndex( dataIndex );

      random_shuffle( randRelIndex.begin(), randRelIndex.end() );

      for( DataIndex::iterator them = randRelIndex.begin();
	   them != randRelIndex.end();
	   ++ them )
	{
	  TEST( memcmp( t.address( data[ (*them) ].loc ),
			 data[ (*them) ].value,
			 data[ (*them) ].size ) == 0 );

	  t.release( data[ (*them) ].loc );
	  data[ (*them) ].loc = 0;
	}
    }

    TEST( t.getChunkSize() == 0 );

    // rand allocate & release

    {
      DataIndex	randDataIndex( dataIndex );

      random_shuffle( randDataIndex.begin(), randDataIndex.end() );

      DataIndex		    allocIndex;
      DataIndex::iterator   randThem = randDataIndex.begin();

      // tester.getDump() << "\n  Rand a/r test." << endl;

      for( long testCount = 0; testCount < 15000; ++ testCount )
	{
	  if( (rand() % 2) == 1 )
	    {
	      // alloc one.
	      if( randThem != randDataIndex.end() )
		{
		  allocIndex.push_back( *randThem );

		  TEST( data[ (*randThem) ].loc == 0 );
		  data[ (*randThem) ].loc =
		    t.allocate( data[ (*randThem) ].size );
		  TEST( data[ (*randThem) ].loc );
		  memcpy( t.address( data[ (*randThem) ].loc ),
			  data[ (*randThem) ].value,
			  data[ (*randThem) ].size );
		  ++ randThem;
		  // tester.getDump() << 'a';
		  // tester.getDump().flush();
		}
	      else
		{
		  TEST( false );
		}
	    }
	  else
	    {
	      if( allocIndex.size() )
		{
		  // release one
		  random_shuffle( allocIndex.begin(), allocIndex.end() );

		  TEST( data[ allocIndex.back() ].loc );

		  TEST( memcmp( t.address( data[ allocIndex.back() ].loc ),
				 data[ allocIndex.back() ].value,
				 data[ allocIndex.back() ].size ) == 0 );

		  t.release( data[ allocIndex.back() ].loc );
		  TEST( t.good() );
		  data[ allocIndex.back() ].loc = 0;
		  allocIndex.pop_back();
		  // tester.getDump() << 'R';
		  // tester.getDump().flush();
		}
	    }
	}
    }
  }
  return( VALID_VALIDATOR );
}
