//
// File:        tHash01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tHash01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/02/97 05:58
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <MapMemDynamicFixed.hh>
#include <MultiMemOffsetMapDynamic.hh>
#include <Hash.hh>
#include <LibTest.hh>
#include <iomanip>

struct TestKey
{
  long	    h;
  short	    v;
};

struct TestRec : public TestKey 
{
  char	data[16];
};

inline
ostream &
operator << ( ostream & dest, const TestRec & obj )
{
  dest << setw(4) << obj.h << ' ' << setw(2) << obj.v << ' ' << obj.data;
  return( dest );
}

inline
bool
operator == ( const TestRec & one, const TestRec & two )
{
  return( one.h == two.h &&
	  one.v == two.v &&
	  strcmp( one.data, two.data ) == 0 );
}

class TestHashFunct
{
public:
  HashTableBase::Hash	operator () ( const TestRec & key ) const {
    return( key.h );
  };
};

class TestLess
{
public:
  bool	    operator () ( const TestRec & one, const TestRec & two ) const {
    return( one.h < two.h ? true :
	    two.h < one.h ? false :
	    bool( one.v < two.v ) );
  };
};

class TestHash : public Hash< TestRec, TestHashFunct, TestLess >
{
public:

  TestHash( MultiMemOffset *	memMgr,
	    const char *	indexFileName,
	    ios::open_mode	mode = ios::in,
	    unsigned short	permMask = 02,
	    bool		create = false ) :
    Hash< TestRec, TestHashFunct, TestLess >( memMgr,
					      indexFileName,
					      mode,
					      permMask,
					      create ) {};
  

  ostream & dumpKey( ostream & dest, const TestRec & rec ) const {
    dest << "key: " << rec.h << " " << rec.v;
    return( dest );
  };
};
    
      
typedef vector< TestRec >   List;

bool
tHash01( LibTest & tester )
{
  {
    MapMemDynamicFixed	     mmdf( TEST_DATA_DIR "/tHash01.mdf",
				   TestHash::getNodeSize(),
				   1,
				   02 );
      
    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TESTR( mmo.error(), mmo.good() );
    
    TestHash	t( &mmo,
		   TEST_DATA_DIR "/tHash01.hash",
		   (ios::open_mode)(ios::in|ios::out),
		   true,
		   02 );

    TESTR( t.error(), t.good() );

    List	recList;
    
    TestRec	rec;
    Str		str;
    
    for( long k = 1; k < 10; ++ k )
      {
	rec.h = k;
	rec.v = 1;
	if( (rec.h % 2) == 0 )
	  {
	    if( (rec.h % 4) == 0 )
	      {
		for( short v = 0; v < 12; v += 3 )
		  {
		    rec.v = v;
		    str = "";
		    str << "h " << rec.h << " v " << rec.v;
		    str.copy( rec.data, sizeof( rec.data ) );
		    recList.push_back( rec );
		  }
	      }
	    else
	      {
		for( short v = 5; v < 15; v += 5 )
		  {
		    rec.v = v;
		    str = "";
		    str << "h " << rec.h << " v " << rec.v;
		    str.copy( rec.data, sizeof( rec.data ) );
 		    recList.push_back( rec );
		  }
	      }
	  }
	else
	  {
	    rec.v = 1;
	    str = "";
	    str << "h " << rec.h << " v " << rec.v;
	    str.copy( rec.data, sizeof( rec.data ) );
	    recList.push_back( rec );
	  }
      }

    // tester.getDump() << '\n';
    {
      // insert( const Key & key )
      for( List::iterator them = recList.begin();
	   them != recList.end();
	   ++ them )
	{
	  TESTR( t.error(), t.insert( *them ).second == true );
	}
    }
    // t.dumpTable( tester.getDump() ) << endl;

    {
      // find( const Key & key )
      TestRec badRec;

      for( List::iterator them = recList.begin();
	   them != recList.end();
	   ++ them )
	{
	  TestHash::iterator it = t.find( *them );
	  TESTR( "not found", it != t.end() );
	  TESTR( "match", *it == *them );

	  badRec = *them;
	  badRec.v += 1;
	  it = t.find( badRec );
	  TESTR( "found bad", it == t.end() );
	  badRec.v -= 2;
	  it = t.find( badRec );
	  TESTR( "found bad", it == t.end() );
	}
    }

    TESTP( true );
    
    {
      TestHash::iterator it;
      // erase( const Key & key )
      for( int r = 0; r < recList.size(); ++ r )
	{
	  if( (r % 2) == 0 )
	    {
	      TESTR( t.error(), t.erase( recList[r] ) );
	      TESTR( "found erased", t.find( recList[r] ) == t.end() );
	    }
	  else
	    {
	      it = t.find( recList[r] );
	      TESTR( "not found", it != t.end() );
	      TESTR( "match", *it == recList[r] );
	    }
	}
    }
    // t.dumpInfo( tester.getDump() );
    
  }

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 2.2  1997/07/14 10:47:05  houghton
// Bug-Fix: added missing return statement.
// Port(AIX): added bool constructor for the '?:' operator.
//
// Revision 2.1  1997/07/11 17:39:10  houghton
// Initial Version.
//
//
