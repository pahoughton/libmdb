#ifndef _AvlTreeOffset_hh_
#define _AvlTreeOffset_hh_
//
// File:        AvlTreeOffset.hh
// Project:	Mdb
// Desc:        
//
//  This template class provides an Avl Tree that uses offsets
//  (AvlTreeBase::Loc) to link its nodes. 
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     12/14/94 07:21
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

#include <AvlTreeOffsetBase.hh>
#include <MultiMemOffset.hh>
#include <Str.hh>
#include <Compare.hh>
#include <DumpInfo.hh>

template<class T>
class AvlTreeOffset : public AvlTreeOffsetBase
{

public:

  inline AvlTreeOffset( int	(* compareData)( const T & one,
						 const T & two ) = 0,
			T &	(* copyData)( T & dest, const T & src ) = 0,
			MultiMemOffset * memMgr = 0,
			unsigned short	 treeKey = 0,
			bool		 create = false );

  inline Loc		add( const T & rec );
  inline Loc	    	find( const T & key ) const;
  inline T * 	    	findData( const T & key );
  inline bool		del( const T & key );
  
  inline bool 	    	walk( bool (*action)( T & rec ) = 0 );
  inline bool 	    	walk( void * closure,
			      bool (*action)( T & rec, void * closure ) = 0 );
  
  inline bool 	    	walk( void * closure,
			      bool (*action)( const T & rec,
					      void * closure ) ) const;

  inline void 	    	destroy( void (*action)( T & rec ) = 0 );
  inline void 	    	destroy( void * closure,
				 void (*act)( T & rec, void * closure ) = 0 );
			 
  inline void 	    	setWalk( bool (* action)( T & rec ) );
  inline void 	    	setWalk( bool (* action)( T & rec, void * closure ) );
  inline void 	    	setDestroy( void (* act)( T & rec ) );
  inline void      	setDestroy( void (* act)( T & rec, void * closure ) );
  
  inline T *	    	getAddr( Loc dataOffset );
  inline T & 	    	getRef( Loc dataOffset );
  inline T &	    	operator [] ( Loc dataOffset );

  inline const T *	getAddr( Loc dataOffset ) const;
  inline const T & 	getRef( Loc dataOffset ) const;
  inline const T &	operator [] ( Loc dataOffset ) const;

  static size_type    	    getNodeSize( void );

  inline size_type    	    getCount( void ) const;
  
  inline size_type	    size( void ) const;
  
  inline MultiMemOffset *  	getMemMgr( void );
  inline const MultiMemOffset *	getMemMgr( void ) const;

  virtual const char * 	getClassName( void ) const { return "AvlTreeOffset"; };
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;

  virtual ostream &	dumpInfo( ostream &	dest = cerr,
				  const char *	prefix = "    ",
				  bool		showVer = false ) const;

  inline ostream &	dumpTree( ostream & dest );

  inline
  DumpInfo< AvlTreeOffset<T> >	dump( const char *  prefix = "    ",
				      bool	    showVer = true ) const {
    return( DumpInfo< AvlTreeOffset<T> >( *this, prefix, showVer ) );
  };
  
protected:

  struct AvlNode : Node
  {
    T	data;
  };

  struct AvlTree
  {
    size_type   	count;
    Loc   	    	root;
  };

  Addr		getBase( void ) { return( mem->getBase() ); };
  const Addr	getBase( void ) const { return( mem->getBase() ); };

  Loc getMem( size_t sz = sizeof( AvlNode ) ) {
    Loc m = mem->allocate( sz );
    return( m );
  };

  void freeMem( Loc nodeOffset ) {
    mem->release( nodeOffset );
  };

  AvlNode * getNodeAddr( Loc nodeOffset ) {
    return( (AvlNode *) (getBase() + nodeOffset) );
  };

  const AvlNode * getNodeAddr( Loc nodeOffset ) const {
    return( (const AvlNode *) (getBase() + nodeOffset) );
  };

  T & getNodeData( Loc nodeOffset ) {
    return( getNodeAddr( nodeOffset )->data );
  };
  
  const T & getNodeData( Loc nodeOffset ) const {
    return( getNodeAddr( nodeOffset )->data );
  };
  
  Loc getDataOffset( Loc nodeOffset ) const {
    return( nodeOffset + sizeof( Node ) );
  };
  
  AvlTree * getTree( void ) {
    return( (AvlTree *)( getBase() + tree ) );
  };

  const AvlTree * getTree( void ) const {
    return( (const AvlTree *)( getBase() + tree ) );
  };

  virtual int compare( const T & one, const T & two ) const;
  int	(* compareRec)( const T & one, const T & two );

  int compareNode( const Loc one, const Loc two );
  int compareFind( const void * one, const Loc two ) const;

  virtual T & 	copy( T & dest, const T & src );
  T &	(* copyRec)( T & dest, const T & src );
  
  inline bool 	walkNode( Loc root );
  bool	(*walkAction)( T & rec );

  inline bool 	walkNode( Loc root, void * closure );
  bool	(*walkActionClosure)( T & rec, void * closure );

  inline bool 	walkNode( Loc root, void * closure ) const;

  inline void	destroyNode( Loc root );
  void 	(*destroyAction)( T & rec );

  inline void	destroyNode( Loc root, void * closure );
  void 	(*destroyActionClosure)( T & rec , void * closure );
  
private:

  struct    ConstWalkNodeWithClosure
  {
    bool	(*action)( const T & rec, void * closure );
    void *	closure;
  };
  
  AvlTreeOffset( const AvlTreeOffset<T> & copyFrom );
  AvlTreeOffset<T> & operator=( const AvlTreeOffset<T> & assignFrom );

  inline void initTree( int (* cmp)( const T & one, const T & two ),
			T & (* cpy)( T & dest, const T & src) );
	    
  inline void initTree( int (* cmp)( const T & one, const T & two ),
			T & (* cpy)( T & dest, const T & src),
			Loc avlTree );
	    
  enum AvlTreeError
  {
    E_OK,
    E_MEMMGR,
    E_BADTREEKEY,
    E_NOTREE,
    E_NOFUNCT,
    E_UNDEFINED
  };

  AvlTreeError	    avlError;
  Loc	    	    tree;
  MultiMemOffset *  mem;
  
};

//
// Internal use macros
//
// AVLTREE_GLOBALFUNCT - default: not defined
//
// If AVLTREE_GLOBALFUNCT is defined, the compareRec, and copyRec
// function pointers will not be used. Instead, a global assignemt
// operator (operator =) will be used for copies and a global compare()
// function will be used for comparison
//
// Since both the compare, and copy functions for this class
// are virtual, they may be overriden by a sub class.
//
// AVLTREE_FAST - default: not defined
//
// If AVLTREE_FAST is defined, the internal error checking is not
// performed. All values are treated as valid.
// 
#if !defined( AVLTREE_FAST )
#define AVLTREE_SAFE( _v_, _r_ )  if( ! (_v_) ) return( _r_ );
#define AVLTREE_CHECK( _r_ ) if( ! good() ) return( _r_ );
#define AVLTREE_CHECK_NORET() if( ! good() ) return;
#else
#define AVLTREE_SAFE( _v_, _r_ )  _v_;
#define AVLTREE_CHECK( _r_ ) 
#define AVLTREE_CHECK_NORET() 
#endif

#include <AvlTreeOffset.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	AvlTreeOffset	class
//
//  Constructors:
//
//  	AvlTreeOffset( );
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
//  	    Return the name of this class (i.e. AvlTreeOffset )
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
//  	operator <<( ostream & dest, const AvlTreeOffset & src );
//
//	istream &
//	operator >> ( istream & src, AvlTreeOffset & dest );
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
// Copyright:
//
//              This software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1994 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//
// Revision Log:
//
// $Log$
// Revision 2.9  1997/10/01 14:04:03  houghton
// Chaged to reserve 'keys' set.
// Changed to use portable multi platform types.
//
// Revision 2.8  1997/07/13 11:01:10  houghton
// Moved inline definitions to AvlTreeOffset.ii.
// Lots of cleanup.
// Changed constructor.
// Added walk() const.
//
// Revision 2.7  1997/05/15 13:18:29  houghton
// Bug-Fix: reworked constructors and initTree methods to properly
//     initialize 'mem'. Core dumps were occuring when there were memMgr
//     errors and the 'error()' mehtod was called.
//
// Revision 2.6  1997/03/17 14:14:53  houghton
// Changed to verify values during construction to prevent core dumps.
//
// Revision 2.5  1997/03/13 02:22:14  houghton
// Added dumpInfo method.
//
// Revision 2.4  1997/03/07 11:47:05  houghton
// Chagned getCount() to counst.
//
// Revision 2.3  1997/03/03 14:31:36  houghton
// Removed definition of getClassName - it is inlined at the decl.
//
// Revision 2.2  1996/04/27 13:14:43  houghton
// Bug-Fix: inline keyword was missing from AvlTreeOffset<T>::copy()
//
// Revision 2.1  1995/11/10 12:42:01  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:25  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:32  houghton
// New Style Avl an memory management. Many New Classes
//
//
#endif // ! def _AvlTreeOffset_hh_ 
