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
// Revision 2.3  1997/06/25 12:50:37  houghton
// Removed typedef Loc (now in AvlTreeBase).
// cleanup.
//
// Revision 2.2  1997/06/19 12:00:21  houghton
// Changed off_t to Loc
//
// Revision 2.1  1995/11/10 12:42:03  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:26  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:32  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <MdbConfig.hh>

#include <AvlTreeBase.hh>
#include <sys/types.h>
#include <iostream.h>



class AvlTreeOffsetBase : public AvlTreeBase
{

public:

  
protected:

public:
  // this really should be protected, but the compiler
  // on AIX barfs!
  
  struct Node
  {
    Loc	    subTree[2];
    short    bal;
  };

protected:

  AvlTreeOffsetBase( void );
  AvlTreeOffsetBase( const AvlTreeOffsetBase & copyFrom );

  virtual caddr_t   	getBase( void ) = 0;
  virtual const caddr_t getBase( void ) const = 0;
  
  short	    	insertNode( Loc * root, Loc * newNode );
  Loc		findNode( Loc root, const void * key );
  short	    	deleteNode( Loc * root, Loc * key, int minMax = 0);

  bool	    	walkTree( Loc  root );
  bool	    	walkTree( Loc  root, void * closure );

  void	    	initNode( Loc node );
  
  ostream &    	dumpTree( const Loc root,
			  ostream & dest,
			  int level = 0 ) const;

  ostream &	dumpNode( ostream & dest, const Loc root, int level ) const;
  
  virtual ostream & dumpKey( ostream & dest, const Loc keyLoc ) const = 0;
  
  void	    	destroyTree( Loc * root );
  void	    	destroyTree( Loc * root, void * closure );

  virtual int	compareNode( const Loc one, const Loc two) = 0;
  virtual int	compareFind( const void * one, const Loc two ) = 0;

  virtual void  destroyNode( Loc root ) = 0;
  virtual void  destroyNode( Loc root, void * closure ) = 0;

  virtual bool  walkNode( Loc root ) = 0;
  virtual bool  walkNode( Loc root, void * closure ) = 0;
  
  NodeType  	    getNodeType( Loc  root );

  short	    	    balance( Loc * 	root );

private:

  Node *    	    node( Loc root ) const;
  
  void	    	    rotateTwice( Loc * root,
				 SubTree   	dir );

  short	    	    rotateOnce( Loc * 	root,
				SubTree    	dir );

};


//
// Inline methods
//

inline
AvlTreeOffsetBase::AvlTreeOffsetBase( void )
{
}

inline
AvlTreeOffsetBase::Node *
AvlTreeOffsetBase::node( Loc root ) const
{
  return( (Node *)( getBase() + root ) );
}


inline
void
AvlTreeOffsetBase::initNode( Loc root )
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
