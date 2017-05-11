//
// File:        tAvlTreeOffset01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tAvlTreeOffset01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/30/97 10:11
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <AvlTreeOffset.hh>
#include <MapMemDynamicFixed.hh>
#include <MultiMemOffsetMapDynamic.hh>
#include <Compare.hh>
#include <LibTest.hh>

#include <algorithm>

static
int
compareLong( const long & one, const long & two )
{
  return( ::compare( one, two ) );
}

static
long &
copyLong( long & dest, const long & src )
{
  dest = src;
  return( dest );
}


bool
tAvlTreeOffset01( LibTest & tester )
{
  ostream & tout = tester.getDump();
  
  {
    typedef AvlTreeOffset< long >   Tree;
    typedef vector< long >	    List;

    List			    keys;
    
    MapMemDynamicFixed *	mmdf = 0;
    MultiMemOffsetMapDynamic *	mmo = 0;

    mmdf = new MapMemDynamicFixed( TEST_DATA_DIR "/tAvlOffset01.avl",
				   Tree::getNodeSize(),
				   1,
				   02 );
    TESTR( "no mmdf", mmdf != 0 );
    TESTR( mmdf->error(), mmdf->good() );
    
    mmo = new MultiMemOffsetMapDynamic( mmdf, false );
    TESTR( "no mmo", mmo != 0 );
    TESTR( mmo->error(), mmo->good() );
    
    Tree    t( compareLong, copyLong, mmo, 0, true );

    TESTR( t.error(), t.good() );

    Tree::Loc	l;

    {
      // ordered insert
      for( int i = 0 ; i < 1000; ++ i )
	{
	  keys.push_back( i );
	  l = t.add( i );
	  TESTR( t.error(), t.good() );
	  if( l != 0 )
	    {
	      tout << t.dump() << endl;
	      t.dumpTree( tout ) << endl;
	      tout << "l: " << l << " i: " << i << '\n';
	      TESTR( "add failed.", false );
	    }
	}

      TESTR( t.error(), t.good() );
      TESTR( "size match", t.size() == keys.size() );
    }

    {
      // ordered find
      for( List::iterator them = keys.begin();
	   them != keys.end();
	   ++ them )
	{
	  l = t.find( *them );
	  if( l == 0 )
	    {
	      tout << "Finding: " << *them << endl;
	      TESTR( "Find failed.", false );
	    }
	  else
	    {
	      TESTR( "match error",
		     (*them) == t[ l ] );
	    }
	}
    }

    {
      // ordered del
      Tree::size_type ts = t.size();
      
      for( List::iterator them = keys.begin();
	   them != keys.end();
	   ++ them )
	{
	  -- ts;
	  TEST( t.del( *them ) );
	  TESTR( t.error(), t.good() );
	  TESTR( "size match", t.size() == ts );

	  l = t.find( *them );
	  if( l != 0 )
	    {
	      tout << "Finding: " << *them << endl;
	      TESTR( "Found deleted.", false );
	    }
	}

      TESTR( "not empty", t.size() == 0 );
    }

    TESTP( true );
    
    {
      // reverse order insert
      List  revKeys;

      {
	for( List::reverse_iterator rthem = keys.rbegin();
	     rthem != keys.rend();
	     ++ rthem )
	  revKeys.push_back( *rthem );
      }
      
      TESTR( "revKeys",
	     keys.size() == revKeys.size() &&
	     keys.front() == revKeys.back() &&
	     keys.back() == revKeys.front() );

      {
	for( List::iterator them = revKeys.begin();
	     them != revKeys.end();
	     ++ them )
	  {
	    l = t.add( *them );
	    TESTR( t.error(), t.good() );
	    if( l != 0 )
	      {
		tout << t.dump() << endl;
		t.dumpTree( tout ) << endl;
		tout << "l: " << l << " i: " << *them << '\n';
		TESTR( "add failed.", false );
	      }
	  }
	
	TESTR( t.error(), t.good() );
	TESTR( "size match", t.size() == revKeys.size() );
      }

      {
	// reverse ordered find
	for( List::iterator them = revKeys.begin();
	     them != revKeys.end();
	     ++ them )
	  {
	    l = t.find( *them );
	    if( l == 0 )
	      {
		tout << "Finding: " << *them << endl;
		TESTR( "Find failed.", false );
	      }
	    else
	      {
		TESTR( "match error",
		       (*them) == t[ l ] );
	      }
	  }
      }
      
      {
	// reverse ordered del
	Tree::size_type ts = t.size();
      
	for( List::iterator them = revKeys.begin();
	     them != revKeys.end();
	     ++ them )
	  {
	    -- ts;
	    TEST( t.del( *them ) );
	    TESTR( t.error(), t.good() );
	    TESTR( "size match", t.size() == ts );

	    l = t.find( *them );
	    if( l != 0 )
	      {
		tout << "Finding: " << *them << endl;
		TESTR( "Found deleted.", false );
	      }
	  }

	TESTR( "not empty", t.size() == 0 );
      }
      
      
    }

    TESTP( true );
    
    {
      // rand order insert
      List  randKeys( keys.begin(), keys.end() );

      random_shuffle( randKeys.begin(), randKeys.end() );
      
      {
	for( List::iterator them = randKeys.begin();
	     them != randKeys.end();
	     ++ them )
	  {
	    l = t.add( *them );
	    TESTR( t.error(), t.good() );
	    if( l != 0 )
	      {
		tout << t.dump() << endl;
		t.dumpTree( tout ) << endl;
		tout << "l: " << l << " i: " << *them << '\n';
		TESTR( "add failed.", false );
	      }
	  }
	
	TESTR( t.error(), t.good() );
	TESTR( "size match", t.size() == randKeys.size() );
      }

      random_shuffle( randKeys.begin(), randKeys.end() );
      
      {
	// rand ordered find
	for( List::iterator them = randKeys.begin();
	     them != randKeys.end();
	     ++ them )
	  {
	    l = t.find( *them );
	    if( l == 0 )
	      {
		tout << "Finding: " << *them << endl;
		TESTR( "Find failed.", false );
	      }
	    else
	      {
		TESTR( "match error",
		       (*them) == t[ l ] );
	      }
	  }
      }
      
      random_shuffle( randKeys.begin(), randKeys.end() );

      {
	// rand ordered del
	Tree::size_type ts = t.size();
      
	for( List::iterator them = randKeys.begin();
	     them != randKeys.end();
	     ++ them )
	  {
	    -- ts;
	    TEST( t.del( *them ) );
	    TESTR( t.error(), t.good() );
	    TESTR( "size match", t.size() == ts );

	    l = t.find( *them );
	    if( l != 0 )
	      {
		tout << "Finding: " << *them << endl;
		TESTR( "Found deleted.", false );
	      }
	  }

	TESTR( "not empty", t.size() == 0 );
      }
      
      
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
// Revision 2.2  1997/07/19 10:36:14  houghton
// Bug-Fix; added include <algorithm>
//
// Revision 2.1  1997/07/11 17:38:53  houghton
// Initial Version.
//
//
