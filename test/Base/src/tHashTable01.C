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

#include <HashTable.hh>
#include <MdbUtils.hh>
#include <MapMemDynamicDynamic.hh>
#include <MultiMemOffsetMapDynamic.hh>
#include <TestConfig.hh>
#include <LibTest.hh>
#include <StlUtilsMisc.hh>
#include <functional>
#include <set>

struct Rec
{
  long	k;
  long	v;
};

inline
bool
operator < ( const Rec & a, const Rec & b )
{
  return( a.k < b.k );
}

struct HashRec
{
  HashTableBase::HashValue operator () ( const Rec & key ) const {
    return( abs( key.k ) );
  };
};

typedef set< long, less< long > > KeySet;


typedef HashTable< Rec, Rec, MdbIdent< Rec, Rec >,
		   HashRec, less< Rec > >		Table;


#define TEST_INDEX_FN	TEST_DATA_DIR "/tHashTable01.hash"
#define TEST_DATA_FN	TEST_DATA_DIR "/tTashTable01.data"

#define INSERT_TEST( k_ )						      \
{									      \
  Rec i;								      \
  i.k = k_;								      \
  i.v = k * 100;							      \
  									      \
  Table::size_type  origSize;						      \
									      \
  TESTR( "keys insert", keys.insert( i ).second );			      \
  TESTR( "insert", t.insert( i ).second );				      \
									      \
  TEST( t.size() == origSize + 1 );					      \
}

bool
tHashTable01( LibTest & tester )
{
#if defined( FIXME )
  KeySet  keys;
  
  {
    MapMemDynamicFixed	mmdf( TEST_DATA_FN,
			      (ios::open_mode)(ios::in|ios::out),
			      true,
			      Table::getNodeSize(),
			      1,
			      02 );

    TESTR( mmo.error(), mmo.good() );
    
    Table    t( &mmo,
		TEST_INDEX_FN, 
		(ios::open_mode)(ios::in|ios::out),
		true,
		02 );
    
    TESTR( t.error(), t.good() );

    TEST( t.size() == 0 );
    TEST( t.empty() );

    Rec	r;
    
    INSERT_TEST( 1000 );
    INSERT_TEST( 0 );
    INSERT_TEST( 50 );
    INSERT_TEST( 100 );

    {
      for( long k = 0; k < 500; k += 10 )
	{
	  if( k == 0 || k == 50 || k == 100 )
	    {
	      r.k = k;
	      TESTR( "ins dup", ! t.insert( i ).second );
	    }
	  else
	    {
	      INSERT_TEST( i );
	    }
	}
    }
    


  }
#endif
  return( true );
}



#if defined( OLD_TEST )
class HashLong
{
public:
  
  HashTableBase::HashValue	operator() ( long key ) const {
    return( abs( key % 100 ) );
  }
};

class LessLong
{
public:
  bool	operator () ( const long & one, const long & two ) const {
    return( one < two );
  };
};


bool
tHashTable01( LibTest & tester )
{
  {
    const char * TestDataFn = TEST_DATA_DIR "ht01.mdd";
    const char * TestIndexFn = TEST_DATA_DIR "ht01.hash";

    MapMemDynamicDynamic    map( TestDataFn, 16 );

    TESTR( map.error(), map.good() );

    MultiMemOffsetMapDynamic   mmo( &map, false );

    typedef HashTable< long, long,
		       MdbIdent< long, long >,
		       HashLong,
		       LessLong >	TestHash;
    
    TestHash	t( &mmo,
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
		 t.insert( data[i] ).second );
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

#endif

// Revision Log:
//
// $Log$
// Revision 1.6  1997/09/17 16:56:13  houghton
// Changed for new library rename to StlUtils
//
// Revision 1.5  1997/08/18 10:26:41  houghton
// Commented out test code (not ready).
//
// Revision 1.4  1997/07/25 13:50:18  houghton
// Reworking to do major portion of testing (work in progress).
//
// Revision 1.3  1997/07/22 19:47:28  houghton
// Bug-Fix: LessLong::operator () changed to const.
//
// Revision 1.2  1997/07/19 10:39:40  houghton
// Port(Sun5): HashTableBase::Hash was renamed to HashValue.
//
// Revision 1.1  1997/06/05 11:30:11  houghton
// Initial Version.
//
//
