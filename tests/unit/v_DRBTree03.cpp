//
// File:        tDRBTree03.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDRBTree03
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/21/97 09:10
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
tDRBTree03( LibTest & tester )
{
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDRBTree03.drbt",
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
      Rec r;
      r.k = 4;
      
      TEST( t.erase( r, 25 ) );
      
      Tree::iterator it = t.find( r, 25 );
      
      TEST( it == t.end() );

      it = t.find( r, 24 );

      TEST( it != t.end() );
      TEST( (*it).k == 4 );
      TEST( (*it).v == (r.k * 100) + 20 );

      it = t.begin();
      
      TEST( it != t.end() );
      TEST( (*it).k == 4 );
      TEST( (*it).v == (r.k * 100) + 20 );
    }

    {
      Rec r;
      r.k = 18;
      TEST( t.erase( r, 0 ) );

      Tree::iterator it = t.find( r, 0 );
      
      TEST( it == t.end() );

      it = t.find( r, 5 );

      TEST( (*it).k == 18 );
      TEST( (*it).v == ( 18 * 100 ) + 5 );
      
      it = t.end();
      -- it;

      TEST( (*it).k == 18 );
      TEST( (*it).v == ( 18 * 100 ) + 5 );
    }

    {
      Rec r;
      r.k = 10;
      TEST( t.erase( r, 15 ) );

      Tree::iterator it;
      
      for( Tree::EffDate e = 15; e < 20; ++ e )
	{
	  TEST( t.find( r, e ) == t.end() );
	}

      it = t.find( r, 14 );
      TEST( it != t.end() );
      TEST( (*it).k == r.k );
      TEST( (*it).v == (r.k * 100) + 10 );

      it = t.find( r, 20 );
      TEST( it != t.end() );
      TEST( (*it).k == r.k );
      TEST( (*it).v == (r.k * 100) + 20 );
    }
    
    {
      Rec r;
      r.k = 8;
      for( Tree::EffDate e = 0; e < 35; e += 5 )
	{
	  TEST( t.erase( r, e ) );
	}

    }

    {
      Rec r;
      r.k = 16;
      for( Tree::EffDate e = 25; e > -1 ; e -= 5 )
	{
	  TEST( t.erase( r, e ) );
	}
    }

    {
      Rec r;

      for( long k = 1; k < 25; k += 2 )
	{
	  r.k = k;
	  TEST( ! t.erase( r, 10 ) );
	}
    }

    {
      Tree::const_iterator  it;
      
      Rec r;
      Str errDesc;
      for( long k = 0; k < 25; ++ k )
	{
	  for( long e = 0; e < 50; ++ e )
	    {
	      r.k = k;
	      it = t.find( r, e );

	      if( ( k == 4 && ( e >= 5 && e < 25 ) ) ||
		  ( k == 10 && ( ( e >= 5 && e < 15 ) ||
				 ( e >= 20 ) ) ) ||
		  ( k > 4 && k < 20 && ( k % 2 ) == 0 &&
		    k != 8 && k != 10 && k != 16 && e >= 5 ) )
		{
		  errDesc = "not found: ";
		  errDesc << k << " " << e;
		  
		  TESTR( errDesc, it != t.end() );
		  TEST( (*it).k == k );
		  TEST( (*it).v == (k * 100) + RoundDown( min( e, 25L ), 5 ) );
		}
	      else
		{
		  errDesc = "found: ";
		  errDesc << k << " " << e;
		  
		  TESTR( errDesc, it == t.end() );
		}
	    }
	}
    }
    	    
    {
      Tree::const_iterator them = t.begin();
      long	     k = 4;
      long	     e = 20;
      Str errDesc;

      cout << '\n';
      for( ; them != t.end(); ++ them )
	{
	  errDesc = "match: ";
	  errDesc << "k: " << k << " tk: " << (*them).k
		  << " e: " << e << " te: " << t.effective( them );
	  
	  TESTR( errDesc, (*them).k == k );
	  TESTR( errDesc, t.effective( them ) == e );
	  TESTR( errDesc, (*them).v == (k * 100) + e );

	  e -= 5;

	  if( k == 10 && e == 15 ) {
	    e -= 5;
	  }
	  
	  if( e == 0 ) {
	    e = 25;
	    k += 2;
	    
	    if( k == 8 || k == 16 )
	      k += 2;
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
// Revision 4.2  2003/09/18 15:15:54  houghton
// Cleanup.
//
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 2.2  1997/09/17 16:56:12  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.1  1997/07/22 19:46:39  houghton
// Initial Version.
//
//
