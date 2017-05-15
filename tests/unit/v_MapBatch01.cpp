// 2000-06-29 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MapBatch.hpp>
#include <valid/verify.hpp>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;

struct TestRec
{
  long	    one;
  short	    two;
  short	    three;
  char	    four[ 4 ];
  char	    padding[ 3 ];
  char	    eol[ 1 ];
};

typedef MapBatch< TestRec >	    Batch;

valid::verify &
v_MapBatch01( void )
{
  static VVDESC( "mdb::MapBatch01" );
  static const char * TestFn = TEST_DATA_DIR "/tMapBatch01.map";

  remove( TestFn );

  VVWHEN( Batch	t( TestFn,
		   std::ios::out|std::ios::in,
		   true,
		   02) );
  {
    VVTRUE( t.good() );
  }

  return( VALID_VALIDATOR );
}
