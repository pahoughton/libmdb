//
// File:        Hash.C
// Project:	Mdb
// Desc:        
//
//  Example source code for using the Hash class.
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/03/97 06:21
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <Hash.hh>
#include <MultiMemOffsetMapDynamic.hh>
#include <MapMemDynamicFixed.hh>

struct ExRec
{
  long	key;	    // main part of the key
  short	subKey;	    // sub part of the key
  char	data[16];
};

struct ExHashFunct
{
  HashTableBase::Hash	operator () ( const ExRec & rec ) const {
    return( rec.key );
  };
};

struct ExLessKey
{
  bool	    operator () ( const ExRec & one, const ExRec & two ) const {
    return( one.key == two.key ?
	    one.subKey < two.subKey :
	    one.key < two.key );
  };
};

typedef Hash< ExRec, ExHashFunct, ExLessKey >	ExTable;
    
void
HashExample( void )
{

  MapMemDynamicFixed	mmdf( "./hash.mdf",
			      (ios::open_mode)(ios::in|ios::out),
			      true,
			      ExTable::getNodeSize(),
			      1,
			      02 );

  MultiMemOffsetMapDynamic  mmo( &mmdf, false );
  
  ExTable	table( &mmo,
		       "./hash.hash",
		       (ios::open_mode)(ios::in|ios::out),
		       true,
		       02 );

  if( ! table.good() )
    {
      cerr << table.error() << endl;
      return;
    }

  {
    // insert some records into the table.
    ExRec   r;

    for( long k = 0; k < 10; ++ k )
      {
	r.key = k;
	r.subKey = 0;
	strcpy( r.data, "example" );
	
	if( k == 4 || k == 7 )
	  {
	    for( long sk = 0; sk < 10; sk += 3 )
	      {
		r.subKey = sk;
		if( ! table.insert( r ).second )
		  cerr << "insert failed for: "
		       << r.key << ' ' << r.subKey << endl;
	      }
	  }
	else
	  {
	    if( ! table.insert( r ).second )
	      cerr << "insert failed for: "
		   << r.key << ' ' << r.subKey << endl;
	  }
      }
  }

  {
    // find one.
    ExRec k;

    k.key = 4;
    k.subKey = 3;
    
    Table::iterator it = table.find( k );

    if( it != table.end() )
      {
	cout << "Found: " << (*it).key
	     << " " << (*it).subKey
	     << " " << (*it).data
	     << endl;
      }
    else
      {
	cerr << "Not found: " << k.key << " " << k.subKey << endl;
      }
  }

  {
    // iterate through all recs

    for( Table::iterator them = table.begin();
	 them != table.end();
	 ++ them )
      {
	cout << (*them).k << ' '
	     << (*them).subKey << ' '
	     << (*them).data << endl;
      }
  }
  
  {
    // erase one
    ExRec k;

    k.key = 3;
    k.subKey = 0;
    
    if( ! table.erase( k ) )
      {
	cerr << "erase error: " << k.key << " " << k.subKey << endl;
      }
  }

  {
    // erase all
    if( ! table.erase( table.begin(), table.end() ) )
      {
	cerr << "erase all error: " << table.error() << endl;
      }

  }

}
	    
// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/11 17:36:34  houghton
// Initial Version.
//
//



