#ifndef _Hash_hh_
#define _Hash_hh_
//
// File:        Hash.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/02/97 05:13
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
#include <HashTable.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif

template< class Key,
	  class HashFunct,
	  class LessKey >
class Hash
{

public:

  typedef HashTable< Key, Key, ident< Key, Key >, HashFunct, LessKey > Table;
  typedef Table::const_iterator	const_iterator;
  
  inline Hash( ChunkMgr &	chunkMgr,
	       const char *	indexFileName,
	       ios::open_mode	mode = ios::in,
	       unsigned short	permMask = 0,
	       bool		create = false );
  
  virtual ~Hash( void );

  
protected:

private:

  Hash( const Hash & from );
  Hash & operator =( const Hash & from );

};

#if !defined( inline )
#include <Hash.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const Hash & src );

istream &
operator >> ( istream & src, const Hash & dest );


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	Hash	class
//
//  Constructors:
//
//  	Hash( );
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
//  	    Return the name of this class (i.e. Hash )
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
//  	operator <<( ostream & dest, const Hash & src );
//
//	istream &
//	operator >> ( istream & src, Hash & dest );
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
// Revision 2.1  1997/06/05 11:29:10  houghton
// Initial Version.
//
//
#endif // ! def _Hash_hh_ 

