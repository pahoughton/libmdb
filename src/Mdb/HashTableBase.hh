#ifndef _HashTableBase_hh_
#define _HashTableBase_hh_
//
// File:        HashTableBase.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/19/97 04:43
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
#include <MultiMemOffset.hh>
#include <MapFile.hh>

#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class HashTableBase
{

public:

  typedef long		    HashValue;
  typedef MDB_TYPE_LOC	    Loc;
  
  typedef MDB_TYPE_SIZE	    size_type;
  
  HashTableBase( MultiMemOffset *   memMgr,
		 const char *	indexFileName,
		 ios::open_mode	mode = ios::in,
		 bool		create = false,
		 unsigned short	permMask = 02 );

  virtual ~HashTableBase( void );

  inline bool		empty( void ) const;
  inline size_type	size( void ) const;
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;

  static const HashValue  badHash;

  struct Header
  {
    unsigned long   version;
    unsigned long   count;
  };

  struct HashNodeBase
  {
    Loc	    next;
    Loc	    prev;
  };

  class DumpMethods
  {
  public:

    virtual ostream &	dumpNode( ostream & dest,
				  Loc STLUTILS_UNUSED( node ) ) const {
      return( dest );
    };
  };
  
  ostream & dumpTable( ostream & dest, const DumpMethods & meth ) const;
  
protected:

  virtual ostream &	dumpNode( ostream & dest, Loc node ) const;
  
  enum ErrorNum
  {
    E_OK,
    E_VERSION,
    E_UNDEFINED
  };
  
  inline Header &	header( void );
  inline const Header &	header( void ) const;
  
  inline const Loc &	hashLoc( HashValue hash ) const;
  inline Loc &		hashLoc( HashValue hash );

  inline const HashNodeBase &	hashNode( Loc node ) const;
  inline HashNodeBase &		hashNode( Loc node );
  
  Loc		insert( HashValue hash, Loc node );
  bool		erase( HashValue hash, Loc node );
  bool		erase( HashValue firstHash, Loc firstNode,
		       HashValue lastHash, Loc lastNode );
  
  inline Loc	find( HashValue hash ) const;

  inline HashValue	first( void ) const;
  inline Loc	next( HashValue & hash, Loc & node  ) const;
  inline Loc	prev( HashValue & hash, Loc & node ) const;
  inline HashValue	endHash( void ) const;

  bool		setError( ErrorNum err );
  
  MultiMemOffset *  mgr;
  MapFile *	    index;
  ErrorNum	    errorNum;
  
private:

  HashTableBase( const HashTableBase & from );
  HashTableBase & operator =( const HashTableBase & from );

  static const unsigned long hashTableVersion;
    
};

#if !defined( inline )
#include <HashTableBase.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const HashTableBase & src );

istream &
operator >> ( istream & src, const HashTableBase & dest );


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	HashTableBase	class
//
//  Constructors:
//
//  	HashTableBase( );
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
//  	    Return the name of this class (i.e. HashTableBase )
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
//  	operator <<( ostream & dest, const HashTableBase & src );
//
//	istream &
//	operator >> ( istream & src, HashTableBase & dest );
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
// Revision 2.5  1997/09/17 16:55:58  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.4  1997/07/19 10:20:43  houghton
// Port(Sun5): HashTableBase::Hash was renamed to HashValue becuase
//     'Hash' was conflicting with the 'Hash' template class.
//
// Revision 2.3  1997/07/13 11:13:49  houghton
// Changed to use MultiMemOffset.
// Changed dumpTable().
// Added erase( first, last ).
//
// Revision 2.2  1997/06/05 13:42:28  houghton
// Changed for AIX: had to make HashNodeBase a public member.
//
// Revision 2.1  1997/06/05 11:29:11  houghton
// Initial Version.
//
//
#endif // ! def _HashTableBase_hh_ 

