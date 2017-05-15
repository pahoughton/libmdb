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
v_MapFile02( void )
{
  static VVDESC( "mdb::MapFile02" );

  static const char *	TestFn = TEST_DATA_DIR "/tMapFile02.map";
  MapFile::MapAddr	baseAddr = (MapFile::MapAddr)0x41000000;

  remove( TestFn );

  {
    TEST( MapFile::getPageSize() > 0 );

    // prove that constructor arg mode is ignored when create is true.
    // use a base addr.
    MapFile	t( TestFn,
		   baseAddr,
		   std::ios::in,
		   true,
		   MapFile::getPageSize() * 2,
		   022 );

    TEST( t.good() );


    TEST( ::compare( t.getFileName(), TestFn ) == 0 );
    TEST( ::compare( t.getAccess(), "-rw-r--r--" ) );
    TEST( t.getMode() == (std::ios::openmode)(std::ios::in | std::ios::out) );
    TEST( t.getSize() == MapFile::getPageSize() * 2 );
    TEST( t.getBase() == baseAddr );
    TEST( t.getEnd() == t.getBase() + t.getSize() );

    {
      long * lptr = (long *)t.getBase();
      long   v = 0;

      for( ; lptr < (long *)t.getEnd(); ++ lptr, ++ v )
	*lptr = v;

      TEST( v == t.getSize() / sizeof( long ) );
    }

    {
      long v = 0;
      for( const long * lptr = (const long *)t.getBase();
	   lptr < (const long *)t.getEnd();
	   ++ lptr, ++ v )
	{
	  TEST( *lptr == v );
	}
    }
  }

  {

    const MapFile	t( TestFn,
			   baseAddr,
			   std::ios::in );

    TEST( t.good() );


    TEST( ::compare( t.getFileName(), TestFn ) == 0 );
    TEST( ::compare( t.getAccess(), "-rw-r--r--" ) );
    TEST( t.getMode() == std::ios::in );
    TEST( t.getSize() == MapFile::getPageSize() * 2 );
    TEST( t.getBase() == baseAddr );
    TEST( t.getEnd() == t.getBase() + t.getSize() );

    {
      long v = 0;
      for( const long * lptr = (const long *)t.getBase();
	   lptr < (const long *)t.getEnd();
	   ++ lptr, ++ v )
	{
	  TEST( *lptr == v );
	}
      TEST( v == t.getSize() / sizeof( long ) );
    }

  }

  {
    std::ifstream  in( TestFn );

    TEST( in.good() );

    long    t;
    long    v = 0;
    for( ; in.read( (char *)&t, sizeof( t ) ).good(); ++ v )
      {
	TEST( t == v );
      }

  }

  return( VALID_VALIDATOR );
}
