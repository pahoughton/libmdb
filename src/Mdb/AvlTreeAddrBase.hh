#ifndef _AvlTreeAddrBase_hh_
#define _AvlTreeAddrBase_hh_
//
// File:        AvlTreeAddrBase.hh
// Desc:
//
//  NO PUBLIC INTERFACE
//
//  This class provides the methods for managing Avl Trees using
//  direct memory addresses. It can be used for both normal heap
//  (malloc) memory or fixed addressing memory maps. The
//  AvlTreeOffsetBase class provides the same functionallity using
//  memory offsets.
//
//  This base class should only be referenced from the AvlTree
//  template class. It has protected constructors so it can not
//  be directly instantiated.
//              
//  DataTypes:
//
//  	AvlTreeAddrBase	    	class
//  	AvlTreeAddrBase::Node	a single avl tree node.
//
//  Protected Constructors:
//
//  	AvlTreeAddrBase( void );
//  	    This constructor does not do anything it is only declared
//  	    to ensure the class can not be instanciated.
//
//  	AvlTreeAddrBase( const AvlTreeAddrBase & copyFrom );
//  	    Since this class has no data, the copy constructor doesn't
//  	    need to do anything. It defined to prevent
//  	    direct instanciation.
//
//  Protected Interface:
//
//  	short
//  	insertNode( Node ** root, Node ** newNode );
//  	    Insert a new node into the tree. If 'newNode' is a
//  	    duplicate node (see compare), 'newNode' will contain
//  	    the existing node on return.
//  	    Returns the change in tree balance cause by inserting
//  	    the new node.
//
//  	Node *
//  	findNode( Node * root, Node * key );
//  	const Node *
//  	findNode( Node * root, Node * key ) const;
//  	    Search the tree for a node that matches key.
//  	    Returns the address of the found node or 0 if not found.
//
//  	short
//  	deleteNode( Node ** root, Node ** key, int minMax = 0 );
//  	    Delete the node from the tree that matches 'key'. If 'minMax'
//  	    is > 0, the value of 'key' is ignored and the largest
//  	    (right most) node is deleted. If 'minMax' is < 0, the value
//  	    of 'key' is ignored and the smalles (left most) key is
//  	    deleted. 'key' is set to the the deleted node or 0 if
//  	    no match is found.
//  	    Returns the change in tree balance caused by deleting the node.
//
//  	Bool
//  	walkTree( Node * root );
//  	Bool
//  	walkTree( const Node * root ) cosnt;
//  	    Walk the entire tree in accending order (left to right) and
//  	    execute a user function at each node.
//  	    Returns FALSE if the entire tree was walked. TRUE
//  	    will be returned if the walk was aborted buy 'walkNode'
//
//  	Bool
//  	walkTree( Node * root, void * closure );
//  	Bool
//  	walkTree( const Node * root, void * closure ) const;
//  	    Walk the entire tree in accending order (left to right) and
//  	    execute a user function at each node. 'closure' is passed
//  	    to the user function.
//  	    Returns FALSE if the entire tree was walked. TRUE
//  	    will be returned if the walk was aborted buy 'walkNode'
//
//  	void
//  	initNode( Node * node );
//  	    Initialize a new node. Sets all values to 0.
//
//  	ostream &
//  	dumpTree( const Node * root, ostream & dest, int level = 0 ) const;
//  	    This method will output a 'graph' of the tree structure.
//  	    The graph show each node's position and balance in the tree.
//  	    Returns the ostream passed as 'dest'
//
//  	void
//  	destroyTree( Node ** root );
//  	    Remove all nodes from the tree. A user function is call
//  	    at each node so the data tied to the node can be deleted.
//
//  	void
//  	destroyTree( Node ** root, void * closure );
//  	    Remove all nodes from the tree. A user function is call
//  	    at each node so the data tied to the node can be deleted.
//  	    'closure' is passed to the user function.
//
//  	virutal
//  	int
//  	compare( const Node * one, const Node * two ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    It is used to determine difference between the two nodes
//  	    passed to it.
//  	    Returns > 0 if node 'one' is greater than node 'two', = 0 if
//  	    node 'one' and 'two' are equal and < 0 if node 'one' is less
//  	    than node two.
//
//  	vitual
//  	void
//  	destroyNode( Node * node ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    It is called for each node from within 'destroyTree'. It should
//  	    release all memory used by the node including the 'node' it
//  	    self.
//  	    
//  	vitual
//  	void
//  	destroyNode( Node * node, void * closure ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    It is called for each node from within 'destroyTree'. It should
//  	    release all memory used by the node including the 'node' it
//  	    self.'closure' is the value passed as closure to 'dumpTree'.
//
//  	vitual
//  	Bool
//  	walkNode( Node * node ) = 0;
//  	vitual
//  	Bool
//  	walkNode( const Node * node ) const = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    'walkNode' is called by 'walkTree' for each node in the tree.
//  	    Return FALSE to continue to the next node in the tree.
//  	    Return TRUE if you want to abort the walk.
//  	    
//  	vitual
//  	Bool
//  	walkNode( Node * node, void * closure ) = 0;
//  	vitual
//  	Bool
//  	walkNode( const Node * node, void * closure ) const = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    'walkNode' is called by 'walkTree' for each node in the
//  	    tree.'closure' is the value passed as closure to 'walkTree'.
//  	    Return FALSE to continue to the next node in the tree.
//  	    Return TRUE if you want to abort the walk.
//  	    
//
//  Private Methods:
//
//  	NodeType
//  	getNodeType( Node * node );
//  	    Return the type of 'node'.
//
//  	void
//  	rotateTwice( Node ** root, SubTree dir );
//  	    This method is called by balance to rotate the nodes twice
//  	    and balance the tree. For a more detailed explination,
//  	    see the comments in AvlTreeMethods.INC.
//
//  	short
//  	rotateOnce( Node ** root, SubTree dir );
//  	    This methode is called by balance to rotate the node and
//  	    balance the tree. For a more detailed explination,
//  	    see the comments in AvlTreeMethods.INC.
//
//  	short
//  	balance( Node ** root )
//  	    Re-balance the subTree at 'root'.
//  	    Returns the change in balance.
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
// Created:     12/14/94 07:11
//
// Revision History:
//
// 
// $Log$
// Revision 1.1  1995/02/13 16:08:31  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <sys/types.h>
#include <iostream.h>
#include <Common.h>

#include <AvlTreeBase.hh>


class AvlTreeAddrBase : public AvlTreeBase
{
  
public:

protected:

  struct Node
  {
    struct Node *  subTree[2];
    Bool	    bal;
  };

  AvlTreeAddrBase( void ) {};
  AvlTreeAddrBase( const AvlTreeAddrBase & copyFrom ) {};
  
  short	    	insertNode( Node ** root, Node ** newNode );
  Node *	findNode( Node * root, const void * key );
  const Node *	findNode( const Node * root, const void * key ) const;
  short	    	deleteNode( Node ** root, Node ** key, int minMax = 0);

  Bool	    	walkTree( Node * root );
  Bool	    	walkTree( const Node * root ) const;
  Bool	    	walkTree( Node * root, void * closure );
  Bool	    	walkTree( const Node * root, void * closure ) const;

  void	    	initNode( Node * node );
  
  ostream & 	dumpTree( const Node * root, ostream & dest, int level = 0 ) const;
  
  void	    	destroyTree( Node ** root );
  void	    	destroyTree( Node ** root, void * closure );
		 
  virtual int	compareNode( const Node * one, const Node *two) const = 0;
  virtual int	compareFind( const void * one, const Node * two) const = 0;

  virtual Bool  walkNode( Node * node ) = 0;
  virtual Bool  walkNode( const Node * node ) const = 0;
  virtual Bool  walkNode( Node * node, void * closure ) = 0;  
  virtual Bool  walkNode( const Node * node, void * closure ) const = 0;
  
  virtual void  destroyNode( Node * node ) = 0;
  virtual void  destroyNode( Node * node, void * closure ) = 0;

private:

  NodeType  	    getNodeType( Node * root );

  void	    	    rotateTwice( Node ** root, SubTree dir );

  short	    	    rotateOnce( Node ** root, SubTree dir );

  short	    	    balance( Node ** root );
  
};


//
// Inline methods
//

inline
void
AvlTreeAddrBase::initNode( Node * root )
{
  root->subTree[ LEFT ]     = 0;
  root->subTree[ RIGHT ]    = 0;
  root->bal 	    	    = 0;
}

#endif // ! def _AvlTreeAddrBase_hh_ 
//
// This software is a modificed version of Brad Appleton's
// 'c' avl library.
//
// It is public domain.
//

