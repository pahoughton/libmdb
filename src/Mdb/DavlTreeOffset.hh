#ifndef _DavlTreeOffset_hh_
#define _DavlTreeOffset_hh_
//
// File:        DavlTreeOffset.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     01/10/95 10:38
//
// Revision History:
//
// 
// $Log$
// Revision 1.3  1995/11/05 16:23:43  houghton
// Added Old Clue classes
//
// Revision 1.1  1995/02/13  16:08:36  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <MultiMemOffset.hh>
#include <DavlTreeOffsetBase.hh>
#include <Str.hh>

#include <limits.h>

template<class K, class D>
class DavlTreeOffset : public DavlTreeOffsetBase
{

public:
  
  DavlTreeOffset( int	(* compareKey)( const K & one,
					const K & two ) = 0,
		  K & 	(* keyCopy)( K &    	dest,
				     const K & 	src ) = 0,
		  D & 	(* dataCopy)( D &    	dest,
				     const D & 	src ) = 0,
		  MultiMemOffset *  keyMemMgr = 0,
		  MultiMemOffset *  dataMemMgr = 0);

  DavlTreeOffset( off_t	    davlTree,
		  int	(* compareKey)( const K & one,
					const K & two ) = 0,
		  K & 	(* keyCopy)( K &    	dest,
				     const K & 	src ) = 0,
		  D & 	(* dataCopy)( D &    	dest,
				     const D & 	src ) = 0,
		  MultiMemOffset *  keyMemMgr = 0,
		  MultiMemOffset *  dataMemMgr = 0 );

  off_t	    	add( const K & key, time_t when, const D & data );
  off_t	    	find( const K & key, time_t when = LONG_MAX );
  D *    	findData( const K & key, time_t when = LONG_MAX );
  Bool	    	del( const K & key, time_t when );
  
  Bool	    	walk( time_t when = LONG_MAX, 
		      Bool (* action)( K & key, time_t  when,D & data ) = 0 );
  Bool	    	walk( void * closure, time_t when = LONG_MAX,		     
		      Bool (* action)( K & key, time_t when, D & data,
				       void * closure ) = 0 );

  Bool	    	walkHist(time_t  when = LONG_MAX,
			 Bool 	(* action)( K & key, time_t when,
					    D & rec, Bool deleted) = 0 );
  Bool	    	walkHist(void * closure, time_t when = LONG_MAX,
			 Bool 	(* action)( K & key, time_t  when,
					    D & rec, Bool deleted,
					    void *  closure ) = 0 );

  Bool	    	walkHist(const K & key, time_t when = LONG_MAX,
			 Bool 	(* action)( K & key, time_t when,
					    D & rec, Bool deleted) = 0 );
  Bool	    	walkHist(const K & key, void * closure, time_t when = LONG_MAX,
			 Bool 	(* action)( K & key, time_t  when,
					    D & rec, Bool deleted,
					    void *  closure ) = 0 );

  void		trim( time_t when,
		      void (* action)( K & key, time_t when,
				       D & data, Bool deleted ) = 0 );
  void		trim( void * closure, time_t when,
		      void (* action)( K & key, time_t when,
				       D & data, Bool deleted,
				       void * closure ) = 0 );
  
  void	    	destroy( void (*action)( K & key, time_t when,
					 D & data, Bool deleted ) = 0 );  
  void	    	destroy( void * closure,
			 void (*action)( K & key, time_t when,
					 D & data, Bool deleted,
					 void * closure ) = 0 );
  
  void 	    	setWalk( Bool (*action)( K & key, time_t when, D & data ) );
  void 	    	setWalk( Bool (*action)( K & key, time_t when, D & data,
					 void * closure ) );
  
  void 	    	setWalkHist( Bool (*action)( K & key, time_t when,
					     D & data, Bool deleted ) );
  void 	    	setWalkHist( Bool (*action)( K & key, time_t when,
					     D & data, Bool deleted,
					     void * closure ) );
  
  void 	    	setWalkKeyHist( Bool (*action)( K & key, time_t when,
						D & data, Bool deleted ) );
  void 	    	setWalkKeyHist( Bool (*action)( K & key, time_t when,
						D & data, Bool deleted,
						void * closure ) );
  
  void 	    	setTrim( void (*action)( K & key, time_t when,
					 D & data, Bool deleted ) );
  void 	    	setTrim( void (*action)( K & key, time_t when,
					 D & data, Bool deleted,
					 void * closure ) );
  
  void 	    	setDestroy( void (*action)( K & key, time_t when,
					    D & data, Bool deleted ) );
  void 	    	setDestroy( void (*action)( K & key, time_t when,
					    D & data, Bool deleted,
					    void * closure ) );
  
  ostream & 	dump( ostream & dest ) const;
  
  inline K *	getKeyAddr( off_t  keyOffset );
  inline D *	getDataAddr( off_t dataOffset );
  K &	    	getKeyRef( off_t  keyOffset );
  D &	    	getDataRef( off_t dataOffset );
  time_t    	getWhen( off_t dataOffset );
  
  static size_t	    getKeySize( void );
  static size_t     getDataSize( void );
  unsigned long	    getCount( void );
  unsigned long	    getHistCount( void );  
  off_t	    	    getTreeOffset( void );  

  MultiMemOffset *  	    getKeyMemMgr( void );
  const MultiMemOffset *    getKeyMemMgr( void ) const;
  MultiMemOffset *  	    getDataMemMgr( void );
  const MultiMemOffset *    getDataMemMgr( void ) const;
  
  virtual const char *	getClassName( void ) const;
  virtual Bool	    	good( void ) const;
  virtual const char *	error( void ) const;
  
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
    off_t   	    	root;
  };

  void setKeyBase( void ) {
    setBase( keyMem->getBase() );
  };

  void setDataBase( void ) {
    setBaseData( dataMem->getBase() );
  }
  
  off_t	getKeyMem( size_t sz = sizeof( DavlNode ) ) {
    off_t m = keyMem->getMem( sz );
    setKeyBase();
    return( m );
  };

  void freeKeyMem( off_t keyOffset ) {
    keyMem->freeMem( keyOffset );
    setKeyBase();
  };
  
  off_t getDataMem( size_t sz = sizeof( DavlHist ) ) {
    off_t m = dataMem->getMem( sz );
    setDataBase();
    return( m );
  };

  void freeDataMem( off_t dataOffset ) {
    dataMem->freeMem( dataOffset );
    setDataBase();
  };
  
  DavlNode * getNodeAddr( off_t nodeOffset ) {
    return( (DavlNode *) (getBase() + nodeOffset ) );
  };

  off_t getNodeKeyOffset( off_t nodeOffset ) {
    return( nodeOffset + sizeof( HistNode ) );
  };
  
  K * getNodeKeyAddr( off_t nodeOffset ) {
    return( getKeyAddr( getNodeKeyOffset( nodeOffset ) ) );
  }
  
  K & getNodeKey( off_t nodeOffset ) {
    return( getNodeAddr( nodeOffset )->key );
  };

  DavlHist * getHistAddr( off_t histOffset ) {
    return( (DavlHist *)(getBaseData() + histOffset ) );
  };

  off_t getHistDataOffset( off_t histOffset ) {
    return( histOffset + sizeof( HistData ) );
  };

  D * getHistDataAddr( off_t histOffset ) {
    return( getDataAddr( getHistDataOffset( histOffset ) ) );
  }
  
  D & getHistData( off_t histOffset ) {
    return( getHistAddr( histOffset )->data );
  };

  time_t & getHistWhen( off_t histOffset ) {
    return( getHistAddr( histOffset )->when );
  };

  DavlHist * getHistAddrFromDataOffset( off_t dataOffset ) {
    return( getHistAddr( (dataOffset - sizeof( HistData ) ) ) );
  };
  
  struct DavlTree * getTree( void ) {
    return( (DavlTree *)( getBase() + tree ) );
  };
  
  const struct DavlTree * getTree( void ) const {
    return( (const DavlTree *)( ((caddr_t)keyMem->getBase()) + tree ) );
  };
  
  virtual int compare( const K & one, const K & two );
  int	(* compareKey)( const K & one, const K & two );

  int compareNode( const off_t one, const off_t two );
  int compareFind( const void * one, const off_t two );
  
  virtual K & 	copy( K & dest, const K & src );
  virtual D & 	copy( D & dest, const D & src );
  
  K &	(* copyKey)( K & dest, const K & src );
  D &	(* copyData)( D & dest, const D & src );

  Bool	walkHistAction( off_t root, off_t hist );
  Bool	(*walkAction)( K & key, time_t when, D & data );

  Bool	walkHistAction( off_t root, off_t hist, void * closure );  
  Bool	(*walkActionClosure)( K & key, time_t when, D & data, void * closure );

  Bool	walkAllHistAction( off_t root, off_t hist );
  Bool	(*walkAllAction)( K & key, time_t when, D & data, Bool deleted );

  Bool	walkAllHistAction( off_t root, off_t hist, void * closure );
  Bool	(*walkAllActionClosure)( K & key, time_t when, D & data, Bool deleted,
				 void * closure );
  
  Bool	walkKeyHistAction( off_t root, off_t hist );
  Bool	(*walkKeyAction)( K & key, time_t when, D & data, Bool deleted );

  Bool	walkKeyHistAction( off_t root, off_t hist, void * closure );
  Bool	(*walkKeyActionClosure)( K & key, time_t when, D & data, Bool deleted,
				 void * closure );

  void	trimHistAction( off_t root, off_t hist );
  void  (*trimAction)( K & key, time_t when, D & data, Bool deleted );

  void	trimHistAction( off_t root, off_t hist, void * closure );
  void  (*trimActionClosure)( K & key, time_t when, D & data, Bool deleted,
			      void * closure );

  void	destroyHistAction( off_t root, off_t hist  );
  void 	(*destroyAction)( K & key, time_t when, D & data, Bool deleted );

  void	destroyHistAction( off_t root, off_t hist, void * closure );
  void 	(*destroyActionClosure)( K & key, time_t when,
				 D & data, Bool deleted, void * closure );
  
private:

  DavlTreeOffset( const DavlTreeOffset<K,D> & copyFrom );
  DavlTreeOffset<K,D> & operator=( const DavlTreeOffset<K,D> & assignFrom );

  inline void initTree( int (*cmp)( const K & one, const K & two ),
			K & (*cpyKey)( K & dest, const K & src ),
			D & (*cpyData)( D & dest, const D & src ),
			MultiMemOffset * keyMemMgr = 0,
			MultiMemOffset * dataMemMgr = 0,
			off_t davlTree = 0 );

  enum DavlError
  {
    E_OK,
    E_MEMMGR,
    E_NOTREE,
    E_NOFUNCT,
    E_UNDEFINED
  };

  DavlError 	    davlError;
  off_t	    	    tree;

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

//
// Inline methods
//

// constructor - create a new tree
template<class K, class D>
inline
DavlTreeOffset<K,D>::DavlTreeOffset(
  int	(* compKey)( const K & one, const K & two ),
  K & 	(* keyCopy)( K & dest, const K & src ),
  D & 	(* dataCopy)( D & dest, const D & src ),
  MultiMemOffset *  keyMemMgr,
  MultiMemOffset *  dataMemMgr
  )
{
  initTree( compKey, keyCopy, dataCopy, keyMemMgr, dataMemMgr );
}

// constructor - manage an existing tree
template<class K, class D>
inline
DavlTreeOffset<K,D>::DavlTreeOffset(
  off_t	avlTree,
  int	(* compKey)( const K & one, const K & two ),
  K & 	(* keyCopy)( K & dest, const K & src ),
  D & 	(* dataCopy)( D & dest, const D & src ),
  MultiMemOffset *  keyMemMgr,
  MultiMemOffset *  dataMemMgr
  )
{
  initTree( compKey, keyCopy, dataCopy, keyMemMgr, dataMemMgr, avlTree );
}

// add - add a node and/or history to the tree
template<class K, class D>
inline
off_t
DavlTreeOffset<K,D>::add( const K & key, time_t when, const D &	data )
{
  DAVLTREE_CHECK( 0 );
  
  off_t	    newNode = getKeyMem();
  off_t	    addNode = newNode;
  DAVLTREE_SAFE( newNode, 0 );
  
  initNode( newNode );

  off_t	    newData = getDataMem();
  DAVLTREE_SAFE( newData, 0 );
  
  initHist( newData );
  
  copy( getNodeKey( newNode ), key );
  copy( getHistData( newData ), data );

  getHistWhen( newData ) = when;
  
  insertNode( &(getTree()->root), &addNode );

  getTree()->histCount++;

  if( addNode == 0 )
    {
      getTree()->count++;
      getNodeAddr( newNode )->hist = newData;
    }
  else
    {
      insertHist( & (getNodeAddr( addNode )->hist), newData );
      freeKeyMem( newNode );
    }
  
  return( 0 );
}

// find - find data ( returns a 'D' offset )
template<class K,class D>
inline
off_t
DavlTreeOffset<K,D>::find( const K & key, time_t when )
{
  DAVLTREE_CHECK( 0 );
  
  off_t	foundNode = findNode( getTree()->root, &key );

  if( foundNode )
    {
      off_t foundHist = findHist( getNodeAddr( foundNode )->hist, when );

      if( foundHist )
	{
	  return( getHistDataOffset( foundHist ) );
	}
    }
  return( 0 );
}

// findData - find data and return a D pointer
template<class K,class D>
inline
D *
DavlTreeOffset<K,D>::findData( const K & key, time_t when )
{
  off_t found = find( key, when );
  return( (found) ? getDataAddr( found ) : 0 );
}

// del - add a delete marker to the tree
template<class K,class D>
inline
Bool
DavlTreeOffset<K,D>::del( const K & key, time_t when )
{
  DAVLTREE_CHECK( FALSE );
  
  off_t	    delData = getDataMem();
  DAVLTREE_SAFE( delData, FALSE );

  initHist( delData );
  getHistWhen( delData ) = when;

  getHistAddr( delData )->deleted = TRUE;
  
  off_t	    delNode = findNode( getTree()->root, &key  );

  if( delNode != 0 )
    {
      getTree()->histCount++;
      insertHist( &(getNodeAddr( delNode )->hist), delData  );
      return( TRUE );
    }
  else
    {
      freeDataMem( delData );
      return( FALSE );
    }
}

// walk - exeucte user function at every node in the tree
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walk(
  time_t when,
  Bool (* action)( K & key, time_t when, D & data ) )
{
  DAVLTREE_CHECK( TRUE );
  
  if( action ) walkAction = action;

  return( walkTree( getTree()->root, when ) );
}

// walk - exeucte user function at every node in the tree
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walk(
  void *    closure,
  time_t    when,
  Bool 	    (* action)( K & key, time_t when, D & data, void * closure )
  )
{
  DAVLTREE_CHECK( TRUE );
  
  if( action ) walkActionClosure = action;

  return( walkTree( getTree()->root, when, closure ) );
}

// walkHist - exeucte user function at every hist in the tree
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkHist(
  time_t    when,
  Bool 	    (* action)( K & key, time_t when, D & data, Bool deleted )
  )
{
  DAVLTREE_CHECK( TRUE );
  
  if( action ) walkAllAction = action;

  return( walkTreeHist( getTree()->root, when ) );
}

// walkHist - exeucte user function at every hist in the tree
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkHist(
  void *    closure,
  time_t    when,
  Bool 	    (* action)( K & key, time_t when,
			D & data, Bool deleted, void * closure )
  )
{
  DAVLTREE_CHECK( TRUE );
  
  if( action ) walkAllActionClosure = action;

  return( walkTreeHist( getTree()->root, when, closure ) );
}

// walkHist - execute user function for each hist rec of the key
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkHist(
  const K & key,
  time_t    when,
  Bool 	    (* action)( K & key, time_t when,
			D & data, Bool deleted )
  )
{
  DAVLTREE_CHECK( TRUE );
  
  off_t node = findNode( getTree()->root, &key );

  if( node )
    {
      if( action ) walkKeyAction = action;

      return( walkKeyHist( node, when ) );
    }
  else
    {
      return( TRUE );
    }
}
    
// walkHist - execute user function for each hist rec of the key
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkHist(
  const K & key,
  void * closure,
  time_t    when,
  Bool 	    (* action)( K & key, time_t when,
			D & data, Bool deleted, void * closure )
  )
{
  DAVLTREE_CHECK( TRUE );
  
  off_t node = findNode( getTree()->root, &key );

  if( node )
    {
      if( action ) walkKeyActionClosure = action;

      return( walkKeyHist( node, when, closure ) );
    }
  else
    {
      return( TRUE );
    }
}
    
  
// trim - remove some history from the tree
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::trim(
  time_t    when,
  void 	    (* action)( K & key, time_t when, D & rec, Bool deleted )
  )
{
  DAVLTREE_CHECK_NORET();
  
  if( action ) trimAction = action;

  trimTree( &(getTree()->root), when );
}

// trim - remove some history from the tree
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::trim(
  void *    closure,
  time_t    when,
  void 	    (* action)( K & key, time_t when,
			D & rec, Bool deleted, void * closure )
  )
{
  DAVLTREE_CHECK_NORET();
  
  if( action ) trimActionClosure = action;

  trimTree( &(getTree()->root), when, closure );
}

// destroy - remove all data from the tree
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::destroy(
  void (* action)( K & key, time_t when, D & data, Bool deleted )
  )
{
  DAVLTREE_CHECK_NORET();
    
  if( action ) destroyAction = action;
  destroyTree( &(getTree()->root) );
}

// destroy - remove all data from the tree
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::destroy(
  void * closure,
  void (* action)( K & key, time_t when,
		   D & data, Bool deleted, void * closure )
  )
{
  DAVLTREE_CHECK_NORET();
    
  if( action ) destroyActionClosure = action;
  destroyTree( &(getTree()->root), closure );
}

// setWalk - specifiy the user function to execute for a walk
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setWalk( Bool (*action)( K & key, time_t when, D & data ) )
{
  walkAction = action;
}

// setWalk - specifiy the user function to execute for a walk
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setWalk(
  Bool (*action)( K & key, time_t when, D & data, void * closure )
  )
{
  walkActionClosure = action;
}

// setWalkHist - specifiy the user function to execute for a walk
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setWalkHist(
  Bool (*action)( K & key, time_t when, D & data, Bool deleted ) )
{
  walkAllAction = action;
}

// setWalkHist - specifiy the user function to execute for a walk
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setWalkHist(
  Bool (*action)( K & key, time_t when, D & data, Bool deleted, void * closure ) )
{
  walkAllActionClosure = action;
}

// setWalkKeyHist - specify the use function to execut for key walk
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setWalkKeyHist(
  Bool (*action)( K & key, time_t when, D & data, Bool deleted )
  )
{
  walkKeyAction = action;
}

// setWalkKeyHist - specify the use function to execut for key walk
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setWalkKeyHist(
  Bool (*action)( K & key, time_t when, D & data, Bool deleted, void * closure )
  )
{
  walkKeyActionClosure = action;
}

// setTrim - specifiy the user function to execute for a trim
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setTrim(
  void (*action)( K & key, time_t when, D & data, Bool deleted ) )
{
  trimAction = action;
}

// setTrim - specifiy the user function to execute for a trim
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setTrim(
  void (*action)( K & key, time_t when, D & data, Bool deleted, void * closure ) )
{
  trimActionClosure = action;
}

// setDestroy - specifiy the user function to execute for a destroy
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setDestroy(
  void (*action)( K & key, time_t when, D & data, Bool deleted ) )
{
  destroyAction = action;
}

// setDestroy - specifiy the user function to execute for a destroy
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::setDestroy(
  void (*action)( K & key, time_t when, D & data, Bool deleted, void * closure ) )
{
  destroyActionClosure = action;
}

// dump - send the tree structure to dest
template<class K, class D>
inline
ostream &
DavlTreeOffset<K,D>::dump( ostream & dest ) const
{
  DAVLTREE_CHECK( dest );
  return( dumpTree( getTree()->root, dest ) );
}

// getKeyAddr - get a K pointer from a key offset
template<class K, class D>
inline
K *
DavlTreeOffset<K,D>::getKeyAddr( off_t keyOffset )
{
  return( (K *)(getBase() + keyOffset ) );
}

// getDataAddr - get a D pointer from a data offset
template<class K, class D>
inline
D *
DavlTreeOffset<K,D>::getDataAddr( off_t dataOffset )
{
  return( (D *)(getBaseData() + dataOffset ) );
}

// getKeyRef - get a K pointer from a key offset
template<class K, class D>
inline
K &
DavlTreeOffset<K,D>::getKeyRef( off_t keyOffset )
{
  return( *getKeyAddr( keyOffset ) );
}

// getDataAddr - get a D pointer from a data offset
template<class K, class D>
inline
D &
DavlTreeOffset<K,D>::getDataRef( off_t dataOffset )
{
  return( *getDataAddr( dataOffset ) );
}

// getWhen - get the hist time for a data offset
template<class K, class D>
inline
time_t
DavlTreeOffset<K,D>::getWhen( off_t dataOffset )
{
  return( getHistAddrFromDataOffset( dataOffset )->when );
}

// getKeySize - return the size of a key node
template<class K, class D>
inline
size_t
DavlTreeOffset<K,D>::getKeySize( void )
{
  return( max( sizeof( DavlNode ), sizeof( DavlTree ) ) );
}

// getDataSize - return the size of a Hist entry
template<class K, class D>
inline
size_t
DavlTreeOffset<K,D>::getDataSize( void )
{
  return( sizeof( DavlHist ) );
}

// getCount - return number of keys in tree
template<class K, class D>
inline
unsigned long
DavlTreeOffset<K,D>::getCount( void )
{
  return( getTree()->count );
}

// getHistCount - return number of hist entries
template<class K, class D>
inline
unsigned long
DavlTreeOffset<K,D>::getHistCount( void )
{
  return( getTree()->histCount );
}

// getTreeOffset - return the tree header offset
template<class K, class D>
inline
off_t
DavlTreeOffset<K,D>::getTreeOffset( void )
{
  return( tree );
}

// getKeyMemMgr - return the key memory manager
template<class K, class D>
inline
MultiMemOffset *
DavlTreeOffset<K,D>::getKeyMemMgr( void )
{
  return( keyMem );
}

// getKeyMemMgr - return the key memory manager
template<class K, class D>
inline
const MultiMemOffset *
DavlTreeOffset<K,D>::getKeyMemMgr( void ) const
{
  return( keyMem );
}

// getDataMemMgr - return the data (hist) memory manager
template<class K, class D>
inline
MultiMemOffset *
DavlTreeOffset<K,D>::getDataMemMgr( void )
{
  return( dataMem );
}

// getDataMemMgr - return the data (hist) memory manager
template<class K, class D>
inline
const MultiMemOffset *
DavlTreeOffset<K,D>::getDataMemMgr( void ) const
{
  return( dataMem );
}


// getClassName - return the class Name
template<class K, class D>
inline
const char *
DavlTreeOffset<K,D>::getClassName( void ) const
{
  return( "DavlTreeOffset<K,D>" );
}

// good - return TRUE if no detected errors, else FALSE
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::good( void ) const
{
  return( tree != 0 && dataMem->good() && keyMem->good() );
}

// error - return a description of the current state
template<class K, class D>
inline
const char *
DavlTreeOffset<K,D>::error( void ) const
{
  static Str errStr;
  errStr.reset();

  errStr << getClassName();
  
  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {
      switch( davlError )
	{
	case E_NOFUNCT:
	  errStr << ": no copy or compare function";
	  break;

	case E_NOTREE:
	  errStr << ": no tree (tree == 0)";
	  break;

	case E_MEMMGR:
	  if( keyMem->good() && dataMem->good() )
	    {
	      errStr << ": unknown mem error";
	    }
	  break;

	default:
	  if( keyMem->good() && dataMem->good() )
	    {
	      errStr << ": unknown error";
	    }
	  break;
	}

      if( ! keyMem->good() )
	{
	  errStr << ": " << keyMem->error();
	}
      
      if( ! dataMem->good() )
	{
	  errStr << ": " << dataMem->error();
	}
      
    }
  return( errStr.cstr() );
}

// compare - virtual key compare function
template<class K, class D>
inline
int
DavlTreeOffset<K,D>::compare( const K & one, const K & two )
{

#ifdef AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
  return( ::Compare( one, two ) );
#else
  return( compareRec( one, two ) );
#endif
#else // ! AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
  return( (compareKey) ? compareKey( one, two) :
	  ::Compare( one, two ) );
#else
  return( (compareKey) ? compareKey( one, two ) : 0 );
#endif
#endif
}

// compareNode - override for base class compareNode function
template<class K, class D>
inline
int
DavlTreeOffset<K,D>::compareNode( const off_t one, const off_t two )
{
  return( compare( getNodeKey( one ), getNodeKey( two ) ) );
}
  
// compareFind - override for base class compareNode function
template<class K, class D>
inline
int
DavlTreeOffset<K,D>::compareFind( const void * one, const off_t two )
{
  return( compare( *((K*)one), getNodeKey( two ) ) );
}
  
//copy - copy key src to dest
template<class K, class D>
K & 
DavlTreeOffset<K,D>::copy( K & dest, const K & src )
{
#ifdef AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
   dest = src;
#else
   copyKey( dest, src );
#endif
#else // ! AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
   if( copyKey )
     {
       copyKey( dest, src );
     }
   else
     {
       dest = src;
     }
#else
   if( copyKey ) copyKey( dest, src );
#endif
#endif
   return( dest );
}

//copy - copy key src to dest
template<class K, class D>
D &
DavlTreeOffset<K,D>::copy( D & dest, const D & src )
{

#ifdef AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
   dest = src;
#else
   copyData( dest, src );
#endif
#else // ! AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
   if( copyData )
     {
       copyData( dest, src );
     }
   else
     {
       dest = src;
     }
#else
   if( copyData ) copyData( dest, src );
#endif
#endif
   return( dest );
}


// walkHistAction - call user function for a node
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkHistAction( off_t root, off_t hist )
{
  Bool abort = TRUE;
  if( walkAction )
    {
      abort = walkAction( getNodeKey( root ),
			  getHistWhen( hist ),
			  getHistData( hist ) );
      setKeyBase();
      setDataBase();
    }
  return( abort );			  
}

// walkHistAction - call user function for a node
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkHistAction( off_t root, off_t hist, void * closure )
{
  Bool abort = TRUE;
  if( walkActionClosure )
    {
      abort = walkActionClosure( getNodeKey( root ),
				 getHistWhen( hist ),
				 getHistData( hist ),
				 closure );
      setKeyBase();
      setDataBase();
    }
  return( abort );			  
}

// walkAllHistAction - call user function for a node
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkAllHistAction( off_t root, off_t hist )
{
  Bool abort = TRUE;
  if( walkAllAction )
    {
      abort = walkAllAction( getNodeKey( root ),
			     getHistWhen( hist ),
			     getHistData( hist ),
			     getHistAddr( hist )->deleted );
      setKeyBase();
      setDataBase();
    }
  return( abort );			  
}

// walkAllHistAction - call user function for a node
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkAllHistAction( off_t root, off_t hist, void * closure )
{
  Bool abort = TRUE;
  if( walkAllActionClosure )
    {
      abort = walkAllActionClosure( getNodeKey( root ),
				    getHistWhen( hist ),
				    getHistData( hist ),
				    getHistAddr( hist )->deleted,
				    closure );
      setKeyBase();
      setDataBase();
    }
  return( abort );			  
}

// walkKeyHistAction - call user function for a node
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkKeyHistAction( off_t root, off_t hist )
{
  Bool abort = TRUE;
  if( walkKeyAction )
    {
      abort = walkKeyAction( getNodeKey( root ),
			     getHistWhen( hist ),
			     getHistData( hist ),
			     getHistAddr( hist )->deleted );
      setKeyBase();
      setDataBase();
    }
  return( abort );			  
}

// walkKeyHistAction - call user function for a node
template<class K, class D>
inline
Bool
DavlTreeOffset<K,D>::walkKeyHistAction( off_t root, off_t hist, void * closure )
{
  Bool abort = TRUE;
  if( walkKeyActionClosure )
    {
      abort = walkKeyActionClosure( getNodeKey( root ),
				    getHistWhen( hist ),
				    getHistData( hist ),
				    getHistAddr( hist )->deleted,
				    closure );
      setKeyBase();
      setDataBase();
    }
  return( abort );			  
}

// trimHistAction - call user function for trim
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::trimHistAction( off_t root, off_t hist )
{
  if( hist )
    {
      if( trimAction )
	{
	  trimAction( getNodeKey( root ), getHistWhen( hist ),
		      getHistData( hist ), getHistAddr( hist )->deleted );
	  setKeyBase();
	  setDataBase();
	}
      freeDataMem( hist );
    }
  else
    {
      freeKeyMem( root );
    }
}
      

// trimHistAction - call user function for trim
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::trimHistAction( off_t root, off_t hist, void * closure )
{
  if( hist )
    {
      if( trimActionClosure )
	{
	  trimActionClosure( getNodeKey( root ), getHistWhen( hist ),
		      getHistData( hist ), getHistAddr( hist )->deleted,
		      closure );
	  setKeyBase();
	  setDataBase();
	}
      freeDataMem( hist );
    }
  else
    {
      freeKeyMem( root );
    }
}
      
// destroyHistAction - call user function for destroy
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::destroyHistAction( off_t root, off_t hist )
{
  if( hist )
    {
      if( destroyAction )
	{
	  destroyAction( getNodeKey( root ), getHistWhen( hist ),
			 getHistData( hist ), getHistAddr( hist )->deleted );
	  setKeyBase();
	  setDataBase();
	}
      freeDataMem( hist );
    }
  else
    {
      freeKeyMem( root );
    }
}
      
// destroyHistAction - call user function for destroy
template<class K, class D>
inline
void
DavlTreeOffset<K,D>::destroyHistAction( off_t root, off_t hist, void * closure )
{
  if( hist )
    {
      if( destroyActionClosure )
	{
	  destroyActionClosure( getNodeKey( root ), getHistWhen( hist ),
				getHistData( hist ), getHistAddr( hist )->deleted,
				closure );
	  setKeyBase();
	  setDataBase();
	}
      freeDataMem( hist );
    }
  else
    {
      freeKeyMem( root );
    }
}
      

template<class K, class D>
inline
void
DavlTreeOffset<K,D>::initTree(
  int (*cmp)( const K & one, const K & two ),
  K & (*cpyKey)( K & dest, const K & src ),
  D & (*cpyData)( D & dest, const D & src ),
  MultiMemOffset * keyMemMgr,
  MultiMemOffset * dataMemMgr,
  off_t davlTree
  )
{
  davlError = E_OK;
  
  compareKey = cmp;
  copyKey = cpyKey;
  copyData = cpyData;

  walkAction = 0;
  walkActionClosure = 0;
  walkAllAction = 0;
  walkAllActionClosure = 0;

  trimAction = 0;
  trimActionClosure = 0;

  destroyAction = 0;
  destroyActionClosure = 0;
 
  keyMem = (keyMemMgr) ? keyMemMgr : &MultiMemOffsetMalloc;
  dataMem = (dataMemMgr ) ? dataMemMgr : &MultiMemOffsetMalloc;

  setKeyBase();
  setDataBase();

  if( ! davlTree )
    {
      tree = getKeyMem( sizeof( DavlTree ) );

      if( tree )
	{
	  getTree()->count = 0;
	  getTree()->histCount = 0;
	  getTree()->root = 0;
	}
      else
	{
	  davlError = E_NOTREE;
	}
    }
  else
    {
      tree = davlTree;
    }

#if !defined( AVLTREE_FAST )
#if !defined( AVLTREE_GLOBALFUNCT )
  if( ! compareKey || ! copyKey || ! copyData )
    {
      davlError = E_NOFUNCT;
      return;
    }
#endif // ! def AVLTREE_GLOBALFUNCT

  if( ! keyMem->good() || ! dataMem->good() )
    {
      davlError = E_MEMMGR;
    }
#endif // ! def AVLTREE_FAST
}      
      
#endif // ! def _DavlTreeOffset_hh_ 
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
