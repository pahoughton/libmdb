#ifndef _AvlTree_hh_
#define _AvlTree_hh_
/**
  File:         AvlTree.hh
  Project:	Mdb (%PP%)
  Item: 	%PI% (%PF%)
  Desc:        
  
              
    This template will store any data type in a balanced binary AVL tree.
    It provides all the methods need to access and manage and AVL tree
    based in memory. There is also an AvlTreeOffset template class
    that can utilize offsets instead of pointers to managed the tree.    
  
  Notes:

  Author:      Paul Houghton - (houghton@cworld.wiltel.com)
  Created:     11/15/94 06:20
  
  Revision History: (See end of file for Revision Log)
  
    Last Mod By:    %PO%
    Last Mod:	    %PRT%
    Version:	    %PIV%
    Status:	    %PS%
  
  %PID%
**/

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

#include <AvlTree.ii>

//
//  Example:
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
//
//
// This software is a modificed version of Brad Appleton's
// 'c' avl library.
//
// It is public domain.
//
// Revision History:
//
// 
// $Log$
// Revision 4.2  2001/07/30 11:15:02  houghton
// *** empty log message ***
//
// Revision 4.1  2001/07/27 00:57:42  houghton
// Change Major Version to 4
//
// Revision 2.1  1995/11/10 12:41:58  houghton
// Change to Version 2
//
// Revision 1.1  1995/02/13  16:08:30  houghton
// New Style Avl an memory management. Many New Classes
//
//
#endif // ! def _AvlTree_hh_ 




