//
// File:        tMapMemDlist.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapMemDlist
//  
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     08/19/98 10:11
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <MapMemDlist.hh>
#include <MapMemDynamicDynamic.hh>
#include <MultiMemOffsetMapDynamic.hh>

#include <LibTest.hh>

typedef MapMemDlist< long > DList;

bool
tMapMemDlist( LibTest & tester )
{
  {
    const char * TestFn = TEST_DATA_DIR "/mdl01.mdl";
    
    MapMemDynamicDynamic    dataMap( TestFn,
				     (ios::open_mode)(ios::in|ios::out),
				     true,
				     1,
				     1,
				     02 );
    
    TESTR( dataMap.error(), dataMap.good() );

    MultiMemOffsetMapDynamic   dataMgr( &dataMap, false );

    TESTR( dataMgr.error(), dataMgr.good() );

    DList   t( &dataMgr );

    long    val;

    DList::Loc	first( 0 );
    DList::Loc	last( 0 );

    cout << '\n';
    
    for( val = 0; val < 10; ++ val )
      {
	cout << "inserting " << val << endl;
	t.insert( first, last, t.end(), val );
      }

    for( DList::iterator them = t.begin( first );
	 them != t.end();
	 ++ them )
      {
	cout << *them << endl;
      }

    while( t.begin( first ) != t.end() )
      {
	cout << "Erasing " << *(t.begin( first )) << endl;
	
	t.erase( first, last, t.begin( first ) );

	for( DList::iterator ethem = t.begin( first );
	     ethem != t.end();
	     ++ ethem )
	  {
	    cout << *ethem << endl;
	  }
      }
  }

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.2  2000/05/27 14:31:10  houghton
// Bug-Fix: was missing '/' in file name.
//
// Revision 2.1  1998/10/23 13:21:00  houghton
// Initial Version.
//
//
