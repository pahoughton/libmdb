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
#include <algorithm>
#include <vector>
#include <iomanip>
#include <cstring>

struct Node
{
  Node( off_t o = 0, int s = 0) : offset(o), size(s) {};
  off_t	offset;
  int	size;
};

typedef vector<Node> NodeList;

bool
tMapMemDynamicDynamic( void )
{
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
  return( true );
}


// Revision Log:
//
// $Log$
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
