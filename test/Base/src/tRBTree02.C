//
// File:        tRBTree02.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tRBTree02
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/06/97 03:58
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <RBTree.hh>
#include <MdbUtils.hh>
#include <MapMemDynamicFixed.hh>
#include <MultiMemOffsetMapDynamic.hh>

#include <LibTest.hh>
#include <vector>
#include <functional>
#include <set>
#include <algorithm>

struct Rec
{
  long	    k;
  long	    v;
};

class LessRec
{
public:
  inline bool	operator () ( const Rec & one, const Rec & two ) const {
    return( one.k < two.k );
  };
};

inline
ostream &
operator << ( ostream & dest, const Rec & obj )
{
  dest << obj.k;
  return( dest );
}

typedef RBTree< Rec, Rec, MdbIdent< Rec, Rec >, LessRec >   Tree;
typedef vector< Rec >	List;
typedef set< Rec, LessRec >   Set;
typedef vector< long >	KeyList;

#define INSERT_TEST( value )						      \
    i.k = value;							      \
    i.v = i.k * 2;							      \
									      \
    TEST( recSet.insert( i ).second );					      \
    TESTR( t.error(), t.insert( i ).second );				      \
    if( ! t.testTree( tester.getDump() ) )				      \
      {									      \
        t.dumpTree( tester.getDump() ) << '\n';				      \
	TEST( false );							      \
      }									      \
									      \
    {									      \
      long fc = 0;							      \
      Rec  f;								      \
      f.k = (*recSet.begin()).k - 1;					      \
      TEST( t.find( f ) == t.end() );					      \
      f.k = (*recSet.rbegin()).k + 1;					      \
      TEST( t.find( f ) == t.end() );					      \
      Tree::iterator it;						      \
      for( Set::iterator rThem = recSet.begin();			      \
	   rThem != recSet.end();					      \
	   ++ rThem )							      \
	{								      \
	  it = t.find( *rThem );					      \
	  TEST( it != t.end() );					      \
	  TEST( (*it).k == (*rThem).k );				      \
	  TEST( (*it).v == (*rThem).v );				      \
	  ++ fc;							      \
	}								      \
      TEST( fc == t.size() );						      \
      TEST( t.size() == recSet.size() );				      \
    }
		

#define ERASE_TEST( value )						      \
  {									      \
    Rec e;								      \
    e.k = value;							      \
    recSet.erase( e );							      \
    TESTR( "erase", t.erase( e ) );					      \
    TESTR( "find e", t.find( e ) == t.end() );				      \
    if( ! t.testTree( tester.getDump() ) )				      \
      {									      \
        t.dumpTree( tester.getDump() ) << '\n';				      \
	TEST( false );							      \
      }									      \
    if( ! t.empty() )							      \
      {									      \
	long b = (*recSet.begin()).k;					      \
	long e = (*recSet.rbegin()).k;					      \
	long fc = 0;							      \
	long nc = 0;							      \
	Rec  f;								      \
	for( b -= 1, e += 1; b <= e ; ++ b )				      \
	  {								      \
	    f.k = b;							      \
	    if( recSet.find( f ) != recSet.end() )			      \
	      {								      \
		TESTR( "find", t.find( f ) != t.end() );		      \
		TESTR( "k", (*t.find( f )).k == b );			      \
		TESTR( "v", (*t.find( f )).v == ( b * 2 ) );		      \
		++ fc;							      \
	      }								      \
	    else							      \
	      {								      \
		TESTR( "! find", t.find( f ) == t.end() );		      \
		++ nc;							      \
	      }								      \
	  }								      \
	TESTR( "fc", fc == t.size() );					      \
	TESTR( "nc", nc > 1 );						      \
	TESTR( "size", t.size() == recSet.size() );			      \
      }									      \
  }
    
bool
tRBTree02( LibTest & tester )
{
  Set	  recSet;

  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree02.rbt",
				  (ios::open_mode)(ios::in|ios::out),
				  true,
				  Tree::getNodeSize(),
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0, true );

    TESTR( t.error(), t.good() );

    Rec	  i;
    
    TESTP( true );
    tester.getDump() << "insert";
    INSERT_TEST( 15 );
    INSERT_TEST( 11 );
    {
      for( long k = 10; k < 20; ++ k )
	{
	  if( k == 11 || k == 15 )
	    {
	      i.k = k;
	      TESTR( "ins dup", t.insert( i ).second == false );
	    }
	  else
	    {
	      INSERT_TEST( k );
	    }
	}
    }

    TESTP( true );
    {
      for( long k = 9; k >= 0; -- k )
	{
	  INSERT_TEST( k );
	}
    }

    TESTP( true );
    {
      KeyList randKeys;
      
      for( long k = 20; k < 70; ++ k )
	{
	  randKeys.push_back( k );
	}

      random_shuffle( randKeys.begin(), randKeys.end() );

      for( KeyList::iterator them = randKeys.begin();
	   them != randKeys.end();
	   ++ them )
	{
	  INSERT_TEST( *them );
	}
    }
    
  }

  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree02.rbt",
				  ios::in );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TESTR( mmo.error(), mmo.good() );
    
    const Tree    t( &mmo, 0 );

    TESTR( t.error(), t.good() );

    TESTP( true );
    tester.getDump() << "find";
    {
      // find( const Key & key ) const
      Tree::const_iterator  it;
      for( Set::iterator rThem = recSet.begin();
	   rThem != recSet.end();
	   ++ rThem )
	{
	  it = t.find( *rThem );
	  TEST( it != t.end() );
	  TEST( (*it).k == (*rThem).k &&
		(*it).v == (*rThem).v );
	}
    }

    TESTP( true );
    tester.getDump() << "begin";
    {
      // begin() const
      // end() const
      Set::iterator	    rThem = recSet.begin();
      Tree::const_iterator  them = t.begin();
      long  pk = -1;
      long  cnt = 0;
      
      for( ; them != t.end() && rThem != recSet.end();
	   ++ them, ++ rThem, ++ pk )
	{
	  TEST( (*them).k == (*rThem).k &&
		(*them).v == (*rThem).v );
	  TEST( pk + 1 == (*them).k );
	  ++ cnt;
	}
      TEST( them == t.end() );
      TEST( rThem == recSet.end() );
      TEST( cnt == t.size() );
    }

    TESTP( true );
    tester.getDump() << "rbegin";
    {
      // rbegin() const
      // rend() const
      
      Set::reverse_iterator	    rThem = recSet.rbegin();
      Tree::const_reverse_iterator  them = t.rbegin();
      long  pk = (*rThem).k + 1;
      long  cnt = 0;
      
      for( ; them != t.rend() && rThem != recSet.rend();
	   ++ them, ++ rThem, -- pk )
	{
	  TEST( (*them).k == (*rThem).k &&
		(*them).v == (*rThem).v );
	  TEST( pk - 1 == (*them).k );
	  ++ cnt;
	}
      TEST( them == t.rend() );
      TEST( rThem == recSet.rend() );
      TEST( cnt == t.size() );
    }
    TESTP( true );
  }

  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree02.rbt",
				  (ios::open_mode)(ios::in|ios::out),
				  false );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0 );

    TESTR( t.error(), t.good() );

    TESTP( true );
    tester.getDump() << "erease";
    // t.dumpTree( tester.getDump() ) << '\n';
    ERASE_TEST( 69 );	// last
    ERASE_TEST( 0 );	// first
    // t.dumpInfo( tester.getDump() ) << '\n';
    ERASE_TEST( 13 );	// root
    // t.dumpInfo( tester.getDump() ) << '\n';
    ERASE_TEST( 49 );
    ERASE_TEST( 61 );
    ERASE_TEST( 38 );
    ERASE_TEST( 1 );
    ERASE_TEST( 68 );
    ERASE_TEST( 50 );
    ERASE_TEST( 58 );
    ERASE_TEST( 56 );
    ERASE_TEST( 57 );
    ERASE_TEST( 55 );
    ERASE_TEST( 54 );
    ERASE_TEST( 53 );
    ERASE_TEST( 5 );
    ERASE_TEST( 6 );
    ERASE_TEST( 7 );
    ERASE_TEST( 8 );
    ERASE_TEST( 2 );
    
    TESTP( true );
    {
      KeyList erList;
      
      for( Tree::iterator them = t.begin();
	   them != t.end();
	   ++ them )
	erList.push_back( (*them).k );

      for( KeyList::reverse_iterator erThem = erList.rbegin();
	   erThem != erList.rend();
	   ++ erThem )
	{
	  ERASE_TEST( *erThem );
	}
      
    }

    TESTP( t.size() == 0 );
    // t.dumpInfo( tester.getDump() );
    // t.dumpTree( tester.getDump() );
    
  }

  {
    MapMemDynamicFixed	    mmdf( TEST_DATA_DIR "/tRBTree02.rbt",
				  (ios::open_mode)(ios::in|ios::out),
				  false );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0 );

    TESTR( t.error(), t.good() );

    long    first = 10;
    long    last = 200000;
    KeyList keys;
    
    {
      for( long k = first; k < last; k += 2 )
	{
	  keys.push_back( k );
	}
    }

    random_shuffle( keys.begin(), keys.end() );
    TESTP( true );
    tester.getDump() << "insert";
    {
      Rec i;
      long progress = 0;
      for( KeyList::iterator kThem = keys.begin();
	   kThem != keys.end();
	   ++ kThem )
	{
	  i.k = *kThem;
	  i.v = i.k * 2;
	  
	  TEST( recSet.insert( i ).second );
	  TESTR( t.error(), t.insert( i ).second );
	  ++ progress;
	  if( (progress % 1000) == 0 )
	    TESTP( true );
	  
	}

      if( ! t.testTree( tester.getDump() ) )
	{
	  t.dumpTree( tester.getDump() ) << '\n';
	  TEST( false );
	}
    }
    TESTP( true );
    tester.getDump() << "insert dup";
    {
      Rec i;
      for( KeyList::iterator kThem = keys.begin();
	   kThem != keys.end();
	   ++ kThem )
	{
	  i.k = *kThem;
	  i.v = *kThem - 1;
	  TESTR( "ins dup", ! t.insert( i ).second );
	}
    }
    TESTP( true );
    TEST( t.size() == keys.size() );
    
    tester.getDump() << "find";
    {
      Tree::const_iterator  it;
      Rec		    r;
      long		    fcount = 0;
      for( long k = 0; k < (last + 10); ++ k )
	{
	  r.k = k;
	  it = t.find( r );

	  if( k >= first && k < last && ( k % 2 ) == 0 )
	    {
	      ++ fcount;
	      TEST( it != t.end() );
	      TEST( (*it).k == k );
	      TEST( (*it).v == (k * 2) );
	    }
	  else
	    {
	      TEST( it == t.end() );
	    }
	}
      TEST( fcount == t.size() );
    }

    random_shuffle( keys.begin(), keys.end() );

    TESTP( true );
    tester.getDump() << "erase";
    {
      Rec e;
      long progress = 0;
      for( KeyList::iterator kThem = keys.begin();
	   kThem != keys.end();
	   ++ kThem )
	{
	  e.k = *kThem;
	  recSet.erase( e );
	  TESTR( "erase", t.erase( e ) );
	  TESTR( "find e", t.find( e ) == t.end() );
	  ++ progress;
	  if( (progress % 1000) == 0 )
	    {
	      if( ! t.testTree( tester.getDump() ) )
		{
		  t.dumpTree( tester.getDump() ) << '\n';
		  TEST( false );
		}
	      TESTP( true );
	    }
	}
      
      if( ! t.testTree( tester.getDump() ) )
	{
	  t.dumpTree( tester.getDump() ) << '\n';
	  TEST( false );
	}
    }

    TEST( t.size() == 0 );

  }
    
  return( true );
}
	
      
    
// Revision Log:
//
// $Log$
// Revision 2.3  1997/07/19 10:40:47  houghton
// Bug-Fix: added include <algorithm>
//
// Revision 2.2  1997/07/14 10:50:06  houghton
// Bug-Fix: added if( ! empty ) to ERASE_TEST. Not supose to use
//     iterators when a collection is empty!
//
// Revision 2.1  1997/07/11 17:39:38  houghton
// Initial Version.
//
//
