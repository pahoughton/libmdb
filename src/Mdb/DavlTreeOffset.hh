#ifndef _DavlTreeOffset_hh_
#define _DavlTreeOffset_hh_
//
// File:        DavlTreeOffset.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     01/10/95 10:38
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
#include <DavlTreeOffsetBase.hh>
#include <MultiMemOffset.hh>
#include <DumpInfo.hh>
#include <ctime>
#include <limits.h>

template<class K, class D>
class DavlTreeOffset : public DavlTreeOffsetBase
{

public:

  inline DavlTreeOffset( int	(* compareKey)( const K & one,
						const K & two ) = 0,
			 K & 	(* keyCopy)( K &    	dest,
					     const K & 	src ) = 0,
			 D & 	(* dataCopy)( D &    	dest,
					      const D & 	src ) = 0,
			 MultiMemOffset *  keyMemMgr = 0,
			 MultiMemOffset *  dataMemMgr = 0,
			 unsigned short	    treeKey = 0,
			 bool		    create = false );

  inline Loc	add( const K & key, time_t when, const D & data );
  inline Loc	find( const K & key, time_t when = LONG_MAX ) const;
  inline D *    findData( const K & key, time_t when = LONG_MAX );
  inline bool	del( const K & key, time_t when );
  
  inline bool  	walk( time_t when = LONG_MAX, 
		      bool (* action)( K & key, time_t  when,D & data ) = 0 );
  inline bool  	walk( void * closure, time_t when = LONG_MAX,		     
		      bool (* action)( K & key, time_t when, D & data,
				       void * closure ) = 0 );

  inline bool  	walkHist(time_t  when = LONG_MAX,
			 bool 	(* action)( K & key, time_t when,
					    D & rec, bool deleted) = 0 );
  inline bool  	walkHist(void * closure, time_t when = LONG_MAX,
			 bool 	(* action)( K & key, time_t  when,
					    D & rec, bool deleted,
					    void *  closure ) = 0 );

  inline bool  	walkHist(const K & key, time_t when = LONG_MAX,
			 bool 	(* action)( K & key, time_t when,
					    D & rec, bool deleted) = 0 );
  inline bool  	walkHist(const K & key, void * closure, time_t when = LONG_MAX,
			 bool 	(* action)( K & key, time_t  when,
					    D & rec, bool deleted,
					    void *  closure ) = 0 );

  inline void	trim( time_t when,
		      void (* action)( K & key, time_t when,
				       D & data, bool deleted ) = 0 );
  inline void	trim( void * closure, time_t when,
		      void (* action)( K & key, time_t when,
				       D & data, bool deleted,
				       void * closure ) = 0 );
  
  inline void  	destroy( void (*action)( K & key, time_t when,
					 D & data, bool deleted ) = 0 );  
  inline void  	destroy( void * closure,
			 void (*action)( K & key, time_t when,
					 D & data, bool deleted,
					 void * closure ) = 0 );
  
  inline void  	setWalk( bool (*action)( K & key, time_t when, D & data ) );
  inline void  	setWalk( bool (*action)( K & key, time_t when, D & data,
					 void * closure ) );
  
  inline void  	setWalkHist( bool (*action)( K & key, time_t when,
					     D & data, bool deleted ) );
  inline void  	setWalkHist( bool (*action)( K & key, time_t when,
					     D & data, bool deleted,
					     void * closure ) );
  
  inline void  	setWalkKeyHist( bool (*action)( K & key, time_t when,
						D & data, bool deleted ) );
  inline void  	setWalkKeyHist( bool (*action)( K & key, time_t when,
						D & data, bool deleted,
						void * closure ) );
  
  inline void  	setTrim( void (*action)( K & key, time_t when,
					 D & data, bool deleted ) );
  inline void  	setTrim( void (*action)( K & key, time_t when,
					 D & data, bool deleted,
					 void * closure ) );
  
  inline void  	setDestroy( void (*action)( K & key, time_t when,
					    D & data, bool deleted ) );
  inline void  	setDestroy( void (*action)( K & key, time_t when,
					    D & data, bool deleted,
					    void * closure ) );
  
  inline K *	getKeyAddr( Loc  keyOffset );
  inline D *	getDataAddr( Loc dataOffset );
  
  inline const K *	getKeyAddr( Loc keyOffset ) const;
  inline const D *	getDataAddr( Loc dataOffset ) const;

  inline K &	    	getKeyRef( Loc  keyOffset );
  inline D &	    	getDataRef( Loc dataOffset );

  inline const K &	getKeyRef( Loc keyOffset ) const;  
  inline const D &	getDataRef( Loc dataOffset ) const;
  
  inline time_t    	getWhen( Loc dataOffset );
  
  inline D &		operator [] ( Loc dataOffset );
  inline const D &	operator [] ( Loc dataOffset ) const;
  
  static size_t		getKeySize( void );
  static size_t		getDataSize( void );
  
  inline size_type	getCount( void ) const;
  inline size_type	getHistCount( void ) const;  
  size_type		size( void ) const;
  
  MultiMemOffset *  	    getKeyMemMgr( void );
  const MultiMemOffset *    getKeyMemMgr( void ) const;
  MultiMemOffset *  	    getDataMemMgr( void );
  const MultiMemOffset *    getDataMemMgr( void ) const;

  virtual const char *	getClassName( void ) const;
  virtual bool	    	good( void ) const;
  virtual const char *	error( void ) const;

  virtual ostream &	dumpInfo( ostream &	dest = cerr,
				  const char *	prefix = "    ",
				  bool		showVer = true ) const;
  
  inline ostream & 	dumpTree( ostream & dest ) const;

  inline
  DumpInfo< DavlTreeOffset<K,D> > dump( const char *  prefix = "    ",
					bool	      showVer = true ) const {
    return( DumpInfo< DavlTreeOffset<K,D> >( *this, prefix, showVer ) );
  };
  
    
protected:

  struct DavlNode : HistNode
  {
    K	key;
  };

  struct DavlHist : HistData
  {
    D	data;
  };

  struct DavlTree
  {
    unsigned long   	count;
    unsigned long   	histCount;
    Loc   	    	root;
  };

  caddr_t	    getBase( void ) {
    return( (caddr_t) keyMem->getBase() );
  };

  const caddr_t	    getBase( void ) const {
    return( (caddr_t) keyMem->getBase() );
  };

  caddr_t	    getBaseData( void ) {
    return( (caddr_t) dataMem->getBase() );
  };

  const caddr_t	    getBaseData( void ) const {
    return( (caddr_t) dataMem->getBase() );
  };
    
  Loc	getKeyMem( size_t sz = sizeof( DavlNode ) ) {
    Loc m = keyMem->allocate( sz );
    return( m );
  };

  void freeKeyMem( Loc keyOffset ) {
    keyMem->release( keyOffset );
  };
  
  Loc getDataMem( size_t sz = sizeof( DavlHist ) ) {
    Loc m = dataMem->allocate( sz );
    return( m );
  };

  void freeDataMem( Loc dataOffset ) {
    dataMem->release( dataOffset );
  };
  
  DavlNode * getNodeAddr( Loc nodeOffset ) {
    return( (DavlNode *) (getBase() + nodeOffset ) );
  };

  const DavlNode *	getNodeAddr( Loc nodeOffset ) const {
    return( (DavlNode *) (getBase() + nodeOffset ) );
  };

  Loc getNodeKeyOffset( Loc nodeOffset ) {
    return( nodeOffset + sizeof( HistNode ) );
  };
  
  K * getNodeKeyAddr( Loc nodeOffset ) {
    return( getKeyAddr( getNodeKeyOffset( nodeOffset ) ) );
  }
  
  K & getNodeKey( Loc nodeOffset ) {
    return( getNodeAddr( nodeOffset )->key );
  };

  const K &	getNodeKey( Loc nodeOffset ) const {
    return( getNodeAddr( nodeOffset )->key );
  };

  DavlHist * getHistAddr( Loc histOffset ) {
    return( (DavlHist *)(getBaseData() + histOffset ) );
  };

  const DavlHist * getHistAddr( Loc histOffset ) const {
    return( (DavlHist *)(getBaseData() + histOffset ) );
  };

  Loc getHistDataOffset( Loc histOffset ) const {
    return( histOffset + sizeof( HistData ) );
  };

  D * getHistDataAddr( Loc histOffset ) {
    return( getDataAddr( getHistDataOffset( histOffset ) ) );
  }
  
  const D * getHistDataAddr( Loc histOffset ) const {
    return( getDataAddr( getHistDataOffset( histOffset ) ) );
  }
  
  D & getHistData( Loc histOffset ) {
    return( getHistAddr( histOffset )->data );
  };

  const D & getHistData( Loc histOffset ) const {
    return( getHistAddr( histOffset )->data );
  };

  time_t & getHistWhen( Loc histOffset ) {
    return( getHistAddr( histOffset )->when );
  };

  time_t    getHistWhen( Loc histOffset ) const {
    return( getHistAddr( histOffset )->when );
  };

  DavlHist * getHistAddrFromDataOffset( Loc dataOffset ) {
    return( getHistAddr( (dataOffset - sizeof( HistData ) ) ) );
  };
  
  DavlTree * getTree( void ) {
    return( (DavlTree *)( getBase() + tree ) );
  };
  
  const DavlTree * getTree( void ) const {
    return( (const DavlTree *)( ((caddr_t)keyMem->getBase()) + tree ) );
  };
  
  virtual int compare( const K & one, const K & two ) const;
  int	(* compareKey)( const K & one, const K & two );

  int compareNode( const Loc one, const Loc two );
  int compareFind( const void * one, const Loc two ) const;
  
  virtual K & 	copyK( K & dest, const K & src );
  virtual D & 	copyD( D & dest, const D & src );
  
  K &	(* copyKey)( K & dest, const K & src );
  D &	(* copyData)( D & dest, const D & src );

  bool	walkHistAction( Loc root, Loc hist );
  bool	(*walkAction)( K & key, time_t when, D & data );

  bool	walkHistAction( Loc root, Loc hist, void * closure ) const;
  
  bool	walkHistAction( Loc root, Loc hist, void * closure );  
  bool	(*walkActionClosure)( K & key, time_t when, D & data, void * closure );

  bool	walkAllHistAction( Loc root, Loc hist );
  bool	(*walkAllAction)( K & key, time_t when, D & data, bool deleted );

  bool	walkAllHistAction( Loc root, Loc hist, void * closure ) const;
  
  bool	walkAllHistAction( Loc root, Loc hist, void * closure );
  bool	(*walkAllActionClosure)( K & key, time_t when, D & data, bool deleted,
				 void * closure );
  
  bool	walkKeyHistAction( Loc root, Loc hist );
  bool	(*walkKeyAction)( K & key, time_t when, D & data, bool deleted );

  bool	walkKeyHistAction( Loc root, Loc hist, void * closure );
  bool	(*walkKeyActionClosure)( K & key, time_t when, D & data, bool deleted,
				 void * closure );

  void	trimHistAction( Loc root, Loc hist );
  void  (*trimAction)( K & key, time_t when, D & data, bool deleted );

  void	trimHistAction( Loc root, Loc hist, void * closure );
  void  (*trimActionClosure)( K & key, time_t when, D & data, bool deleted,
			      void * closure );

  void	destroyHistAction( Loc root, Loc hist  );
  void 	(*destroyAction)( K & key, time_t when, D & data, bool deleted );

  void	destroyHistAction( Loc root, Loc hist, void * closure );
  void 	(*destroyActionClosure)( K & key, time_t when,
				 D & data, bool deleted, void * closure );

private:

  DavlTreeOffset( const DavlTreeOffset<K,D> & copyFrom );
  DavlTreeOffset<K,D> & operator=( const DavlTreeOffset<K,D> & assignFrom );

  struct ConstWalkAllHistClosure
  {
    bool	(*action)( const K & key,
			   time_t when,
			   const D & data,
			   bool deleted,
			   void * closure );
    void *	closure;
  };

  struct ConstWalkHistClosure
  {
    bool	(*action)( const K & key,
			   time_t when,
			   const D & data,
			   void * closure );
    void *	closure;
  };
  
  
  inline void initTree( int (*cmp)( const K & one, const K & two ),
			K & (*cpyKey)( K & dest, const K & src ),
			D & (*cpyData)( D & dest, const D & src ) );

  inline void initTree( int (*cmp)( const K & one, const K & two ),
			K & (*cpyKey)( K & dest, const K & src ),
			D & (*cpyData)( D & dest, const D & src ),
			Loc davlTree );

  enum DavlError
  {
    E_OK,
    E_MEMMGR,
    E_BADTREEKEY,
    E_NOTREE,
    E_NOFUNCT,
    E_UNDEFINED
  };

  DavlError 	    davlError;
  Loc	    	    tree;

  MultiMemOffset *  keyMem;
  MultiMemOffset *  dataMem;
};

//
// Internal use macros
//
// DAVLTREE_GLOBALFUNCT - default: not defined
//
// If DAVLTREE_GLOBALFUNCT is defined, the compareRec, and copyRec
// function pointers will not be used. Instead, a global assignemt
// operator (operator =) will be used for copies and a global Compare()
// function will be used for comparison
//
// Since both the compare, and copy functions for this class
// are virtual, they may be overriden by a sub class.
//
// DAVLTREE_FAST - default: not defined
//
// If DAVLTREE_FAST is defined, the internal error checking is not
// performed. All values are treated as valid.
// 
#if !defined( DAVLTREE_FAST )
#define DAVLTREE_SAFE( _v_, _r_ )    if( ! (_v_) ) return( _r_ );
#define DAVLTREE_CHECK( _r_ ) 	    if( ! good() ) return( _r_ );
#define DAVLTREE_CHECK_NORET() 	    if( ! good() ) return;
#else
#define DAVLTREE_SAFE( _v_, _r_ )  
#define DAVLTREE_CHECK( _r_ ) 
#define DAVLTREE_CHECK_NORET() 
#endif

#include <DavlTreeOffset.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	DavlTreeOffset	class
//
//  Constructors:
//
//  	DavlTreeOffset( );
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
//  	    Return the name of this class (i.e. DavlTreeOffset )
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
//  	operator <<( ostream & dest, const DavlTreeOffset & src );
//
//	istream &
//	operator >> ( istream & src, DavlTreeOffset & dest );
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
// Revision 4.1  2001/07/27 00:57:42  houghton
// Change Major Version to 4
//
// Revision 2.9  1997/10/01 14:04:09  houghton
// Chaged to reserve 'keys' set.
// Changed to use portable multi platform types.
//
// Revision 2.8  1997/09/17 16:55:55  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.7  1997/07/19 10:22:56  houghton
// Bug-Fix: added include <DumpInfo.hh> and <ctime>.
//
// Revision 2.6  1997/07/13 11:08:06  houghton
// Cleanup.
// Moved inline definitions to .ii.
// Change constructor.
//
// Revision 2.5  1997/06/25 12:53:13  houghton
// Changed dump to const.
// Added get*Addr() const and get*Ref() const.
// Added size().
// Removed getDataBase & setDataBase.
//
// Revision 2.4  1997/06/23 12:55:20  houghton
// Cleanup.
//
// Revision 2.3  1997/06/19 13:34:05  houghton
// Changed include StlUtilsConfig to include MdbConfig.
// Cleanup.
//
// Revision 2.2  1997/06/19 12:00:28  houghton
// Changed off_t to Loc
//
// Revision 2.1  1995/11/10 12:42:10  houghton
// Change to Version 2
//
// Revision 1.4  1995/11/05  16:32:27  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:36  houghton
// New Style Avl an memory management. Many New Classes
//
//
#endif // ! def _DavlTreeOffset_hh_ 
