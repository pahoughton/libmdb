//
// File:        tMapMemDynamicDynamic.C
// Project:	Clue
// Desc:        
//
//  Compiled sources for tMapMemDynamicDynamic
//  
// Author:      Paul A. Houghton - (houghton@cshore.wcom.com)
// Created:     03/07/97 08:17
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <MapMemDynamicDynamic.hh>
#include <LibTest.hh>
#include <LibLog.hh>

#include <algorithm>
#include <vector>
#include <iomanip>
#include <cstring>

struct Data
{
  Data( MapMemDynamicDynamic::Loc l = 0, long s = 0) : loc(l), size(s) {};
  MapMemDynamicDynamic::Loc	loc;
  long				size;
};

typedef vector<Data> DataList;

bool
tMapMemDynamicDynamic( LibTest & tester )
{
#if defined( MANUAL_REVIEW )
  Log	    l( tester.getDump(), LogLevel::Test );
  
  _LibLog = &l;

  tester.getDump() << endl;
  
  {
    MapMemDynamicDynamic    t( TEST_DATA_DIR "/mmdd.mdd",
			       40,
			       4096,
			       0 );

    TESTR( t.error(), t.good() );

    Data	d;
    DataList	data;

    d.loc = 0;
    
    d.size  =    3;	data.push_back( d );	// 0
    d.size  =   28;	data.push_back( d );	// 1
    d.size  =   30;	data.push_back( d );	// 2
    d.size  = 1000;	data.push_back( d );	// 3
    d.size  =   30;	data.push_back( d );	// 4
    d.size  =   80;	data.push_back( d );	// 5
    d.size  =   80;	data.push_back( d );	// 6
    d.size  =  160;	data.push_back( d );	// 7
    d.size  =   80;	data.push_back( d );	// 8
    d.size  =   80;	data.push_back( d );	// 9
    d.size  = 1024;	data.push_back( d );	// 10
    d.size  = 1024;	data.push_back( d );	// 11
    d.size  = 1024;	data.push_back( d );	// 12
    d.size  = 1024;	data.push_back( d );	// 13
    d.size  = 1024;	data.push_back( d );	// 14
    d.size  = 1024;	data.push_back( d );	// 15
    d.size  =  146;	data.push_back( d );	// 16
    d.size  =   80;	data.push_back( d );	// 17
    d.size  =   80;	data.push_back( d );    // 18
    d.size  = 4096 * 8;	data.push_back( d );	// 19
    d.size  =   80;	data.push_back( d );	// 20
    d.size  =   80;	data.push_back( d );	// 21
    d.size  = 3720;	data.push_back( d );	// 22

    // allocate()
    for( DataList::iterator them = data.begin();
	 them != data.end();
	 ++ them )
      {
	(*them).loc = t.allocate( (*them).size );
	TESTR( t.error(), (*them).loc != 0 );
      }

    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    int rNode = 12;

    // release 'only'
    tester.getDump() << "Rel only: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'last'
    rNode = 14;
    tester.getDump() << "Rel last: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'join psn'
    rNode = 13;
    tester.getDump() << "Rel join psn: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'first'
    rNode = 5;
    tester.getDump() << "Rel first: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'join ps'
    rNode = 15;
    tester.getDump() << "Rel join ps: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'join sn'
    rNode = 11;
    tester.getDump() << "Rel join sn: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'mid beg'
    rNode = 7;
    tester.getDump() << "Rel mid beg: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );


    // release 'last'
    rNode = 19;
    tester.getDump() << "Rel last: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'mid end'
    rNode = 17;
    tester.getDump() << "Rel mid end: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'last'
    rNode = 20;
    tester.getDump() << "Rel join ps last: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release 'last'
    rNode = 22;
    tester.getDump() << "Rel last: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release join psn last & shrink
    rNode = 21;
    tester.getDump() << "Rel join psn last & shrink: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );


    // allocate last node
    d.size  = 7990;	data.push_back( d );	// 23

    data[ 23 ].loc = t.allocate( data[ 23 ].size );
    TESTR( t.error(), data[ 23 ].loc );
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // expand new
    
    d.size  = 6000;	data.push_back( d );	// 24

    data[ 24 ].loc = t.allocate( data[ 24 ].size );
    TESTR( t.error(), data[ 24 ].loc );
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // alloc find best fit
    d.size  = 124;	data.push_back( d );	// 25

    data[ 25 ].loc = t.allocate( data[ 25 ].size );
    TESTR( t.error(), data[ 25 ].loc );
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    // release first node
    rNode = 0;
    tester.getDump() << "Rel first: data[" << rNode
		     << "] l: " << data[rNode].loc
		     << " s: " << data[rNode].size << endl;

    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );
    
    // alloc first best fit
    d.size  = 20;	data.push_back( d );	// 26

    data[ 26 ].loc = t.allocate( data[ 26 ].size );
    TESTR( t.error(), data[ 26 ].loc );
    
    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    
    for( DataList::iterator them = data.begin();
	 them != data.end();
	 ++ them )
      {
	if( (*them).loc )
	  {
	    tester.getDump() << "Allocated: " << setw(6) << (*them).loc
			     << "  size: " << setw(6) << (*them).size
			     << endl;
	  }
      }
  }
#endif // defined( MANUAL_REVIEW )

  {
    MapMemDynamicDynamic    t( TEST_DATA_DIR "/mmdd.mdd",
			       40,
			       4096,
			       0 );

    TESTR( t.error(), t.good() );

    Data	d;
    DataList	data;

    d.loc = 0;
    
    d.size  =    3;	data.push_back( d );	// 0
    d.size  =   28;	data.push_back( d );	// 1
    d.size  =   30;	data.push_back( d );	// 2
    d.size  = 1000;	data.push_back( d );	// 3
    d.size  =   30;	data.push_back( d );	// 4
    d.size  =   80;	data.push_back( d );	// 5
    d.size  =   80;	data.push_back( d );	// 6
    d.size  =  160;	data.push_back( d );	// 7
    d.size  =   80;	data.push_back( d );	// 8
    d.size  =   80;	data.push_back( d );	// 9
    d.size  = 1024;	data.push_back( d );	// 10
    d.size  = 1024;	data.push_back( d );	// 11
    d.size  = 1024;	data.push_back( d );	// 12
    d.size  = 1024;	data.push_back( d );	// 13
    d.size  = 1024;	data.push_back( d );	// 14
    d.size  = 1024;	data.push_back( d );	// 15
    d.size  =  146;	data.push_back( d );	// 16
    d.size  =   80;	data.push_back( d );	// 17
    d.size  =   80;	data.push_back( d );    // 18
    d.size  = 4096 * 8;	data.push_back( d );	// 19
    d.size  =   80;	data.push_back( d );	// 20
    d.size  =   80;	data.push_back( d );	// 21
    d.size  = 3720;	data.push_back( d );	// 22

    // allocate()
    for( DataList::iterator them = data.begin();
	 them != data.end();
	 ++ them )
      {
	(*them).loc = t.allocate( (*them).size );
	TESTR( t.error(), (*them).loc != 0 );

	char * chunk;

	chunk = (char *)t.address( (*them).loc );
	
	TESTR( t.error(), chunk != 0 );

	memset( chunk, 0xff, (*them).size);
      }

    int rNode = 12;

    // release 'only'
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'last'
    rNode = 14;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'join psn'
    rNode = 13;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'first'
    rNode = 5;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'join ps'
    rNode = 15;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'join sn'
    rNode = 11;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'mid beg'
    rNode = 7;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'last'
    rNode = 19;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'mid end'
    rNode = 17;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'last'
    rNode = 20;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release 'last'
    rNode = 22;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // release join psn last & shrink
    rNode = 21;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // allocate last node
    char *  chunk;
    
    d.size  = 7990;	data.push_back( d );	// 23

    data.back().loc = t.allocate( data.back().size );
    TESTR( t.error(), data.back().loc );
    chunk = (char *)t.address( data.back().loc );
    TESTR( t.error(), chunk != 0 );
    memset( chunk, 0xff, data.back().size );

    // expand new
    
    d.size  = 6000;	data.push_back( d );	// 24

    data.back().loc = t.allocate( data.back().size );
    TESTR( t.error(), data.back().loc );
    chunk = (char *)t.address( data.back().loc );
    TESTR( t.error(), chunk != 0 );
    memset( chunk, 0xff, data.back().size );

    // alloc find best fit
    d.size  = 124;	data.push_back( d );	// 25

    data.back().loc = t.allocate( data.back().size );
    TESTR( t.error(), data.back().loc );
    chunk = (char *)t.address( data.back().loc );
    TESTR( t.error(), chunk != 0 );
    memset( chunk, 0xff, data.back().size );

    // release first node
    rNode = 0;
    t.release( data[rNode].loc );
    data[rNode].loc = 0;
    
    // alloc first best fit
    d.size  = 20;	data.push_back( d );	// 26

    data.back().loc = t.allocate( data.back().size );
    TESTR( t.error(), data.back().loc );
    chunk = (char *)t.address( data.back().loc );
    TESTR( t.error(), chunk != 0 );
    memset( chunk, 0xff, data.back().size );

    t.dumpInfo( tester.getDump() );
    t.dumpNodes( tester.getDump() );

    for( DataList::iterator them = data.begin();
	 them != data.end();
	 ++ them )
      {
	if( (*them).loc )
	  {
	    tester.getDump() << "Allocated: " << setw(6) << (*them).loc
			     << "  size: " << setw(6) << (*them).size
			     << endl;
	  }
      }
  }

#if 0
  vector< int > TestNodeSize;
  vector< int > TestIndex;
  
  {
    for( int i = 0; i < 20 ; i++ )
      {
	TestNodeSize.push_back( 40 ); //50 + (rand() % 20) );
	TestIndex.push_back( i );
      }
      
  }

  NodeList    nodes;
    
  {
    
    MapMemDynamicDynamic	t( "../data/mmdd.map", 16, 4096, 0 );

    cout << t.dump();

    t.dumpFreeList( cout ) << endl;
    t.dumpNodes( cout ) << endl;

    {
      int cnt = 0;
      for( vector<int>::iterator them = TestNodeSize.begin();
	   them != TestNodeSize.end();
	   ++ them )
	{
	  off_t offset = t.getMem( *them );

	  cout << "GETMEM: " <<  cnt++ << setw(4)
	       << (*them) << setw(8) << offset << endl;
	  t.dumpNodes( cout ) << endl;

	  memset( t.getAddr( offset ), 0xff, *them );
	  
	  nodes.push_back( Node( offset, (*them) ) );
	  
	}
    }

    int ni = 0;

    for( ; ni < 5; ++ ni )
	{
	  t.freeMem( nodes[ni].offset );
	  cout << "FREEMEM: " << nodes[ni].offset << endl;
	  t.dumpNodes( cout ) << endl;
	}

    
    t.freeMem( nodes[ni + 3].offset );
    cout << "FREEMEM: " << nodes[ni].offset << endl;
    t.dumpNodes( cout ) << endl;
    
    t.freeMem( nodes[ni + 1].offset );
    cout << "FREEMEM: " << nodes[ni].offset << endl;
    t.dumpNodes( cout ) << endl;
    
    t.freeMem( nodes[ni + 2].offset );
    cout << "FREEMEM: " << nodes[ni].offset << endl;
    t.dumpNodes( cout ) << endl;
    
    t.freeMem( nodes[ni].offset );
    cout << "FREEMEM: " << nodes[ni].offset << endl;
    t.dumpNodes( cout ) << endl;

    ni += 4;
    
    {
      int cnt = 0;
      for( vector<int>::iterator them = TestNodeSize.begin();
	   them != TestNodeSize.end();
	   ++ them )
	{
	  off_t offset = t.getMem( *them );

	  cout << "GETMEM: " << cnt++ << setw(4) << (*them) << setw(8) << offset << endl;
	  t.dumpNodes( cout ) << endl;

	  memset( t.getAddr( offset ), 0xff, *them );
	  
	  nodes.push_back( Node( offset, (*them) ) );
	  
	}
    }
	  
    for( ; ni < 15; ++ ni )
	{
	  t.freeMem( nodes[ni].offset );
	  cout << "FREEMEM: " << nodes[ni].offset << endl;
	  t.dumpNodes( cout ) << endl;
	}

    {
      int cnt = 0;
      for( vector<int>::iterator them = TestNodeSize.begin();
	   them != TestNodeSize.end();
	   ++ them )
	{
	  off_t offset = t.getMem( *them );

	  cout << "GETMEM: " << cnt++ << setw(4) << (*them) << setw(8) << offset << endl;
	  t.dumpNodes( cout ) << endl;

	  memset( t.getAddr( offset ), 0xff, *them );
	  
	  nodes.push_back( Node( offset, (*them) ) );
	  
	}
    }
	  
  }
#if 0

    {
      for( NodeList::iterator them = nodes.begin();
	   them != nodes.end();
	   ++ them )
	{
	  cout << "Alloced Nodes: " << setw(6) << (*them).offset
	       << setw(8) << (*them).size
	       << endl;
	}
    }
    
    random_shuffle( TestIndex.begin(), TestIndex.end() );

    cout << t.dump() << endl;
    t.dumpNodes( cout );
    
    //    int junk;
    //    cin >> junk;
  }

  {
    cout << "\n*** REOPEN ***" << endl;

    //    int junk;
    //    cin >> junk;
    
    MapMemDynamicDynamic	t( "../data/mmdd.map",
				   (ios::open_mode)(ios::in | ios::out) );

    cout << t.dump() << endl;
    t.dumpNodes( cout );

    {
      for( vector<int>::iterator them = TestIndex.begin();
	   them != TestIndex.end();
	   ++ them )
	{
	  cout << "\n *** Freeing: " << (*them)
	       << " offset: " << nodes[ (*them) ].offset
	       << " size: " << nodes[ (*them) ].size
	       << endl;
	  
	  t.freeMem( nodes[ (*them) ].offset );
	  t.dumpNodes( cout );
	}
    }
    
    cout << t.dump() << endl;
    t.dumpFreeList( cout );
  }
#endif
#endif

  _LibLog = 0;
  return( true );
}


// Revision Log:
//
// $Log$
// Revision 2.4  1997/06/27 12:16:22  houghton
// Major rework.
//
// Revision 2.3  1997/03/19 16:23:51  houghton
// Reworked test.
//
// Revision 2.2  1997/03/13 02:43:48  houghton
// Added some tests.
//
// Revision 2.1  1997/03/08 10:30:26  houghton
// Initial Version.
//
//
