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
#include <ChunkMgr.hh>
#include <MapFile.hh>

#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class HashTableBase
{

public:

  typedef long		    Hash;
  typedef ChunkMgr::Loc	    Loc;
  
  typedef size_t	    size_type;
  
  HashTableBase( ChunkMgr &	chunkMgr,
		 const char *	indexFileName,
		 ios::open_mode	mode = ios::in,
		 unsigned short	permMask = 0,
		 bool		create = false );

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

  static const Hash  badHash;
  
protected:

  enum ErrorNum
  {
    E_OK,
    E_VERSION,
    E_UNDEFINED
  };
  
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

  inline Header &	header( void );
  inline const Header &	header( void ) const;
  
  inline const Loc &	hashLoc( Hash hash ) const;
  inline Loc &		hashLoc( Hash hash );

  inline const HashNodeBase &	hashNode( Loc node ) const;
  inline HashNodeBase &		hashNode( Loc node );
  
  Loc		insert( Hash hash, Loc node );
  bool		erase( Hash hash, Loc node );

  inline Loc	find( Hash hash ) const;

  inline Hash	first( void ) const;
  inline Loc	next( Hash & hash, Loc & node  ) const;
  inline Loc	prev( Hash & hash, Loc & node ) const;
  inline Hash	endHash( void ) const;

  bool		setError( ErrorNum err );
  
  ChunkMgr &	mgr;
  MapFile *	index;
  ErrorNum	errorNum;
  
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
// Revision 2.1  1997/06/05 11:29:11  houghton
// Initial Version.
//
//
#endif // ! def _HashTableBase_hh_ 

