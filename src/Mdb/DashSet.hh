#ifndef _Dash_hh_
#define _Dash_hh_
//
// File:        Dash.hh
// Project:	Mdb
// Desc:        
//
//	A 'Dash' is a hash table for a data type that includes an
//      effecive date for each key value.
//
// Quick Start: - short example of class usage
//
//	struct Data
//	{
//	  long	    id;
//	  long      value;
//	};
//
//	class DataHashFunct
//	{
//	public:
//
//	  HashTableBase::Hash	operator()( const Data & data ) const {
//	    return( data.id % 1000 );
//	  };
//	};
//
//	class LessData
//	{
//	public:
//
//	  bool	operator()( const Data & one, const Data & two ) const {
//	    return( one.id < two.id );
//	  };
//	};
//
//	typedef Dash< Data, DataHashFunct, DataLess >    DataDash;
//
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/02/97 09:42
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <MdbConfig.hh>
#include <DashTable.hh>
#include <projectn.h>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


template< class Key,
	  class HashFunct,
	  class LessKey >
class Dash
{

public:

  typedef DashTable< Key, Key, ident< Key, Key >, HashFunct, LessKey > Table;

  typedef Table::size_type		size_type;
  typedef Table::iterator		iterator;
  typedef Table::const_iterator		const_iterator;
  // typedef Table::reverse_iterator	reverse_iterator;
  // typedef Table::const_reverse_iterator	const_reverse_iterator;
  typedef Table::pair_iterator_bool	pair_iterator_bool;

  typedef Table::EffDate		EffDate;
  
  inline Dash( ChunkMgr &	chunkMgr,
	       const char *	indexFileName,
	       ios::open_mode	mode = ios::in,
	       unsigned short	permMask = 0,
	       bool		create = false );

  virtual ~Dash( void ) {};

  inline pair_iterator_bool	insert( const Key & key, EffDate effDate ) {
    return( table.insert( key, effDate, key ) );
  };
  
  inline bool			erase( const Key & key, EffDate effDate ) {
    return( table.erase( key, effDate ) );
  };


  inline bool			erase( const iterator & pos ) {
    return( table.erase( pos ) );
  };

#if defined( FIXME )
  inline bool			erase( const iterator & first,
				       const iterator & last ) {
    return( table.erase( first, last ) );
  };
#endif
  
  inline iterator		find( const Key & key, EffDate when ) {
    return( table.find( key, when ) );
  };

  inline const_iterator		find( const Key & key, EffDate when ) const {
    return( table.find( key, when ) );
  };
  
  inline iterator	    begin( void ) { return( table.begin() ); };
  inline iterator	    end( void ) { return( table.end() ); };
  
  inline const_iterator	    begin( void ) const { return( table.begin() ); };
  inline const_iterator	    end( void ) const { return( table.end() ); };

  // inline reverse_iterator	rbegin( void ) { return( table.rbegin() ); };
  // inline reverse_iterator	rend( void ) { return( table.rend() ); };

  // inline const_reverse_iterator	rbegin( void ) const {return(table.rbegin());};
  // inline const_reverse_iterator	rend( void ) const {return(table.rend());};

  inline size_type	    size( void ) const { return( table.size() ); };
  inline bool		    empty( void ) const { return( table.empty() ); };
  
protected:

private:

  // DashTable will prevent these
  //
  // Dash( const Dash & from );
  // Dash & operator =( const Dash & from );

  Table	    table;
};


#include <Dash.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	Dash	class
//
//  Constructors:
//
//  	Dash( );
//
//  Destructors:
//
//  Public Interface:
//
//	virtual ostream &
//	write( ostream & dest ) const;
//	    write the data for this class in binary form to the ostream.
//
//	virtual istream &
//	read( istream & src );
//	    read the data in binary form from the istream. It is
//	    assumed it stream is correctly posistioned and the data
//	    was written to the istream with 'write( ostream & )'
//
//	virtual ostream &
//	toStream( ostream & dest ) const;
//	    output class as a string to dest (used by operator <<)
//
//	virtual istream &
//	fromStream( istream & src );
//	    Set this class be reading a string representation from
//	    src. Returns src.
//
//  	virtual Bool
//  	good( void ) const;
//  	    Return true if there are no detected errors associated
//  	    with this class, otherwise false.
//
//  	virtual const char *
//  	error( void ) const;
//  	    Return a string description of the state of the class.
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. Dash )
//
//  	virtual const char *
//  	getVersion( bool withPrjVer = true ) const;
//  	    Return the version string of this class.
//
//	virtual ostream &
//	dumpInfo( ostream & dest, const char * prefix, bool showVer );
//	    output detail info to dest. Includes instance variable
//	    values, state info & version info.
//
//	static const ClassVersion version
//	    Class and project version information. (see ClassVersion.hh)
//
//  Protected Interface:
//
//  Private Methods:
//
//  Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const Dash & src );
//
//	istream &
//	operator >> ( istream & src, Dash & dest );
//
// Example:
//
// See Also:
//
// Files:
//
// Documented Ver:
//
// Tested Ver:
//
// Revision Log:
//
// $Log$
// Revision 2.2  1997/06/05 13:39:39  houghton
// Commented out copy constructor and operator = private delarations.
//
// Revision 2.1  1997/06/05 11:29:08  houghton
// Initial Version.
//
//
#endif // ! def _Dash_hh_ 
