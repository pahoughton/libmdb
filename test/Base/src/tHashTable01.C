//
// File:        tHashTable01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tHashTable01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/19/97 08:55
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <LibTest.hh>
#include <HashTable.hh>
#include <ClueUtils.hh>
#include <functional>
#include <projectn.h>

class HashFunction
{
public:
  
  HashTableBase::Hash	operator() ( long key ) const {
    return( abs( key % 100 ) );
  }
};

bool
tHashTable01( LibTest & tester )
{
  {
    const char * TestDataFn = TEST_DATA_DIR "ht01.mdd";
    const char * TestIndexFn = TEST_DATA_DIR "ht01.hash";

    MapMemDynamicDynamic    map( TestDataFn, 16 );

    TESTR( map.error(), map.good() );
    
    ChunkMgr		    chunkMgr( map );

    typedef HashTable< long, long, ident< long, long >, HashFunction, less< long > >	TestHash;
    
    TestHash	t( chunkMgr,
		   TestIndexFn,
		   (ios::open_mode)(ios::in | ios::out),
		   0002,
		   true );

    TESTR( t.error(), t.good() );

    static const long  data[] = { 5, 4, 8, 1, 2, 3, 9, 7, -1 };

    {
      for( int i = 0; data[i] != -1; ++ i )
	{
	  TESTR( t.error(),
		 t.insert( data[i], data[i] * 10 ).second );
	}
    }

    tester.getDump() << endl;
    
    for( TestHash::const_iterator them = t.begin();
	 them != t.end();
	 ++ them )
      {
	tester.getDump() << *them << endl;
	
	// TEST( (*them).first * 10 == (*them).second );
      }

#if 0
    for( int i = 0; data[i] != -1; ++ i )
      {
	TEST( t.get( data[i] ) == data[i] * 10 );
      }

    TEST( t.find( 10 ) == ChunkMgr::badLoc );

    TEST( t.insert( 5, 100 ) != ChunkMgr::badLoc );

    TEST( t.get( 5 ) == 100 );
    
    TEST( t.erase( 5 ) );

    TEST( t.find( 5 ) == ChunkMgr::badLoc );
    
    TEST( t.insert( 5, 200 ) != ChunkMgr::badLoc );

    TEST( t.find( 5 ) != ChunkMgr::badLoc );
    
    TEST( t.get( 5 ) == 200 );

#endif
  }

  return( true );
}

			   

// Revision Log:
//
// $Log$
// Revision 1.1  1997/06/05 11:30:11  houghton
// Initial Version.
//
//
