#ifndef _AvlTreeOffsetBase_hh_
#define _AvlTreeOffsetBase_hh_
//
// File:        AvlTreeOffsetBase.hh
// Desc:        
//              
//  NO PUBLIC INTERFACE
//
//  This class provides the methods for managing Avl Tress using
//  offsets instead of direct memory addresses. It can be used in
//  a memory map that does not always map to the same address. The
//  AvlTreeAddrBase class should be used if you are using fixed address
//  maps or heap (malloc) memory.
//
//  This base class should only be referenced from the AvlTreeOffset
//  template class. It has protected constructors so it can not be
//  directly instantiated.
//
//  DataTypes:
//
//  	AvlTreeOffsetBase    	class
//  	AvlTreeOffsetBase::Node	a single avl tree node.
//
//  Protected Constructors:
//
//  	AvlTreeOffsetBase( void );
//  	    This constructor does not do anything it is only declared
//  	    to ensure the class can not be instanciated.
//
//  	AvlTreeOffsetBase( const AvlTreeOffsetBase & copyFrom );
//  	    Since this class has no data, the copy constructor doesn't
//  	    need to do anything. It defined to prevent
//  	    direct instanciation.
//
//  Protected Interface:
//
//  	short
//  	insertNode( off_t * root, off_t * newNode );
//  	    Insert a new node into the tree. If 'newNode' is a
//  	    duplicate node (see compare), 'newNode' will contain
//  	    the existing node on return.
//  	    Returns the change in tree balance cause by inserting
//  	    the new node.
//
//  	off_t
//  	findNode( off_t root, off_t key );
//  	    Search the tree for a node that matches key.
//  	    Returns the address of the found node or 0 if not found.
//
//  	short
//  	deleteNode( off_t * root, off_t * key, int minMax = 0 );
//  	    Delete the node from the tree that matches 'key'. If 'minMax'
//  	    is > 0, the value of 'key' is ignored and the largest
//  	    (right most) node is deleted. If 'minMax' is < 0, the value
//  	    of 'key' is ignored and the smalles (left most) key is
//  	    deleted. 'key' is set to the the deleted node or 0 if
//  	    no match is found.
//  	    Returns the change in tree balance caused by deleting the node.
//
//  	bool
//  	walkTree( off_t root );
//  	    Walk the entire tree in accending order (left to right) and
//  	    execute a user function at each node.
//  	    Returns FALSE if the entire tree was walked. TRUE
//  	    will be returned if the walk was aborted buy 'walkNode'
//
//  	bool
//  	walkTree( off_t root, void * closure );
//  	    Walk the entire tree in accending order (left to right) and
//  	    execute a user function at each node. 'closure' is passed
//  	    to the user function.
//  	    Returns FALSE if the entire tree was walked. TRUE
//  	    will be returned if the walk was aborted buy 'walkNode'
//
//  	void
//  	initNode( off_t node );
//  	    Initialize a new node. Sets all values to 0.
//
//  	ostream &
//  	dumpTree( const off_t root, ostream & dest, int level = 0 ) const;
//  	    This method will output a 'graph' of the tree structure.
//  	    The graph show each node's position and balance in the tree.
//  	    Returns the ostream passed as 'dest'
//
//  	void
//  	destroyTree( off_t * root );
//  	    Remove all nodes from the tree. A user function is call
//  	    at each node so the data tied to the node can be deleted.
//
//  	void
//  	destroyTree( off_t * root, void * closure );
//  	    Remove all nodes from the tree. A user function is call
//  	    at each node so the data tied to the node can be deleted.
//  	    'closure' is passed to the user function.
//
//  	virutal
//  	int
//  	compare( const off_t one, const off_t two ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    It is used to determine difference between the two nodes
//  	    passed to it.
//  	    Returns > 0 if node 'one' is greater than node 'two', = 0 if
//  	    node 'one' and 'two' are equal and < 0 if node 'one' is less
//  	    than node two.
//
//  	vitual
//  	void
//  	destroyNode( off_t node ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    It is called for each node from within 'destroyTree'. It should
//  	    release all memory used by the node including the 'node' it
//  	    self.
//  	    
//  	vitual
//  	void
//  	destroyNode( off_t node, void * closure ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    It is called for each node from within 'destroyTree'. It should
//  	    release all memory used by the node including the 'node' it
//  	    self.'closure' is the value passed as closure to 'dumpTree'.
//
//  	vitual
//  	bool
//  	walkNode( off_t node ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    'walkNode' is called by 'walkTree' for each node in the tree.
//  	    Return FALSE to continue to the next node in the tree.
//  	    Return TRUE if you want to abort the walk.
//  	    
//  	vitual
//  	bool
//  	walkNode( off_t node, void * closure ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    'walkNode' is called by 'walkTree' for each node in the
//  	    tree.'closure' is the value passed as closure to 'walkTree'.
//  	    Return FALSE to continue to the next node in the tree.
//  	    Return TRUE if you want to abort the walk.
//  	    
//  	NodeType
//  	getNodeType( off_t node );
//  	    Return the type of 'node'.
//
//  	short
//  	balance( off_t * root );
//  	    Re-balance the subTree at 'root'.
//  	    Returns the change in balance.
//
//
//  Protected Data:
//
//  	baseAddr    the current base address for all offsets
//
//  Private Methods:
//
//  	Node *
//  	node( off_t root );
//  	    convert an offset to a Node *;
//
//  	void
//  	rotateTwice( off_t * root, SubTree dir );
//  	    This method is called by balance to rotate the nodes twice
//  	    and balance the tree. For a more detailed explination,
//  	    see the comments in AvlTreeMethods.INC.
//
//  	short
//  	rotateOnce( off_t * root, SubTree dir );
//  	    This methode is called by balance to rotate the node and
//  	    balance the tree. For a more detailed explination,
//  	    see the comments in AvlTreeMethods.INC.
//
// Notes:
//
//  For more information on the Avl Balanced Tree Algorithims, see:
//    G.M. Adelson-Velskii and E.M. Landis. Doklady Akademia Nauk SSSR, 146,
//    (1962), 263-66; English translation in Soviet Math, 3, 1259-63
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
//  	    	The source in this class is based on an Avl libraray
//  	    	(avl.c) written by: Brad Appleton on 03/01/89
//
// Created:     12/13/94 09:48
//
// Revision History:
//
// 
// $Log$
// Revision 1.3  1995/11/05 16:32:26  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:32  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <ClueConfig.hh>

#include <AvlTreeBase.hh>
#include <sys/types.h>
#include <iostream.h>



class AvlTreeOffsetBase : public AvlTreeBase
{
  
protected:

public:
  // this really should be protected, but the compiler
  // on AIX barfs!
  
  struct Node
  {
    off_t    subTree[2];
    short    bal;
  };

protected:

  AvlTreeOffsetBase( void );
  AvlTreeOffsetBase( const AvlTreeOffsetBase & copyFrom );

  caddr_t   	getBase( void ) const;
  caddr_t    	setBase( void * newBase );
  
  short	    	insertNode( off_t * root, off_t * newNode );
  off_t 	findNode( off_t root, const void * key );
  short	    	deleteNode( off_t * root, off_t * key, int minMax = 0);

  bool	    	walkTree( off_t  root );
  bool	    	walkTree( off_t  root, void * closure );

  void	    	initNode( off_t node );
  
  ostream &    	dumpTree( const off_t root, ostream & dest, int level = 0 ) const;
  
  void	    	destroyTree( off_t * root );
  void	    	destroyTree( off_t * root, void * closure );
		 
  virtual int	compareNode( const off_t one, const off_t two) = 0;
  virtual int	compareFind( const void * one, const off_t two ) = 0;

  virtual void  destroyNode( off_t root ) = 0;
  virtual void  destroyNode( off_t root, void * closure ) = 0;

  virtual bool  walkNode( off_t root ) = 0;
  virtual bool  walkNode( off_t root, void * closure ) = 0;
  
  NodeType  	    getNodeType( off_t  root );

  short	    	    balance( off_t * 	root );

private:

  Node *    	    node( off_t root ) const;
  
  void	    	    rotateTwice( off_t * root,
				 SubTree   	dir );

  short	    	    rotateOnce( off_t * 	root,
				SubTree    	dir );

  caddr_t    	    baseAddr;
  
};


//
// Inline methods
//

inline
AvlTreeOffsetBase::AvlTreeOffsetBase( void )
{
  baseAddr = 0;
}

inline
caddr_t
AvlTreeOffsetBase::getBase( void ) const
{
  return( baseAddr );
}

inline
caddr_t
AvlTreeOffsetBase::setBase( void * newBase )
{
  caddr_t old = baseAddr;
  baseAddr = (caddr_t)newBase;
  return( old );
}

inline
AvlTreeOffsetBase::Node *
AvlTreeOffsetBase::node( off_t root ) const
{
  return( (Node *)( baseAddr + root ) );
}


inline
void
AvlTreeOffsetBase::initNode( off_t root )
{
  node( root )->subTree[ LEFT ]     = 0;
  node( root )->subTree[ RIGHT ]    = 0;
  node( root )->bal 	    	    = 0;
}

#endif // ! def _AvlTreeOffsetBase_hh_ 
//
// This software is a modificed version of Brad Appleton's
// 'c' avl library.
//
// It is public domain.
//
