#ifndef _AvlTreeOffset_hh_
#define _AvlTreeOffset_hh_
//
// File:        AvlTreeOffset.hh
// Desc:
//
//  This template class provides an Avl Tree that does not use
//  any memory addresses. All links are accomplished with offsets.
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     12/14/94 07:21
//
// Revision History:
//
// 
// $Log$
// Revision 1.2  1995/11/05 16:23:42  houghton
// Added Old Clue classes
//
// Revision 1.1  1995/02/13  16:08:32  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <AvlTreeOffsetBase.hh>
#include <MultiMemOffset.hh>
#include <Str.hh>
#include <Compare.hh>


template<class T>
class AvlTreeOffset : public AvlTreeOffsetBase
{

public:

  AvlTreeOffset( int	(* compareData)( const T & one, const T & two ) = 0,
		 T &	(* copyData)( T & dest, const T & src ) = 0,
		 MultiMemOffset * memMgr = 0 );

  AvlTreeOffset( off_t  avlTree,
		 int	(* compareData)( const T & one, const T & two ) = 0,
		 T &	(* copyData)( T & dest, const T & src ) = 0,
		 MultiMemOffset * memMgr = 0 );

  off_t	    	add( const T & rec );
  off_t	    	find( const T & key );
  T * 	    	findData( const T & key );
  bool		del( const T & key );
  
  bool 	    	walk( bool (*action)( T & rec ) = 0 );
  bool 	    	walk( void * closure,
		      bool (*action)( T & rec, void * closure ) = 0 );

  void 	    	destroy( void (*action)( T & rec ) = 0 );
  void 	    	destroy( void * closure,
			 void (*action)( T & rec, void * closure ) = 0 );
			 
  void 	    	setWalk( bool (* action)( T & rec ) );
  void 	    	setWalk( bool (* action)( T & rec, void * closure ) );
  void 	    	setDestroy( void (* action)( T & rec ) );
  void      	setDestroy( void (* action)( T & rec, void * closure ) );
  
  ostream & 	dump( ostream & dest );

  T *	    	getAddr( off_t dataOffset );
  T & 	    	getRef( off_t dataOffset );
  T &	    	operator []( off_t dataOffset );

  static size_t	    	    getNodeSize( void );
  unsigned long	    	    getCount( void );
  off_t	    	    	    getTreeOffset( void );
  MultiMemOffset *  	    getMemMgr( void );
  const MultiMemOffset *    getMemMgr( void ) const;

  virtual const char * 	getClassName( void ) const { return "AvlTreeOffset"; };
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  
protected:

  struct AvlNode : Node
  {
    T	data;
  };

  struct AvlTree
  {
    unsigned long   	count;
    off_t   	    	root;
  };

  off_t getMem( size_t sz = sizeof( AvlNode ) ) {
    off_t m = mem->getMem( sz );
    setBase( mem->getBase() );
    return( m );
  };

  void freeMem( off_t nodeOffset ) {
    mem->freeMem( nodeOffset );
    setBase( mem->getBase() );
  };

  AvlNode * getNodeAddr( off_t nodeOffset ) {
    return( (AvlNode *) (getBase() + nodeOffset) );
  };

  T & getNodeData( off_t nodeOffset ) {
    return( getNodeAddr( nodeOffset )->data );
  };
  
  off_t getDataOffset( off_t nodeOffset ) {
    return( nodeOffset + sizeof( Node ) );
  };
  
  AvlTree * getTree( void ) {
    return( (AvlTree *)( getBase() + tree ) );
  };

  // since this is a const, setBase can not be called
  const AvlTree * getTree( void ) const {
    return( (const AvlTree *)( ((caddr_t)mem->getBase()) + tree ) );
  };

  virtual int compare( const T & one, const T & two );
  int	(* compareRec)( const T & one, const T & two );

  int compareNode( const off_t one, const off_t two );
  int compareFind( const void * one, const off_t two );

  virtual T & 	copy( T & dest, const T & src );
  T &	(* copyRec)( T & dest, const T & src );
  
  inline bool 	walkNode( off_t root );
  bool	(*walkAction)( T & rec );

  inline bool 	walkNode( off_t root, void * closure );
  bool	(*walkActionClosure)( T & rec, void * closure );

  inline void	destroyNode( off_t root );
  void 	(*destroyAction)( T & rec );

  inline void	destroyNode( off_t root, void * closure );
  void 	(*destroyActionClosure)( T & rec , void * closure );
  
private:

  AvlTreeOffset( const AvlTreeOffset<T> & copyFrom );
  AvlTreeOffset<T> & operator=( const AvlTreeOffset<T> & assignFrom );

  inline void initTree( int (* cmp)( const T & one, const T & two ),
			T & (* cpy)( T & dest, const T & src),
			MultiMemOffset * memMgr = 0,
			off_t avlTree = 0 );
	    
  enum AvlTreeError
  {
    E_OK,
    E_MEMMGR,
    E_NOTREE,
    E_NOFUNCT,
    E_UNDEFINED
  };

  AvlTreeError	    avlError;
  off_t	    	    tree;
  MultiMemOffset *  mem;
  
};

//
// Internal use macros
//
// AVLTREE_GLOBALFUNCT - default: not defined
//
// If AVLTREE_GLOBALFUNCT is defined, the compareRec, and copyRec
// function pointers will not be used. Instead, a global assignemt
// operator (operator =) will be used for copies and a global Compare()
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

//
// Inline methods
//

//
// Public
//

// Constructor - use this constructor to create a new tree
template<class T>
inline
AvlTreeOffset<T>::AvlTreeOffset(
  int	    	    (* compareData)( const T & one, const T & two ),
  T & 	    	    (* copyData)( T & dest, const T & src ),
  MultiMemOffset    * memMgr
  )
{
  initTree( compareData, copyData, memMgr );
}

// Constructor - use this constructor to access an existing tree
template<class T>
inline
AvlTreeOffset<T>::AvlTreeOffset(
  off_t    	    avlTree,
  int	    	    (* compareData)( const T & one, const T & two ),
  T & 	    	    (* copyData)( T & dest, const T & src ),
  MultiMemOffset    * memMgr
  )
{
  initTree( compareData, copyData, memMgr, avlTree );
}


// add - add a new node to the tree
template< class T >
inline
off_t
AvlTreeOffset<T>::add( const T & rec )
{
  AVLTREE_CHECK( 0 );

  off_t	newNode = getMem();
  off_t addNode = newNode;
  
  AVLTREE_SAFE( newNode, 0 );
  
  initNode( newNode );  
  copy( getNodeData( newNode ), rec );
  insertNode(  &(getTree()->root), &addNode );

  if( addNode == 0 )
    {
      getTree()->count++;
    }
  else
    {
      freeMem( newNode );
    }

  return( (addNode) ? getDataOffset( addNode ) : 0 );
}

// find - find a node in the tree
template<class T>
inline
off_t
AvlTreeOffset<T>::find( const T & key )
{
  AVLTREE_CHECK( 0 );

  off_t foundNode = findNode( getTree()->root, &key );

  return( (foundNode) ? getDataOffset( foundNode ) : 0 );
  
}

// find - find a node in the tree
template<class T>
inline
T *
AvlTreeOffset<T>::findData( const T & key )
{
  off_t found = find( key );
  return( (found) ? getAddr( found ) : 0 );
}

// del - remove a node from the tree
template<class T>
inline
bool
AvlTreeOffset<T>::del( const T & key )
{
  //
  // I guess this is the price you pay for not using 
  // memory addresses. We have to copy the key passed
  // into our mem so our compares don't blow up.
  //
  // The problem is that the compare expects to offsets
  // to the same base. The 'key' could be anywhere so,
  // we have to make a local copy.
  //

  AVLTREE_CHECK( 0 );

  off_t	    keyNode = getMem();
  off_t     delNode = keyNode;

  AVLTREE_SAFE( keyNode, 0 );

  copy( getNodeData( keyNode ), key );
  deleteNode( &(getTree()->root), &delNode );

  freeMem( keyNode );
  
  if( delNode  )
    {
      getTree()->count--;
      freeMem( delNode );
    }

  return( delNode );
}

// walk - execute user function for each node in the tree
template<class T>
inline
bool
AvlTreeOffset<T>::walk( bool (*action)( T & rec ) )
{
  AVLTREE_CHECK( 0 );

  setBase( mem->getBase() );

  if( action ) walkAction = action;
  
  return( walkTree( getTree()->root ) );
}

// walk - execute user function for each node in the tree
template<class T>
inline
bool
AvlTreeOffset<T>::walk( void * closure, bool (*action)( T & rec, void * closure ) )
{
  AVLTREE_CHECK( 0 );

  setBase( mem->getBase() );

  if( action ) walkActionClosure = action;
  
  return( walkTree( getTree()->root, closure ) );
}

// destroy - remove all nodes from the tree
template<class T>
inline
void
AvlTreeOffset<T>::destroy( void (* action)( T & rec ) )
{
  AVLTREE_CHECK_NORET();

  if( action ) destroyAction = action;
  destroyTree( &(getTree()->root) );
}

// destroy - remove all nodes from the tree
template<class T>
inline
void
AvlTreeOffset<T>::destroy(
  void * closure,
  void (* action)( T & rec, void * closure )
  )
{
  AVLTREE_CHECK_NORET();

  if( action ) destroyActionClosure = action;
  destroyTree( &(getTree()->root), closure );
}

// dump - show tree structure
template<class T>
inline
ostream &
AvlTreeOffset<T>::dump( ostream & dest )
{
  AVLTREE_CHECK( dest );

  setBase( mem->getBase() );
  
  return( dumpTree( getTree()->root, dest ) );
}

// getAddr - return the address for data at 'dataOffset'
template<class T>
inline
T *
AvlTreeOffset<T>::getAddr( off_t dataOffset )
{
    return( (T *) (getBase() + dataOffset) );
}

// getAddr - return a reference for data at 'dataOffset'
template<class T>
inline
T &
AvlTreeOffset<T>::getRef( off_t dataOffset )
{
    return( *(getAddr( dataOffset )) );
}

// operator [] - return a reference for data at 'dataOffset
template<class T>
inline
T &
AvlTreeOffset<T>::operator []( off_t dataOffset )
{
    return( getRef( dataOffset ) );
}

// setWalk - set the function to execute for each node during a walk
template<class T>
inline
void
AvlTreeOffset<T>::setWalk( bool (* action)( T & rec ) )
{
  walkAction = action;
}

// setWalk - set the function to execute for each node during a walk
template<class T>
inline
void
AvlTreeOffset<T>::setWalk( bool (* action)( T & rec, void * closure ) )
{
  walkActionClosure = action;
}

// setDestroy - set the function to execute for each node during a destroy
template<class T>
inline
void 
AvlTreeOffset<T>::setDestroy( void (* action)( T & rec ) )
{
  destroyAction = action;
}

// setDestroy - set the function to execute for each node during a destroy
template<class T>
inline
void 
AvlTreeOffset<T>::setDestroy( void (* action)( T & rec, void * closure ) )
{
  destroyActionClosure = action;
}

// getNodeSize - return the size of a node
template<class T>
inline
size_t
AvlTreeOffset<T>::getNodeSize( void )
{
  return( sizeof( AvlNode ) );
}

// getCount - return the number of nodes in the tree
template<class T>
inline
unsigned long
AvlTreeOffset<T>::getCount( void )
{
  return( getTree()->count );
}

// getTreeOffset - return the offset to the tree header
template<class T>
inline
off_t
AvlTreeOffset<T>::getTreeOffset( void )
{
  return( tree );
}

// getMemMgr - return the memory manager
template<class T>
inline
MultiMemOffset *
AvlTreeOffset<T>::getMemMgr( void )
{
  return( mem );
}

// getMemMgr - return the memory manager
template<class T>
inline
const MultiMemOffset *
AvlTreeOffset<T>::getMemMgr( void ) const
{
  return( mem );
}

// getClassName - return the name of the class
template<class T>
inline
const char *
AvlTreeOffset<T>::getClassName( void ) const
{
  return( "AvlTreeOffset<T>" );
}

// good - return true if no detected error otherwize false
template<class T>
inline
bool
AvlTreeOffset<T>::good( void ) const
{
  return( tree != 0 && mem->good() );
}

// error - return a string that describes the state of the tree
template<class T>
inline
const char *
AvlTreeOffset<T>::error( void ) const
{
  static Str errStr;
  errStr.reset();

  // use the virtual class name (if defined)
  errStr << getClassName();
  
  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {      
      switch( avlError )
	{
	case E_NOFUNCT:
	  errStr << ": no copy or compare function";
	  break;

	case E_NOTREE:
	  errStr << ": no tree (tree == 0)";
	  break;

	case E_MEMMGR:
	  if( mem->good() )
	    {
	      errStr << ": unknown mem error";
	    }
	  break;

	default:
	  if( mem->good() )
	    {
	      errStr << ": unknown error";
	    }
	  break;
	}

      if( ! mem->good() )
	{
	  errStr << ": " << mem->error();
	}
    }
  return( errStr.cstr() );
}

//
// Protected
//
  
// compare - vitural compare
template<class T>
inline
int
AvlTreeOffset<T>::compare( const T & one, const T & two )
{

#ifdef AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
  return( ::Compare( one, two ) );
#else
  return( compareRec( one, two ) );
#endif
#else // ! AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
  return( (compareRec) ? compareRec( one, two) :
	  ::Compare( one, two ) );
#else
  return( (compareRec) ? compareRec( one, two ) : 0 );
#endif
#endif
}

// compareNode - overrides base class's virtual compare function
template<class T>
inline
int
AvlTreeOffset<T>::compareNode( const off_t one, const off_t two )
{
  return( compare( getNodeData( one ), getNodeData( two ) ) );
}

// compareFind - overrides base class's virtual compare function
template<class T>
inline
int
AvlTreeOffset<T>::compareFind( const void * one, const off_t two )
{
  return( compare( *((T*)one), getNodeData( two ) ) );
}

// copy - copy data 
template<class T>
T &
AvlTreeOffset<T>::copy( T & dest, const T & src )
{
#ifdef AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
   dest = src;
#else
   copyRec( dest, src );
#endif
#else // ! AVLTREE_FAST
#ifdef AVLTREE_GLOBALFUNCT
   if( copyRec )
     {
       copyRec( dest, src );
     }
   else
     {
       dest = src;
     }
#else
   if( copyRec ) copyRec( dest, src );
#endif
#endif
   return( dest );
}

// walkNode - execute user function at each node
template<class T>
inline
bool
AvlTreeOffset<T>::walkNode( off_t root )
{
  bool abort = true;
  if( walkAction )
    {
      abort = walkAction( getNodeData( root ) );
      setBase( mem->getBase() );
    }
  
  return( abort );
}

// walkNode - execute user function at each node
template<class T>
inline
bool
AvlTreeOffset<T>::walkNode( off_t root, void * closure )
{
  bool abort = true;
  if( walkActionClosure )
    {
      abort = walkActionClosure( getNodeData( root ), closure );
      setBase( mem->getBase() );
      return( abort );
    }
  
  return( abort );
}

// destroyNode - excute use function for each node during a destroy
template<class T>
inline
void
AvlTreeOffset<T>::destroyNode( off_t root )
{
  if( destroyAction )
    {
      destroyAction( getNodeData( root ) );
    }

  freeMem( root );

  setBase( mem->getBase() );
}

// destroyNode - excute use function for each node during a destroy
template<class T>
inline
void
AvlTreeOffset<T>::destroyNode( off_t root, void * closure )
{
  if( destroyActionClosure )
    {
      destroyActionClosure( getNodeData( root ), closure );
    }

  freeMem( root );

  setBase( mem->getBase() );
}


template<class T>
inline
void
AvlTreeOffset<T>::initTree(
  int (* cmp)( const T & one, const T & two ),
  T & (* cpy)( T & dest, const T & src),
  MultiMemOffset * memMgr,
  off_t avlTree
  )
{
  avlError = E_OK;
  compareRec = cmp;
  copyRec = cpy;

  walkAction = 0;
  walkActionClosure = 0;
  destroyAction = 0;
  destroyActionClosure = 0;

  mem = (memMgr) ? memMgr : &MultiMemOffsetMalloc;

  setBase( mem->getBase() );
  
  if( ! avlTree )
    {
      tree = getMem( sizeof( AvlTree ) );
      if( tree )
	{
	  getTree()->root = 0;
	  getTree()->count = 0;
	}
      else
	{
	  avlError = E_NOTREE;	  
	}
    }
  else
    {
      tree = avlTree;
    }
  
#if !defined( AVLTREE_FAST )
#if !defined( AVLTREE_GLOBALFUNCT )
  if( ! compareRec || ! copyRec )
    {
      avlError = E_NOFUNCT;
      return;
    }
#endif // ! def AVLTREE_GLOBALFUNCT

  if( ! mem->good() )
    {
      avlError = E_MEMMGR;
    }
#endif // ! def AVLTREE_FAST
    
}
    
#endif // ! def _AvlTreeOffset_hh_ 
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
