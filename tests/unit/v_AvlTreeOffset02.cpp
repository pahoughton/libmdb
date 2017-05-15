//
// File:        tAvlTreeOffset02.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tAvlTreeOffset02
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/01/97 05:15
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <TestAvlTreeOffset.hh>
#include <LibTest.hh>
#include <Net2Host.hh>

#include <algorithm>
#include <fstream>

static long TotalRecs = 10000;

typedef vector< TestAvlTreeOffset::Rec >    List;

static long WalkCounter = 0;

struct WalkData
{
  List::iterator    it;
  LibTest &	    tester;

  WalkData( List::iterator i, LibTest & t ) : it( i ), tester( t ) {};
};

bool
operator < ( const TestAvlTreeOffset::Rec & one,
	     const TestAvlTreeOffset::Rec & two )
{
  return( ::compare( one.k, two.k ) < 0 );
}

bool
TestIterateWalk( TestAvlTreeOffset::Rec & r, void * closure )
{
  Str eStr;
  
  WalkData *	    wd = (WalkData *)closure;

  LibTest &	    tester( wd->tester );
  
  List::iterator &  it = wd->it;

  eStr << "rk: '" << r.k << "' ik: '" << (*it).k << "'\n"
       << "rl: '" << r.l << "' il: '" << (*it).l << "'\n"
       << "rd: '" << r.d << "' id: '" << (*it).d << "'" << endl;
  
  TEST( ::compare( r.k, (*it).k ) == 0 &&
	::compare( r.l, (*it).l ) == 0 &&
	::compare( r.d, (*it).d ) == 0 );
  
  ++ it;

  ++ WalkCounter;
  
  return( false );
}

  
bool
tAvlTreeOffset02( LibTest & tester )
{
  List	    recList;
  
  {
    ifstream f( TEST_DATA_DIR "/TestAvlTreeOffset.data" );

    TEST( f.good() );

    TestAvlTreeOffset::Rec  r;

    while( f.read( (char *)&r, sizeof( r ) ).good() )
      {
	r.l = Net2Host( r.l );
	recList.push_back( r );
      }
  }

  TESTP( true );
  
  {
    TestAvlTreeOffset	t( TEST_DATA_DIR "/tAvlTreeOffset02.avl",
			   (std::ios::openmode)(ios::in | ios::out),
			   true );

    TESTR( t.error(), t.good() );

    {
      TestAvlTreeOffset::Tree::Loc l;
      Str errStr;
      
      for( List::iterator them = recList.begin();
	   them != recList.end();
	   ++ them )
	{
	  l = t.tree().add( *them );
	  if( l != 0 )
	    {
	      errStr << "add failed: '" << (*them).k
		     << "' ret: " << l
		     << " k: '" << t.tree()[l].k << "'.";
		
	      TESTR( errStr, false );
	    }
	}
    }

    TESTR( "tree size", t.tree().size() == recList.size() );
    
  }

  {
    TestAvlTreeOffset	    t( TEST_DATA_DIR "/tAvlTreeOffset02.avl",
			       (std::ios::openmode)(ios::in | ios::out),
			       false );

    TESTR( t.error(), t.good() );
    TESTR( "tree size", t.tree().size() == recList.size() );
    
    {
      // two writer's cant open.
      
      TestAvlTreeOffset	    t2( TEST_DATA_DIR "/tAvlTreeOffset02.avl",
				(std::ios::openmode)(ios::in | ios::out),
				false );
      
      TESTR( "two writers", t.good() );
      TESTR( "two writers", ! t2.good() );
    }

    {
      // a writer & 2 readers should be ok
      
      TestAvlTreeOffset	    t2( TEST_DATA_DIR "/tAvlTreeOffset02.avl",
				ios::in,
				false );
      
      TESTR( "reader", t2.good() );

      const TestAvlTreeOffset	    t3( TEST_DATA_DIR "/tAvlTreeOffset02.avl",
					ios::in,
					false );
      
      TESTR( "reader 2", t3.good() );

      TESTR( "t & t2 same base",
	     t.tree().getMemMgr()->address( 0 ) !=
	     t2.tree().getMemMgr()->address( 0 ) );
      TESTR( "t & t3 same base",
	     t.tree().getMemMgr()->address( 0 ) !=
	     t3.tree().getMemMgr()->address( 0 ) );
      TESTR( "t2 & t3 same base",
	     t2.tree().getMemMgr()->address( 0 ) !=
	     t3.tree().getMemMgr()->address( 0 ) );

      int rec = 47;

      TEST( rec < recList.size() );

      TestAvlTreeOffset::Tree::Loc  found = t.tree().find( recList[ rec ] );

      TEST( found != 0 );

      TestAvlTreeOffset::Tree::Loc  t2f = t2.tree().find( recList[ rec ] );

      TEST( t2f == found );
      
      TestAvlTreeOffset::Tree::Loc  t3f = t3.tree().find( recList[ rec ] );

      TEST( t3f == found );

      TEST( ::compare( t3.tree()[ t3f ].k, recList[ rec ].k ) == 0 &&
	    ::compare( t3.tree()[ t3f ].l, recList[ rec ].l ) == 0 &&
	    ::compare( t3.tree()[ t3f ].d, recList[ rec ].d ) == 0 );
      
      TEST( ::compare( t.tree()[ found ].d, t2.tree()[ t2f ].d ) == 0 );
      TEST( ::compare( t.tree()[ found ].l, t3.tree()[ t3f ].l ) == 0 );
      
      TESTR( t.error(), t.tree().del( recList[ rec ] ) );
      TESTR( t.error(), t.good() );

      found = t.tree().find( recList[ rec ] );
      TEST( found == 0 );

      t2f = t2.tree().find( recList[ rec ] );
      TEST( t2f == 0 );

      t3f = t3.tree().find( recList[ rec ] );
      TEST( t3f == 0 );

      TEST( t.tree().add( recList[ rec ] ) == 0 );
	
      t3f = t3.tree().find( recList[ rec ] );
      TEST( t3f != 0 );

      TEST( ::compare( t3.tree()[ t3f ].k, recList[ rec ].k ) == 0 &&
	    ::compare( t3.tree()[ t3f ].l, recList[ rec ].l ) == 0 &&
	    ::compare( t3.tree()[ t3f ].d, recList[ rec ].d ) == 0 );

      TestAvlTreeOffset::Rec *	tr = t.tree().findData( recList[ rec ] );

      TEST( tr != 0 );

      found = t.tree().find( recList[ rec ] );
      TEST( &t.tree()[ found ] == tr );
      
    }

    TESTP( true );
    
    List  orderList( recList.begin(), recList.end() );
    
    sort( orderList.begin(), orderList.end() );

    TESTP( true );

    {
      // walk

      WalkData wd( orderList.begin(), tester );
      
      TESTR( "walk error",
	     ! t.tree().walk( &wd, TestIterateWalk ) );

      TEST( wd.it == orderList.end() );
      TEST( WalkCounter == t.tree().size() );
    }

    TESTP( true );
    
    {
      // destroy
      
      t.tree().destroy();
      TESTR( t.error(), t.good() );
      
      TESTR( "not empty", t.tree().size() == 0 );

    }

    TESTP( true );
    
    {
      // now fill it back up and walk it again
      {
	TestAvlTreeOffset::Tree::Loc l;
	Str errStr;
      
	for( List::iterator them = orderList.begin();
	     them != orderList.end();
	     ++ them )
	  {
	    l = t.tree().add( *them );
	    if( l != 0 )
	      {
		errStr << "add failed: '" << (*them).k
		       << "' ret: " << l
		       << " k: '" << t.tree()[l].k << "'.";
		
		TESTR( errStr, false );
	      }
	  }
      }
      
      WalkData wd( orderList.begin(), tester );

      WalkCounter = 0;
      
      TESTR( "walk error",
	     ! t.tree().walk( &wd, TestIterateWalk ) );

      TEST( wd.it == orderList.end() );
      // tester.getDump() << "wc: " << WalkCounter << " ts: " << t.tree().size()
      //       << endl;
      
      TEST( WalkCounter == t.tree().size() );
    }

    TESTR( t.error(), t.good() );
    
  }
  return( true );
}


// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 2.3  1997/07/19 10:36:32  houghton
// Bug-Fix; added include <algorithm>
//
// Revision 2.2  1997/07/14 10:45:26  houghton
// Port(AIX): had to case ofstream::read arg to 'char *'.
//
// Revision 2.1  1997/07/11 17:38:56  houghton
// Initial Version.
//
//
