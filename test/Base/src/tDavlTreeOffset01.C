//
// File:        tDavlTreeOffset01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDavlTreeOffset01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/11/97 07:48
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <DavlTreeOffset.hh>
#include <MapMemDynamicFixed.hh>
#include <MultiMemOffsetMapDynamic.hh>
#include <Compare.hh>
#include <LibTest.hh>

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
tDavlTreeOffset01( LibTest & tester )
{
  ostream & tout = tester.getDump();
  
  {
    typedef DavlTreeOffset< long, long >   Tree;
    typedef vector< long >		   List;

    List			    keys;
    
    MapMemDynamicFixed *	dataMmdf = 0;
    MultiMemOffsetMapDynamic *	dataMmo = 0;
    
    MapMemDynamicFixed *	keyMmdf = 0;
    MultiMemOffsetMapDynamic *	keyMmo = 0;

    dataMmdf = new MapMemDynamicFixed( TEST_DATA_DIR "/tDavlOffset01.rec",
				   Tree::getDataSize(),
				   1,
				   02 );
    TESTR( "no mmdf", dataMmdf != 0 );
    TESTR( dataMmdf->error(), dataMmdf->good() );
    
    dataMmo = new MultiMemOffsetMapDynamic( dataMmdf, false );
    TESTR( "no mmo", dataMmo != 0 );
    TESTR( dataMmo->error(), dataMmo->good() );


    keyMmdf = new MapMemDynamicFixed( TEST_DATA_DIR "/tDavlOffset01.key",
				   Tree::getKeySize(),
				   1,
				   02 );
    TESTR( "no mmdf", keyMmdf != 0 );
    TESTR( keyMmdf->error(), keyMmdf->good() );
    
    keyMmo = new MultiMemOffsetMapDynamic( keyMmdf, false );
    TESTR( "no mmo", keyMmo != 0 );
    TESTR( keyMmo->error(), keyMmo->good() );
    
    Tree    t( compareLong, copyLong, copyLong,
	       keyMmo,
	       dataMmo,
	       0, true );

    TESTR( t.error(), t.good() );

    Tree::Loc	l;

    {
      // ordered insert
      for( int i = 0 ; i < 1000; ++ i )
	{
	  keys.push_back( i );
	  l = t.add( i, 0, i );
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
	  tester.getDump() << "ts: " << ts << " it: " << *them << endl;
	  TEST( t.del( *them, 0 ) );
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
	    l = t.add( *them, 0, *them );
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
	    TEST( t.del( *them, 0 ) );
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
	    l = t.add( *them, 0, *them );
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
	    TEST( t.del( *them, 0 ) );
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
// Revision 2.2  1997/07/14 10:46:03  houghton
// Added debuging output. (THIS TEST IS CURRENTLY NOT WORKING.).
//
// Revision 2.1  1997/07/11 17:39:02  houghton
// Initial Version.
//
//
