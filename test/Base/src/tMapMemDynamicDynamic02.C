//
// File:        tMapMemDynamicDynamic02.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapMemDynamicDynamic02
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/27/97 06:34
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <MapMemDynamicDynamic.hh>
#include "TestConfig.hh"
#include <LibTest.hh>

#include <algorithm>
#include <vector>

#include <cstring>

#define MAX_VALUE_SIZE	256

struct Data
{
  Data( MapMemDynamicDynamic::Loc l = 0, long s = 0) : loc(l), size(s) {};
  MapMemDynamicDynamic::Loc	loc;
  long				size;
  unsigned char			value[ MAX_VALUE_SIZE + 1];  
};

typedef vector<Data> DataList;

static const long  TotalChunks = 10000;

typedef	    vector< int >   DataIndex;

bool
tMapMemDynamicDynamic02( LibTest & tester )
{
  DataList	data;
  DataIndex	dataIndex;
  
  {
    Data d;
    d.loc = 0;
    
    for( int i = 0; i < TotalChunks; ++ i )
      {
	dataIndex.push_back( i );
	d.size = (rand() % MAX_VALUE_SIZE);
	for( int c = 0; c < d.size; ++ c )
	  d.value[ c ] = (rand() % UCHAR_MAX);
	data.push_back( d );
      }
  }

  TESTP( true );
    
  {
    MapMemDynamicDynamic    t( TEST_DATA_DIR "/tMMDD02.mdd",
			       1,
			       4096 );

    // allocate all
    for( DataList::iterator them = data.begin();
	 them != data.end();
	 ++ them )
      {
	(*them).loc = t.allocate( (*them).size );
	TESTR( t.error(), (*them).loc );
	memcpy( t.address( (*them).loc ), (*them).value, (*them).size );
      }

    TESTP( true );
    
    // random release all
    {
      DataIndex	    randRelIndex( dataIndex );

      random_shuffle( randRelIndex.begin(), randRelIndex.end() );

      for( DataIndex::iterator them = randRelIndex.begin();
	   them != randRelIndex.end();
	   ++ them )
	{
	  TESTR( "bad value.",
		 memcmp( t.address( data[ (*them) ].loc ),
			 data[ (*them) ].value,
			 data[ (*them) ].size ) == 0 );
	  
	  t.release( data[ (*them) ].loc );
	  data[ (*them) ].loc = 0;
	}
    }
    TESTP( true );
    
    TEST( t.getChunkSize() == 0 );

    // rand allocate & release
    
    {
      DataIndex	randDataIndex( dataIndex );
      
      random_shuffle( randDataIndex.begin(), randDataIndex.end() );

      DataIndex		    allocIndex;
      DataIndex::iterator   randThem = randDataIndex.begin();

      // tester.getDump() << "\n  Rand a/r test." << endl;
      
      for( long testCount = 0; testCount < 15000; ++ testCount )
	{
	  if( (rand() % 2) == 1 )
	    {
	      // alloc one.
	      if( randThem != randDataIndex.end() )
		{
		  allocIndex.push_back( *randThem );

		  TEST( data[ (*randThem) ].loc == 0 );
		  data[ (*randThem) ].loc =
		    t.allocate( data[ (*randThem) ].size );
		  TESTR( t.error(), data[ (*randThem) ].loc );
		  memcpy( t.address( data[ (*randThem) ].loc ),
			  data[ (*randThem) ].value,
			  data[ (*randThem) ].size );
		  ++ randThem;
		  // tester.getDump() << 'a';
		  // tester.getDump().flush();
		}
	      else
		{
		  TESTR( "ran out of index values", false );
		}
	    }
	  else
	    {
	      if( allocIndex.size() )
		{
		  // release one
		  random_shuffle( allocIndex.begin(), allocIndex.end() );
		  
		  TESTR( "not allocated", data[ allocIndex.back() ].loc );
		  
		  TESTR( "bad value.",
			 memcmp( t.address( data[ allocIndex.back() ].loc ),
				 data[ allocIndex.back() ].value,
				 data[ allocIndex.back() ].size ) == 0 );
		  
		  t.release( data[ allocIndex.back() ].loc );
		  TESTR( t.error(), t.good() );
		  data[ allocIndex.back() ].loc = 0;
		  allocIndex.pop_back();
		  // tester.getDump() << 'R';
		  // tester.getDump().flush();
		}
	    }
	}
    }
  }

  return( true );
}
    

  

// Revision Log:
//
// $Log$
// Revision 2.2  1997/07/19 10:40:08  houghton
// Bug-Fix: added include <cstring>
//
// Revision 2.1  1997/07/11 17:39:18  houghton
// Initial Version.
//
//
