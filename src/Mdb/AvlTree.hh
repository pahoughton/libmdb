#ifndef _AvlTree_hh_
#define _AvlTree_hh_
//
// File:        AvlTree.hh
// Desc:        
//              
//  This template will store any data type in a balanced binary AVL tree.
//  It provides all the methods need to access and manage and AVL tree
//  based in memory. There is also an AvlTreeOffset template class
//  that can utilize offsets instead of pointers to managed the tree.
//
//  Quick Start:
//
//  	class YourClass
//  	{
//  	public:
//  	  ...
//  	  static int compare( const YourClass & one, const YourClass & two );
//  	  static void copy( YourClass & one, const YourClass & two );
//  	  ...
//  	};
//
//  	AvlTree<YourClass>  yourTree( YourClass::compare, YourClass::copy );
//
//  	YourClass a, b, c;  // of course these should have real data.
//
//  	yourTree.add( a );
//  	yourTree.add( b );
//  	yourTree.add( c );
//
//  	YourClass key;
//
//  	YourClass * found = yourTree.find( key );
//
// Data Types:
//
//  	AvlTree<T>  	template class;
//  	AvlNode	    	internal tree node.
//  	AvlTree	    	internal tree header.
//
// 
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/15/94 06:20
//
// Revision History:
//
// 
// $Log$
// Revision 2.1  1995/11/10 12:41:58  houghton
// Change to Version 2
//
// Revision 1.1  1995/02/13  16:08:30  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <AvlTreeAddrBase.hh>
#include <MultiMem.hh>
#include <Compare.hh>

#include <Common.h>
#include <sys/types.h>


template<class T>
class AvlTree : AvlTreeBase
{

public:

  // constructor to create a new tree
  AvlTree( int	    	(* compareData)( const T & one, const T & two ) = 0,
	   void	    	(* copyData)( T & dest, const T & src ) = 0,
	   MultiMem *	memMgr = 0 );

  // constructor to manage an existing tree
  AvlTree( void *   	avlTree,
	   int	    	(* compareData)( const T & one, const T & two ) = 0,
	   void	    	(* copyData)( T & dest, const T & src ) = 0,
	   MultiMem *	memMgr = 0 );
  
  T * 	    add( const T & rec );
  T * 	    find( const T & key );
  const T * find( const T & key ) const;
  Bool	    del( const T & rec );

  Bool	    walk( Bool (* action)( T & data ) = 0 );
  Bool	    walk( Bool (* action)( const T & data ) = 0 ) const;
  Bool	    walk( void * closure,
		  Bool (* action)( T & data, void * closure ) = 0);
  Bool	    walk( void * closure,
		  Bool (* action)( const T & data, void * closure ) = 0) const;

  void 	    destroy( void (* action)( T & data ) = 0 );
  void 	    destroy( void * closure,
		     void (* action)( T & data, void * closure ) = 0 );

  void      setWalk( Bool (* action)( T & data ) );
  void	    setWalk( Bool (* action)( const T & data ) );
  void      setWalk( Bool (* action)( T & data, void * closure ) );
  void	    setWalk( Bool (* action)( const T & data, void * closure ) );

  void      setDestroy( Bool (* action)( T & data ) );
  void      setDestroy( Bool (* action)( T & data, void * closure ) );

  ostream & dump( ostream & dest ) const;

  unsigned long	    	getCount( void ) const;
  static size_t	    	getNodeSize( void );
  void *    	    	getTree( void );
  const void *    	getTree( void ) const;
  MultiMem *	    	getMemMgr( void );
  const MultiMem *	getMemMgr( void ) cosnt;

  virtual const char *	getClassName( void ) const { return "AvlTree"; };
  virtual Bool	    	good( void ) const;
  virtual const char *	error( void ) const;
  
protected:

  struct AvlNode : Node 
  {
    T	    data;
  };

  virtual int compare( const T & one, const T & two ) const;
  int	(* compareRec)( const T & one, const T & two );

  int compareNode( const AvlNode * one, const AvlNode * two ) const;
  int compareFind( const void * one, const AvlNode * two ) const;

  virtual void copy( T & dest, const T & src );
  void	(* copyRec)( T & dest, const T & src );
  
  inline Bool walkNode( AvlNode * node );
  Bool (*walkAction)( T & data );
  
  inline Bool walkNode( const AvlNode * node ) const;
  Bool (*walkActionConst)( const T & data );
  
  inline Bool walkNode( AvlNode * node, void * closure );
  Bool (*walkActionClosure)( T & data, void * closure);
  
  inline Bool walkNode( const AvlNode * node, void * closure );
  Bool (*walkActionClosureConst)( T & data, void * closure);
  
  inline void destroyNode( AvlNode * node );
  void (*destroyAction)( T & data );

  inline void destroyNode( AvlNode * node, void * closure );
  void (*destroyActionClosure)( T & data, void * closure);

private:

  inline AvlNode *  getNodeMem( void ) {
    return( (AvlNode *)mem.getMem( sizeof( AvlNode ) ) ); };
  inline void	    freeNodeMem( AvlNode * node ) {
    mem.freeMem( node ); };
  
  inline void initTree( int (* cmp)( const T & one, const T & two ),
			void (* cpy)( T & dest, const T & src),
			MultiMem * memMgr = 0,
			void * avlTree = 0 );
  
  AvlTree( const AvlTree & copyFrom ) {};
  AvlTree & operator=( const AvlTree & assignFrom );

  enum AvlTreeError
  {
    E_OK,
    E_MEM,
    E_NOFUNCT,
    E_UNDEFINED
  };
  
  struct AvlTree
  {
    unsigned long   count;
    struct Node *   root;
  };

  AvlTreeError	avlError;
  AvlTree * 	tree;  
  MultiMem *  	mem;  
  
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
#else
#define AVLTREE_SAFE( _v_, _r_ )  _v_;
#define AVLTREE_CHECK( _r_ ) 
#endif

//
// Inline methods
//

//
// Public Methods
//

// constructor - create a new tree
template<class T>
inline
AvlTree<T>::AvlTree(
  int	    	(* compareData)( const T & one, const T & two ),
  void	    	(* copyData)( T & dest, const T & src ),
  MultiMem * 	memMgr
  )
{
  initTree( compareData, copyData, memMgr );
}

// construcor - manage an existng tree
template<class T>
inline
AvlTree<T>::AvlTree(
  void *    	avlTree,
  int	    	(* compareData)( const T & one, const T & two ),
  void	    	(* copyData)( T & dest, const T & src ),
  MultiMem * 	memMgr
  )
{
  initTree( compareData, copyData, memMgr, avlTree );
}

// add - add a node to the tree
template<class T>
inline
T *
AvlTree<T>::add( const T & rec )
{
  AVLTREE_CHECK( 0 );

  AvlNode * newNode = getNodeMem();
  AvlNode * addNode = newNode;
  
  AVLTREE_SAFE( newNode, 0 );

  initNode( newNode );

  copy( newNode->data, rec );
  
  insertNode( &(tree->root), &addNode );

  if( addNode == 0 )
    {
      tree->count++;
    }
  else
    {
      freeNodeMem( newNode );
    }

  return( (addNode) ? &(addNode->data) : 0 );
}

// find - find a node in the tree
template<class T>
inline
T *
AvlTree<T>::find( const T & rec )
{
  AVLTREE_CHECK( 0 );

  AvlNode key;

  copy( key.data, rec );

  AvlNode * found = findNode( tree->root, &key );

  return( (found) ? &(found->data) : 0 );  
}

// find - find a node in the tree
template<class T>
inline
const T *
AvlTree<T>::find( const T & rec ) const
{
  AVLTREE_CHECK( 0 );
  
  AvlNode * found = findNode( tree->root, &rec );

  return( (found) ? &(found->data) : 0 );  
}

// del - remove a node from the tree
template<class T>
inline
Bool
AvlTree<T>::del( const T & rec )
{
  AVLTREE_CHECK( FALSE );
  
  AvlNode key;
 
  copy( key.data, rec );

  Node * delKey = &key;
  deleteNode( &(tree->root), &delKey );

  if( delKey )
    {
      tree->count--;
      freeNodeMem( delKey );
    }
  return( delKey );
}

// walk - execute a user function for each node the tree
template<class T>
inline
Bool
AvlTree<T>::walk( Bool (*action)( T * data ) )
{
  AVLTREE_CHECK( TRUE );
  
  if( action ) walkAction = action;
  return( walkTree( tree->root ) );
}

// walk - execute a user function for each node the tree
template<class T>
inline
Bool
AvlTree<T>::walk( Bool (*action)( cosnt T * data ) ) const
{
  AVLTREE_CHECK( TRUE );
  
  if( action ) walkActionConst = action;
  return( walkTree( tree->root ) );
}

// walk - execute a user function for each node the tree
template<class T>
inline
Bool
AvlTree<T>::walk(
  void * closure,
  Bool  (*action)( T * data, void * closure )
  )  
{
  AVLTREE_CHECK( TRUE );
  
  if( action ) walkActionClosure = action;
  return( walkTree( tree->root, closure ) );
}

// walk - execute a user function for each node the tree
template<class T>
inline
Bool
AvlTree<T>::walk(
  void * closure ,
  Bool (*action)( cosnt T * data, void * closure )
  ) const
{
  AVLTREE_CHECK( TRUE );
  
  if( action ) walkActionClosureConst = action;
  walkTree( tree->root, closure );
}

// destroy - remove all nodes from the tree
template<class T>
inline
void
AvlTree<T>::destroy( void (*action)( T * data ) )
{
  AVLTREE_CHECK( TRUE );
  
  if( action ) destroyAction = action
  destroyTree( &(tree->root), closure );
}

// destroy - remove all nodes from the tree
template<class T>
inline
void
AvlTree<T>::destroy(
  void * closure,
  void (*action)( T * data, void * closure)
  )
{
  AVLTREE_CHECK( TRUE );
  
  if( action ) destroyActionClosure = action;
  destroyTree( tree->root, closure );
}

// setWalk - set the function to execute for each node during a walk
template<class T>
inline
void
AvlTree<T>::setWalk( Bool (* action)( T & data ) )
{
  walkAction = action;
}

// setWalk - set the function to execute for each node during a walk
template<class T>
inline
void
AvlTree<T>::setWalk( Bool (* action)( const T & data ) )
{
  walkActionConst = action;
}

// setWalk - set the function to execute for each node during a walk
template<class T>
inline
void
AvlTree<T>::setWalk( Bool (* action)( T & data, void * ) )
{
  walkActionClosure = action;
}

// setWalk - set the function to execute for each node during a walk
template<class T>
inline
void
AvlTree<T>::setWalk( Bool (* action)( const T & data, void * ) )
{
  walkActionClosureConst = action;
}

// setDestroy - set the function to execut for each node durring a destroy
template<class T>
inline
void
AvlTree<T>::setDestroy( Bool (* action)( T & data ) )
{
  destroyAction = action;
}

// setDestroy - set the function to execut for each node durring a destroy
template<class T>
inline
void
AvlTree<T>::setDestroy( Bool (* action)( T & data, void * closure ) )
{
  destroyActionClosure = action;
}

// dump - output the tree structure
template<class T>
inline
ostream &
AvlTree<T>::dump( ostream & dest ) const
{
  AVLTREE_CHECK( TRUE );
  
  return( dumpTree( tree->root, dest ) );
}

// getCount - return the number of nodes in the tree
template<class T>
inline
unsigned long
AvlTree<T>::getCount( void ) const
{
  AVLTREE_CHECK( 0 );

  return( tree->count );
}

// getNodeSize - return the size of each node in the tree
template<class T>
inline
size_t
AvlTree<T>::getNodeSize( void  )
{
  return( sizeof( AvlNode ) );
}

// getTree - return a pointer to the head of the tree
template<class T>
inline
void *
AvlTree<T>::getTree( void )
{
  return( tree );
}

// getTree - return a pointer to the head of the tree
template<class T>
inline
cosnt void *
AvlTree<T>::getTree( void ) const
{
  return( tree );
}

// getMemMgr - return a pointer to the memory manager 
template<class T>
inline
const MultiMem *
AvlTree<T>::getMemMgr( void ) cosnt
{
  return( mem );
}

// getMemMgr - return a pointer to the memory manager 
template<class T>
inline
MultiMem *
AvlTree<T>::getMemMgr( void )
{
  return( mem );
}

// good - return TRUE if no errors
template<class T>
inline
Bool
AvlTree<T>::good( void ) const
{
  return( avlError == E_OK && mem.good() );
}

// error - return a description of the current error state
template<class T>
const char *
AvlTree<T>::error( void ) cont
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

// compare - virtual compare T 
template<class T>
inline
int
AvlTree<T>::compare( const T & one, const T & two )
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
  
}

// compareNode - override of base class's virtual compare
template<class T>
inline
int
AvlTree<T>::compareNode( const AvlNode * one, const AvlNode * two )
{
  return( compare( one->data, two->data ) );
}

// compareFind - override of base class's virtual compare
template<class T>
inline
int
AvlTree<T>::compareFind( const void * one, const AvlNode * two )
{
  return( compare( *((T*)data), two->data ) );
}


// copy - copy src to dest
tempate<class T>
void
AvlTree<T>::copy( T & dest, const & src )
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
   if( copyRec ) copyRec( one->data, two->data );
#endif  
}

// walkNode - override of base class's virtual walkNode
template<class T>
inline
Bool
AvlTree<T>::walkNode( AvlNode * node )
{
  return( (walkAction) ? walkAction( node->data ) : TRUE );
}

// walkNode - override of base class's virtual walkNode
template<class T>
inline
Bool
AvlTree<T>::walkNode( const AvlNode * node ) const
{
  return( (walkActionConst) ? walkActionConst( node->data ) : TRUE );
}

// walkNode - override of base class's virtual walkNode
template<class T>
inline
Bool
AvlTree<T>::walkNode( AvlNode * node, void * closure )
{
  return( (walkActionClosure) ? walkActionClosure( node->data, closure ) : TRUE );
}

// walkNode - override of base class's virtual walkNode
template<class T>
inline
Bool
AvlTree<T>::walkNode( const AvlNode * node, void * closure ) const
{
  return( (walkActionClosureConst) ?
	  walkActionClosureConst( node->data, closure ) : TRUE );
}

// destroyNode - overide of base class's virtual destroyNode
template<class T>
inline
void
AvlTree<T>::destroyNode( AvlNode * node )
{
  if( destroyAction )
    {
      destroyAction( node->data );
    }

  freeNodeMem( node );
}

// destroyNode - overide of base class's virtual destroyNode
template<class T>
inline
void
AvlTree<T>::destroyNode( AvlNode * node, void * closure )
{
  if( destroyActionClosure )
    {
      destroyActionClosure( node->data, closure );
    }

  freeNodeMem( node );
}
    
template<class T>
inline
void
AvlTree<T>::initTree(
  int (* cmp)( const T & one, const T & two ),
  void (* cpy)( T & dest, const T & src),
  memMgr = 0,
  avlTree = 0
  )
{
  avlError = E_OK;
  compareRec = cmp;
  copyRec    = cpy;

  walkAction = 0;
  walkActionConst = 0;
  walkActionClosure = 0;
  walkActionClosureConst = 0;

  destroyAction = 0;
  destroyActionClosure = 0;
  
  mem = (memMgr) ? memMgr : &MutliMemMalloc;

  tree = (AvlTree *)((avlTree) ? avlTree : mem->getMem( sizeof( AvlTree ) ));
  
#if !defined( AVLTREE_FAST )
  if( ! tree )
    {
      avlError = E_NOTREE;
    }
  
#if !defined( AVLTREE_GLOBALFUNCT ) 
  if( ! compareRec || ! copyRec )
    {
      avlError = E_NOFUNCT;
      return;
    }
#endif // ! def AVLTREE_GLOBALFUNCT
  
  if( ! mem.good() )
    {
      avlError = E_MEMMGR;
    }
#endif // ! def AVLTREE_FAST
}

#endif // ! def _AvlTree_hh_ 
//
// This software is a modificed version of Brad Appleton's
// 'c' avl library.
//
// It is public domain.
//




