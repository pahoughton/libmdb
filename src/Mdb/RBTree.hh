#ifndef _RBTree_hh_
#define _RBTree_hh_
//
// File:        RBTree.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/09/97 04:05
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
#include <RBTreeBase.hh>
#include <iostream>

template< class K, class D >
class RBTree : public RBTreeBase
{

public:

  typedef RBTreeBase::Loc   Loc;
  
  struct RBNode : public RBNodeBase
  {
    K	key;
    D	data;
  };

  inline RBTree( ChunkMgr & mgr, Loc headerLoc = 0 );

  inline ~RBTree( void );

  inline Loc	    insert( K key, D data );
  
  ostream &	dumpTree( ostream & dest ) const;
  
protected:

  inline bool	lessKey( Loc one, Loc two ) const;
  
private:

  RBTree( const RBTree<K,D> & from );
  RBTree<K,D> & operator = ( const RBTree<K,D> & from );

  inline const RBNode *     nodeAddr( Loc loc ) const {
    return( (const RBNode *)mgr.address( loc ) ); };
  
  inline RBNode *	    nodeAddr( Loc loc ) {
    return( (RBNode *)mgr.address( loc ) ); };  
};

#include <RBTree.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	RBTree	class
//
//  Constructors:
//
//  	RBTree( );
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
//  	    Return the name of this class (i.e. RBTree )
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
//  	operator <<( ostream & dest, const RBTree & src );
//
//	istream &
//	operator >> ( istream & src, RBTree & dest );
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
// Revision 2.1  1997/06/05 11:29:13  houghton
// Initial Version.
//
//
#endif // ! def _RBTree_hh_ 

