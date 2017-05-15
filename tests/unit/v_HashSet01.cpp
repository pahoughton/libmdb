// 1997-07-02 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/HashSet.hpp>
#include <mdb/MapMemDynamicFixed.hpp>
#include <mdb/MultiMemOffsetMapDynamic.hpp>
#include <valid/verify.hpp>

#include <iomanip>
#include <cstring>


#define TEST_DATA_DIR "data"
#define TEST VVTRUE

using namespace mdb;
using namespace clue;

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
std::ostream &
operator << ( std::ostream & dest, const TestRec & obj )
{
  dest << std::setw(4) << obj.h << ' '
       << std::setw(2) << obj.v << ' ' << obj.data;
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
  HashTableBase::HashValue	operator () ( const TestRec & key ) const {
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

class TestHash : public HashSet< TestRec, TestHashFunct, TestLess >
{
public:

  TestHash( MultiMemOffset *	memMgr,
	    const char *	indexFileName,
	    std::ios::openmode	mode = std::ios::in,
	    unsigned short	permMask = 02,
	    bool		create = false ) :
    HashSet< TestRec, TestHashFunct, TestLess >( memMgr,
						 indexFileName,
						 mode,
						 permMask,
						 create ) {};


  std::ostream & dumpKey( std::ostream & dest, const TestRec & rec ) const {
    dest << "key: " << rec.h << " " << rec.v;
    return( dest );
  };
};


typedef std::vector< TestRec >   List;

valid::verify &
v_HashSet01( void )
{
  static VVDESC( "mdb::HashSet01" );

  {
    MapMemDynamicFixed	     mmdf( TEST_DATA_DIR "/tHash01.mdf",
				   TestHash::getNodeSize(),
				   1,
				   02 );

    MultiMemOffsetMapDynamic	mmo( &mmdf, false );

    TEST( mmo.good() );

    TestHash	t( &mmo,
		   TEST_DATA_DIR "/tHashSet01.hash",
		   std::ios::in|std::ios::out,
		   true,
		   02 );

    TEST( t.good() );

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
	  TEST( t.insert( *them ).second == true );
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
	  TEST( it != t.end() );
	  TEST( *it == *them );

	  badRec = *them;
	  badRec.v += 1;
	  it = t.find( badRec );
	  TEST( it == t.end() );
	  badRec.v -= 2;
	  it = t.find( badRec );
	  TEST( it == t.end() );
	}
    }

    {
      TestHash::iterator it;
      // erase( const Key & key )
      for( int r = 0; r < recList.size(); ++ r )
	{
	  if( (r % 2) == 0 )
	    {
	      TEST( t.erase( recList[r] ) );
	      TEST( t.find( recList[r] ) == t.end() );
	    }
	  else
	    {
	      it = t.find( recList[r] );
	      TEST( it != t.end() );
	      TEST( *it == recList[r] );
	    }
	}
    }
    // t.dumpInfo( tester.getDump() );

  }

  return( VALID_VALIDATOR );
}
