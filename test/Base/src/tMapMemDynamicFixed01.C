//
// File:        tMapMemDynamicFixed01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapMemDynamicFixed01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/29/97 04:16
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <MapMemDynamicFixed.hh>
#include "TestConfig.hh"
#include "LibTest.hh"
#include <LibLog.hh>

#include <algorithm>
#include <vector>
#include <iomanip>
#include <cstring>

//
// Note: #define MANUAL_REVIEW 1
//
//  to see detailed output after each operation.
//

typedef vector< MapMemDynamicFixed::Loc >   AllocList;

bool
tMapMemDynamicFixed01( LibTest & tester )
{
  
#if defined( MANUAL_REVIEW )
  Log	    l( tester.getDump(), LogLevel::Test );
  
  _LibLog = &l;

  tester.getDump() << endl;
  
  {
    MapMemDynamicFixed::size_type   recSize = 664;
    
    MapMemDynamicFixed	t( TEST_DATA_DIR "/tMMDF01.mdf",
			   recSize,
			   1,
			   02 );

    TESTR( t.error(), t.good() );

    t.dumpInfo( tester.getDump() ) << endl;
    t.dumpNodes( tester.getDump() ) << endl;

    AllocList	    allocList;
    
    MapMemDynamicFixed::Loc n;
    {
      
      for( int a = 0; a < 6; ++ a )
	{
	  TESTR( t.error(), (n = t.allocate()) );
	  memset( t.address( n ), 0xff, recSize );
	  allocList.push_back( n );
	  
	  tester.getDump() << "Alloc: " << n << endl;
	  t.dumpNodes( tester.getDump() ) << endl;
	}
    }
    t.dumpInfo( tester.getDump() ) << endl;
    
    TESTR( t.error(), (n = t.allocate()) );
    memset( t.address( n ), 0xff, recSize );
    allocList.push_back( n );
    
    tester.getDump() << "Alloc: " << n << endl;
    t.dumpNodes( tester.getDump() ) << endl;
    
    t.dumpInfo( tester.getDump() ) << endl;

    {
      for( int a = 0; a < 17; ++ a )
	{
	  TESTR( t.error(), (n = t.allocate()) );
	  memset( t.address( n ), 0xff, recSize );
	  allocList.push_back( n );
	  
	  tester.getDump() << "Alloc: " << n << endl;
	  t.dumpNodes( tester.getDump() ) << endl;
	}
    }
      
    t.dumpNodes( tester.getDump() ) << endl;    
    t.dumpInfo( tester.getDump() ) << endl;

    {
      int n = 0;
      for( AllocList::iterator them = allocList.begin();
	   them != allocList.end();
	   ++ them )
	{
	  tester.getDump() << "Allocated(" << n << "):  "
			   << setw(6) << (*them) << endl;
	  ++ n;
	}
    }

    int rNode;
    
    rNode = 12;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;    
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;
    
    rNode = 10;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;    
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;
    
    rNode = 14;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;    
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;
    
    rNode = 11;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;    
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;
    
    rNode = 13;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;    
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    rNode = 23;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;    
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    for( rNode = 15; rNode < 22; ++ rNode )
      {
	t.release( allocList[ rNode ] );
	tester.getDump() << "Release(" << rNode << "): "
			 << allocList[ rNode ] << endl;
      }
    
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

    rNode = 22;
    t.release( allocList[ rNode ] );
    tester.getDump() << "Release(" << rNode << "): "
		     << allocList[ rNode ] << endl;    
    t.dumpNodes( tester.getDump() ) << endl;
    t.dumpInfo( tester.getDump() ) << endl;

  }

  _LibLog = 0;
#endif
  
  return( true );
}

			   
// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/07/11 17:39:24  houghton
// Initial Version.
//
//
