// 1997-06-29 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MapMemDynamicFixed.hpp>
#include <clue/LibLog.hpp>
#include <valid/verify.hpp>

#include <algorithm>
#include <vector>
#include <iomanip>
#include <cstring>

//
// Note: #define MANUAL_REVIEW 1
//
//  to see detailed output after each operation.
//

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;

typedef std::vector< MapMemDynamicFixed::Loc >   AllocList;

valid::verify &
v_MapMemDynamicFixed01( void )
{
  static VVDESC( "mdb::MapMemDynamicFixed01" );

#if defined( MANUAL_REVIEW )
  Log	    l( tester.getDump(), LogLevel::Test );

  _LibLog = &l;

  tester.getDump() << endl;
#endif
  {
    MapMemDynamicFixed::size_type   recSize = 664;

    MapMemDynamicFixed	t( TEST_DATA_DIR "/tMMDF01.mdf",
			   recSize,
			   1,
			   02 );

    TEST( t.good() );

#if defined( MANUAL_REVIEW )
    t.dumpInfo( tester.getDump() ) << endl;
    t.dumpNodes( tester.getDump() ) << endl;
#endif
    AllocList	    allocList;

    MapMemDynamicFixed::Loc n;
    {

      for( int a = 0; a < 6; ++ a )
	{
	  TEST( n = t.allocate() );
	  memset( t.address( n ), 0xff, recSize );
	  allocList.push_back( n );

#if defined( MANUAL_REVIEW )
	  tester.getDump() << "Alloc: " << n << endl;
	  t.dumpNodes( tester.getDump() ) << endl;
#endif
	}
    }
#if defined( MANUAL_REVIEW )
    t.dumpInfo( tester.getDump() ) << endl;
#endif

    TEST( n = t.allocate() );
    memset( t.address( n ), 0xff, recSize );
    allocList.push_back( n );

#if defined( MANUAL_REVIEW )
    tester.getDump() << "Alloc: " << n << endl;
    t.dumpNodes( tester.getDump() ) << endl;

    t.dumpInfo( tester.getDump() ) << endl;
#endif

    {
      for( int a = 0; a < 17; ++ a )
	{
	  TEST( n = t.allocate() );
	  memset( t.address( n ), 0xff, recSize );
	  allocList.push_back( n );

#if defined( MANUAL_REVIEW )
	  tester.getDump() << "Alloc: " << n << endl;
	  t.dumpNodes( tester.getDump() ) << endl;
#endif
	}
    }

#if defined( MANUAL_REVIEW )
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    {
      int n = 0;
      for( AllocList::iterator them = allocList.begin();
	   them != allocList.end();
	   ++ them )
	{
	  tester.getDump() << "Allocated(" << n << "):  "
			   << setw(6) << (*them) << endl;
	  ++ n;
	}
    }

    int rNode;

    rNode = 12;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    rNode = 10;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    rNode = 14;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    rNode = 11;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    rNode = 13;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    rNode = 23;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    for( rNode = 15; rNode < 22; ++ rNode )
      {
	t.release( allocList[ rNode ] );
	tester.getDump() << "Release(" << rNode << "): "
			 << allocList[ rNode ] << endl;
      }

    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    rNode = 22;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

  }

  _LibLog = 0;
#endif
  }

  return( VALID_VALIDATOR );
}
