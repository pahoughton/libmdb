// 1997-07-16 (cc) Paul Houghton - (paul4hough@gmail.com)

#include <mdb/MapFile.hpp>
#include <clue/compare>
#include <valid/verify.hpp>

#include <fstream>
#include <cstdio>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;

valid::verify &
v_MapFile03( void )
{
  static VVDESC( "mdb::MapFile03" );

  static const char *	TestFn = TEST_DATA_DIR "/tMapFile03.map";

  remove( TestFn );

  {

    MapFile	t( TestFn,
		   MapFile::getPageSize() * 2,
		   0,
		   02 );

    TEST( t.good() );


    TEST( ::compare( t.getFileName(), TestFn ) == 0 );
    TEST( ::compare( t.getAccess(), "-rw-rw-r--" ) );
    TEST( t.getMode() == (std::ios::openmode)(std::ios::in | std::ios::out) );
    TEST( t.getSize() == MapFile::getPageSize() * 2 );
    TEST( t.getBase() != 0 );
    TEST( t.getEnd() == t.getBase() + t.getSize() );

    memset( t.getBase(), 'a', t.getSize() );

    MapFile::size_type	curSize = t.getSize();
    MapFile::size_type	growAmount = t.getPageSize();

    TEST( t.grow( growAmount, 0 ) == curSize + growAmount );
    TEST( t.getSize() == curSize + growAmount );

    memset( t.getBase() + curSize, 'b', growAmount );

  }

  {
    MapFile	t( TestFn, 0, std::ios::in );

    TEST( t.good() );
    TEST( t.getSize() == t.getPageSize() * 3 );

    {

      for( const char * c = t.getBase();
	   c < t.getEnd();
	   ++ c )
	{
	  if( c - t.getBase() < t.getPageSize() * 2 )
	    {
	      TEST( *c == 'a' );
	    }
	  else
	    {
	      TEST( *c == 'b' );
	    }
	}
    }
  }

  {
    MapFile	t( TestFn, 0, (std::ios::openmode)(std::ios::in|std::ios::out) );

    TEST( t.good() );

    TEST( t.getSize() == t.getPageSize() * 3 );

    TEST( t.shrink( t.getPageSize(), 0 ) == t.getPageSize() * 2 );
    TEST( t.getSize() == t.getPageSize() * 2 );
  }

  {
    FileStat	t( TestFn );

    TEST( t.getSize() == MapFile::getPageSize() * 2 );
  }

  return( VALID_VALIDATOR );
}
