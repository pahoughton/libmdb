//
// File:        tMapFile02.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapFile02
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 08:10
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
tMapFile02( LibTest & tester )
{
  static const char *	TestFn = TEST_DATA_DIR "/tMapFile02.map";
  MapFile::MapAddr	baseAddr = (MapFile::MapAddr)0x41000000;

  remove( TestFn );

  {
    TEST( MapFile::getPageSize() > 0 );
    
    // prove that constructor arg mode is ignored when create is true.
    // use a base addr.
    MapFile	t( TestFn,
		   baseAddr,
		   ios::in,
		   true,
		   MapFile::getPageSize() * 2,
		   022 );

    TESTR( t.error(), t.good() );

  
    TEST( ::compare( t.getFileName(), TestFn ) == 0 );
    TEST( ::compare( t.getAccess(), "-rw-r--r--" ) );
    TEST( t.getMode() == (ios::open_mode)(ios::in | ios::out) );
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
			   ios::in );
    
    TESTR( t.error(), t.good() );

  
    TEST( ::compare( t.getFileName(), TestFn ) == 0 );
    TEST( ::compare( t.getAccess(), "-rw-r--r--" ) );
    TEST( t.getMode() == ios::in );
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
    ifstream  in( TestFn );

    TEST( in.good() );

    long    t;
    long    v = 0;
    for( ; in.read( (char *)&t, sizeof( t ) ).good(); ++ v )
      {
	TEST( t == v );
      }

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
