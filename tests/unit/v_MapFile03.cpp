//
// File:        tMapFile03.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapFile03
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 09:32
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
#include <fstream>
#include <cstdio>

bool
tMapFile03( LibTest & tester )
{
  static const char *	TestFn = TEST_DATA_DIR "/tMapFile03.map";
  
  remove( TestFn );

  {
    
    MapFile	t( TestFn,
		   MapFile::getPageSize() * 2,
		   0,
		   02 );

    TESTR( t.error(), t.good() );

  
    TEST( ::compare( t.getFileName(), TestFn ) == 0 );
    TEST( ::compare( t.getAccess(), "-rw-rw-r--" ) );
    TEST( t.getMode() == (ios::open_mode)(ios::in | ios::out) );
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
    MapFile	t( TestFn, 0, ios::in );
    
    TESTR( t.error(), t.good() );
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
    MapFile	t( TestFn, 0, (ios::open_mode)(ios::in|ios::out) );

    TESTR( t.error(), t.good() );

    TEST( t.getSize() == t.getPageSize() * 3 );

    TEST( t.shrink( t.getPageSize(), 0 ) == t.getPageSize() * 2 );
    TEST( t.getSize() == t.getPageSize() * 2 );
  }

  {
    FileStat	t( TestFn );

    TEST( t.getSize() == MapFile::getPageSize() * 2 );
  }
      
  return( true );
}




// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/07/16 16:38:55  houghton
// Initial Version (work in progress).
//
//
