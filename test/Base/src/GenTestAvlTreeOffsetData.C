//
// File:        GenTestAvlTreeOffsetData.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for GenTestAvlTreeOffsetData
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/01/97 06:17
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

#include <fstream>

static long TotalRecs = 10000;

typedef vector< TestAvlTreeOffset::Rec >    List;

bool
GenTestAvlTreeOffsetData( LibTest & tester )
{
  List	    recList;
  
  {
    ofstream f( TEST_DATA_DIR "/TestAvlTreeOffset.data" );

    TEST( f.good() );
    
    TestAvlTreeOffset::Rec  r;
    
    for( int i = 0; i < TotalRecs; ++ i )
      {
	int strLen;

	while( (strLen = (rand() % sizeof( r.k )) ) < 4 ||
	       strLen >= sizeof( r.k ) );
		       
	for( int kc = 0; kc < strLen; ++ kc )
	  {
	    char rc;
	    while( (rc = (rand() % CHAR_MAX)) < ' ' ||
		   rc > '~' );
	    r.k[kc] = rc;
	  }
	r.k[ strLen ] = 0;
	r.l = Host2Net( rand() );

	while( (strLen = (rand() % sizeof( r.d )) ) < 4 ||
	       strLen >= sizeof( r.d ) );
	
	for( int dc = 0; dc < strLen; ++ dc )
	  {
	    char rc;
	    while( (rc = (rand() % CHAR_MAX)) < ' ' ||
		   rc > '~' );
	    r.d[dc] = rc;
	  }
	r.d[ strLen ] = 0;
	recList.push_back( r );

	f.write( (const char *)&r, sizeof( r ) );
	
	// tester.getDump() << "k: '" << r.k << "' l: " << r.l
	//		 << " d: '" << r.d << "'" << endl;
	
      }
  }

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 2.1  1997/07/11 17:38:36  houghton
// Initial Version.
//
//
