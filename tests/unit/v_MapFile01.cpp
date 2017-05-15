// 1997-07-16 (cc) Paul Houghton - (paul4hough@gmail.com)

#include <mdb/MapFile.hpp>
#include <clue/compare>
#include <valid/verify.hpp>

#include <cstdio>

#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;

valid::verify &
v_MapFile01( void )
{
  static VVDESC( "mdb::MapFile01" );

  static const char * TestFn = TEST_DATA_DIR "/tMapFile01.map";

  remove( TestFn );

  {
    // MapFile( const char *	fileName,
    //		MapAddr		baseAddr,
    //		std::ios::openmode  mode,
    //		bool		create,
    //		size_type	size,
    //		MapMask		permMask );
    //
    // good( void )
    // error( void )
    //
    // getFileName( void );
    // getAccess( void );
    // getMode( void );
    // getSize( void );
    // getBase( void );
    // getEnd( void );
    // getPageSize( void );
    //

    MapFile	t( TestFn,
		   0,
		   (std::ios::openmode)(std::ios::in | std::ios::out),
		   true,
		   0,
		   02 );

    TEST( t.good() );

    TEST( ::compare( t.getFileName(), TestFn ) == 0 );
    TEST( ::compare( t.getAccess(), "-rw-rw-r--" ) );
    TEST( t.getMode() == (std::ios::openmode)(std::ios::in | std::ios::out) );
    TEST( t.getSize() > 0 );
    TEST( t.getBase() != 0 );
    TEST( t.getEnd() == t.getBase() + t.getSize() );
    TEST( t.getPageSize() > 0 );

    memset( t.getBase(), 0x00, t.getSize() );

    {
      long cnt = 0;
      for( const char * c = t.getBase();
	   c < t.getEnd();
	   ++ c, ++ cnt )
	TEST( *c == 0x00 );

      TEST( cnt == t.getSize() );
    }

    memset( t.getBase(), 0xff, t.getSize() );

    {
      for( const unsigned char * c = (const unsigned char *)t.getBase();
	   c < (const unsigned char *)t.getEnd();
	   ++ c )
	TEST( *c == 0xff );
    }

  }

  return( VALID_VALIDATOR );
}
