//
// File:        tMapFile01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapFile01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 07:45
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <MapFile.hh>
#include "TestConfig.hh"
#include <LibTest.hh>
#include <Compare.hh>
#include <cstdio>

bool
tMapFile01( LibTest & tester )
{
  static const char * TestFn = TEST_DATA_DIR "/tMapFile01.map";

  remove( TestFn );
  
  {
    // MapFile( const char *	fileName,
    //		MapAddr		baseAddr,
    //		ios::open_mode  mode,
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
		   (ios::open_mode)(ios::in | ios::out),
		   true,
		   0,
		   02 );

    TESTR( t.error(), t.good() );

    TEST( ::compare( t.getFileName(), TestFn ) == 0 );
    TEST( ::compare( t.getAccess(), "-rw-rw-r--" ) );
    TEST( t.getMode() == (ios::open_mode)(ios::in | ios::out) );
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

  return( true );
}
    

      
// Revision Log:
//
// $Log$
// Revision 2.1  1997/07/16 16:38:55  houghton
// Initial Version (work in progress).
//
//
