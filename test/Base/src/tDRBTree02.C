//
// File:        tDRBTree02.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDRBTree02
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 06:28
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <DRBTree.hh>
#include <MdbUtils.hh>
#include <MapMemDynamicDynamic.hh>
#include <MultiMemOffsetMapDynamic.hh>

#include <ClueUtils.hh>
#include <LibTest.hh>

#include <vector>
#include <functional>

struct Rec
{
  long	k;
  long	v;
};

inline
bool
operator < ( const Rec & one, const Rec & two )
{
  return( one.k < two.k );
}

typedef DRBTree< Rec, Rec, MdbIdent< Rec, Rec >, less< Rec > >   Tree;

tDRBTree02( LibTest & tester )
{
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDRBTree02.drbt",
				  (ios::open_mode)(ios::in|ios::out),
				  true,
				  1,
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdd, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0, true );

    TESTR( t.error(), t.good() );

    {
      Tree::pair_iterator_bool	ins;
      Rec r;
      
      for( long k = 4; k < 20; k += 2 )
	{
	  for( long e = 5; e < 30; e += 5 )
	    {
	      r.k = k;
	      r.v = (k * 100) + e;
	      
	      ins = t.insert( r, e );
	      TEST( ins.second );
	      TEST( (*ins.first).k == r.k );
	      TEST( (*ins.first).v == r.v );
	    }
	}
    }
    
    {
      Tree::const_iterator  it;
      Rec r;
      for( long k = 0; k < 25; ++ k )
	{
	  for( long e = 0; e < 50; ++ e )
	    {
	      r.k = k;
	      it = t.find( r, e );
	      
	      if( k >= 4 && k < 20 && (k % 2) == 0  && e >= 5 )
		{
		  TEST( it != t.end() );
		  TEST( (*it).k == k );
		  TEST( (*it).v == (k * 100) + RoundDown( min( e, 25L ), 5 ) );
		
		}
	      else
		{
		  TEST( it == t.end() );
		}
	    }
	}
    }
    
    {
      Tree::const_iterator them = t.begin();
      long	     k = 4;
      long	     e = 25;
      for( ; them != t.end(); ++ them )
	{
	  TEST( (*them).k == k );
	  TEST( (*them).v == (k * 100) + e );

	  e -= 5;
	  if( e == 0 )
	    {
	      k += 2;
	      e = 25;
	    }
	}
      TEST( them == t.end() );      
      TEST( k == 20 );
      TEST( e == 25 );
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
