//
// File:        tDRBTree04.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDRBTree04
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/22/97 07:42
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

#include <StlUtilsMisc.hh>
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

bool
tDRBTree04( LibTest & tester )
{
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDRBTree04.drbt",
				  (ios::open_mode)(ios::in|ios::out),
				  true,
				  1,
				  1,
				  02 );

    MultiMemOffsetMapDynamic	mmo( &mmdd, false );

    TESTR( mmo.error(), mmo.good() );
    
    Tree    t( &mmo, 0, true );

    TESTR( t.error(), t.good() );

    Rec r;
    Tree::size_type age_20_cnt = 0;
    
    {
      Tree::pair_iterator_bool	ins;

      Tree::size_type insCnt = 0;
      
      for( long k = 4; k < 50; ++ k )
	{
	  for( Tree::EffDate e = 5;
	       ( ( (k % 2) && e < 25) ||
		 ( ((k % 2) == 0) && e < 50 ) );
	       e += 5 )
	    {
	      r.k = k;
	      r.v = (k * 100) + e;

	      ++ insCnt;
	      if( e < 20 )
		++ age_20_cnt;
	      
	      ins = t.insert( r, e );
	      TEST( ins.second );
	      TEST( (*ins.first).k == r.k );
	      TEST( (*ins.first).v == r.v );
	    }
	}
      TEST( t.size() == insCnt );
    }

    {
      Tree::size_type  origSize = t.size();
      TEST( t.trim( 20 ) );
      
      TEST( t.size() == (origSize - age_20_cnt) );
      
      Tree::const_iterator  it;
      
      for( long k = 0; k < 75; ++ k )
	{
	  for( long e = 0; e < 75; ++ e )
	    {
	      r.k = k;
	      it = t.find( r, e );

	      if( k >= 4 && k < 50 && e >= 20 )
		{
		  TEST( it != t.end() );
		}
	      else
		{
		  TEST( it == t.end() );
		}
	    }
	}
    }

    {
      Tree::size_type  erase_30_cnt = 0;
      {
	Tree::size_type  origSize = t.size();
	for( long k = 5; k < 50; k += 2 )
	  {
	    ++ erase_30_cnt;
	    r.k = k;
	    TEST( t.erase( r, 30 ) );
	  }
	
	TEST( t.size() == origSize + erase_30_cnt );
      }
      
      {    
	Tree::const_iterator  it;
	
	for( long k = 0; k < 50; ++ k )
	  {
	    for( long e = 0; e < 75; ++ e )
	      {
		r.k = k;
		it = t.find( r, e );

		if( k >= 4 && k < 50 && e >= 20 && 
		    ( ( ( k % 2) && e < 30 ) ||
		      ( ( k % 2) == 0 ) ) )
		  {
		    TEST( it != t.end() );
		  }
		else
		  {
		    TEST( it == t.end() );
		  }
	      }
	  }
      }

      {
	TEST( t.size() == 184 );
	TEST( t.trim( 35 ) );
	TEST( t.size() == 46 );
      }
    }
    
    {
      Tree::const_iterator them = t.begin();
      long	     k = 4;
      long	     e = 45;
      Str errDesc;

      for( ; them != t.end(); ++ them )
	{
	  errDesc = "match: ";
	  errDesc << "k: " << k << " tk: " << (*them).k
		  << " e: " << e << " te: " << t.effective( them );
	  
	  
	  TESTR( errDesc, (*them).k == k );
	  TESTR( errDesc, t.effective( them ) == e );
	  TESTR( errDesc, (*them).v == (k * 100) + e );

	  e -= 5;

	  if( e == 30 )
	    {
	      e = 45;
	      k += 2;
	      
	    }
	}
      TEST( them == t.end() );      
      TEST( k == 50 );
      TEST( e == 45 );
    }
  }
  return( true );
}



// Revision Log:
//
// $Log$
// Revision 2.3  1997/09/17 16:56:13  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.2  1997/08/18 10:26:25  houghton
// Changed to test tree size.
//
// Revision 2.1  1997/07/22 19:46:59  houghton
// Initial Version.
//
//
