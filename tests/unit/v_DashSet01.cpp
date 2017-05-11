//
// File:        tDash01.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDash01
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/22/97 08:38
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <DashSet.hh>
#include <MdbUtils.hh>
#include <MapMemDynamicDynamic.hh>
#include <MultiMemOffsetMapDynamic.hh>

#include <LibTest.hh>
#include <vector>
#include <functional>


class LongHash
{
public:
  HashTableBase::HashValue operator () ( const long k ) const {
    return( k );
  };
};

typedef DashSet< long, LongHash, less< long > >   Table;

bool
tDashSet01( LibTest & tester )
{
  {
    MapMemDynamicDynamic    mmdd( TEST_DATA_DIR "/tDashSet01.data",
				  (ios::open_mode)(ios::in|ios::out),
				  true,
				  1,
				  1,
				  02 );

    TESTR( mmdd.error(), mmdd.good() );
    
    MultiMemOffsetMapDynamic	mmo( &mmdd, false );

    TESTR( mmo.error(), mmo.good() );
    
    Table    t( &mmo, TEST_DATA_DIR "/tDash01.dash",
		(ios::open_mode)(ios::in|ios::out),
		true,
		02 );

    TESTR( t.error(), t.good() );

    {
      Table::pair_iterator_bool	ins;
      
      for( long k = 4; k < 20; k += 2 )
	{
	  ins = t.insert( k, 0 );
	  TEST( ins.second );
	  TEST( (*ins.first) == k );
	}
    }

    {
      Table::const_iterator  it;
      
      for( long k = 0; k < 25; ++ k )
	{
	  it = t.find( k, 0 );
	  if( k >= 4 && k < 20 && (k % 2) == 0 )
	    {
	      TEST( it != t.end() );
	      TEST( (*it) == k );
	    }
	  else
	    {
	      TEST( it == t.end() );
	    }
	}
    }

    {
      Table::iterator them = t.begin();
      long	     k = 4;
      for( ; them != t.end(); ++ them )
	{
	  TEST( *them == k );
	  k += 2;
	}
      TEST( them == t.end() );      
      TEST( k == 20 );
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
// Revision 2.2  1997/07/25 13:49:38  houghton
// Changed: Dash was renamed to DashSet.
// Changed: Hash was renamed to HashSet.
//
// Revision 2.1  1997/07/22 19:47:05  houghton
// Initial Version.
//
//
