// Hey Emacs this is really -*- c++ -*-
//
// File:        AvlTreeMethods.INC
// Desc:        
//
//  This is about as strange as it gets! These are the algorithims
//  for the AvlTree management. They need to be implemented with two
//  different fundemental concepts. The first is 'the normal' way using
//  pointers. The second is using offsets. Now the only real difference
//  is the offsets need to be translated into pointers to access 
//  the memory locations. 
//
//  I didn't wan to implement the fundimental algorithms twice so
//  I'm using the preprocessor (cpp) to differenciate between the two.
//
//  This file is included in 'AvlTreeOffsetBase.C' and 
//  'AvlTreeAddrBase.C'. Just befor it is included, the following 
//  macros are defined:
//
//  	ClassName   	= the name of the class
//  	NodeDataType	= the DataType of a Node
//  	NODE( o )   	= translate a node into a address
//
//  	AvlTreeOffsetBase defines these as:
//
//  	    #define ClassName	    AvlTreeOffsetBase
//  	    #define NodeDataType    off_t
//  	    #define NODE( n )	    node( n ) 
//              
//		the node method translates an offset to a AvlOffsetNode *
//
//  	AvlTreeAddrBase defines these as:
//
//  	    #define ClassName	    AvlTreeAddrBase
//  	    #define NodeDataType    AvlTreeAddrBase::Node *
//  	    #define NODE( n )	    (n)
//
//
// Notes:
//
//  For more information on the Avl Balanced Tree Algorithims, see:
//    G.M. Adelson-Velskii and E.M. Landis. Doklady Akademia Nauk SSSR, 146,
//    (1962), 263-66; English translation in Soviet Math, 3, 1259-63
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
//  	    	This source is based on an Avl libraray
//  	    	(avl.c) written by: Brad Appleton on 03/01/89
//
// Created:     12/13/94 11:21 
//
// Revision History:
//
// $Log$
// Revision 2.3  1997/06/25 12:49:52  houghton
// Added dumpNode and reworked dumpTree to use it.
//
// Revision 2.2  1995/12/04 11:19:16  houghton
// Changed to eliminate compile warnings.
//
// Revision 2.1  1995/11/10  12:42:01  houghton
// Change to Version 2
//
// Revision 1.4  1995/11/05  16:32:25  houghton
// Revised
//
// Revision 1.2  1995/02/13  16:15:39  houghton
// Fix comment character
//
// # Revision 1.1  1995/02/13  16:08:31  houghton
// # New Style Avl an memory management. Many New Classes
// #
//
const char AvlTreeMethodsRcsId[] =
"$Id$";

#include <algorithm>
#include <iomanip>
#include <cstdlib>

// **********************************************************************
// insertNode - insert a new node into the tree
//  	Returns effect on subtree balance (1,0,-1).
// **********************************************************************
short
ClassName::insertNode(
  NodeDataType *    root,
  NodeDataType *    newNode
  )
{

  short	    inc;
  int	    diff;

  if( *root == 0 )
    {
      *root = *newNode;
      *newNode = 0;
      return( 1 );
    }

  diff = compareNode( *newNode, *root );

  if( diff < 0 )
    {
      inc = -insertNode( &( NODE( *root )->subTree[ LEFT ] ), newNode );
      if( *newNode != 0 )
	{
	  return( 0 );
	}
    }
  else
    {
      if( diff > 0 )
	{
	  inc = insertNode( &( NODE( *root )->subTree[ RIGHT ] ), newNode );
	  if( *newNode != 0 )
	    {
	      return( 0 );
	    }
	}
      else
	{
	  *newNode = *root;
	  return( 0 );
	}
    }

  NODE( *root )->bal += inc;

  if( inc && NODE( *root )->bal )
    {
      return( 1 - balance( root ) );
    }
  else
    {
      return( 0 );
    }
}

// **********************************************************************
// findNode - find a node in the tree
//  	Returns found node or 0 if not found
// **********************************************************************
NodeDataType
ClassName::findNode(
  NodeDataType 	root,
  const void *	key
  )
{

  for( int diff = compareFind( key, root );
      root != 0 && diff != 0;
      diff = compareFind( key, root ) )
    {
      if( diff < 0 )
	{
	  root = NODE( root )->subTree[ LEFT ];
	}
      else
	{
	  root = NODE( root )->subTree[ RIGHT ];
	}
      
      if( root == 0 )
	{
	  return( root );
	}
    }

  return( root );
}

#if defined( AVL_ADDRESSES )
// **********************************************************************
// findNode - find a node in the tree
//  	Returns found node or 0 if not found
// **********************************************************************
const NodeDataType
ClassName::findNode(
  const NodeDataType 	root,
  const void *	key
  ) const
{

  for( int diff = compareFind( key, root );
      root != 0 && diff != 0;
      diff = compareFind( key, root ) )
    {
      if( diff < 0 )
	{
	  root = NODE( root )->subTree[ LEFT ];
	}
      else
	{
	  root = NODE( root )->subTree[ RIGHT ];
	}
      
      if( root == 0 )
	{
	  return( root );
	}
    }

  return( root );
}
#endif //  def AVL_ADDRESSES 


// **********************************************************************
// deleteNode - delete a node from the tree
//  	Returns effect on subtree balance (1,0,-1).
// **********************************************************************
short
ClassName::deleteNode(
  NodeDataType *    root,
  NodeDataType *    key,
  int	    	    minMax
  )
{

  short     dec = 0;
  int	    diff = minMax;

  if( *root == 0 )
    {
      *key = 0;
      return( 0 );
    }

  if( diff == 0 )
    {
      diff = compareNode( *key , *root );
    }
  else
    {
      if( diff < 0 && NODE( *root )->subTree[ LEFT ] == 0 )
	{
	  diff = 0;
	}

      if( diff > 0 && NODE( *root )->subTree[ RIGHT ] == 0 )
	{
	  diff = 0;
	}
    }

  if( diff < 0 )
    {
      dec = -deleteNode( &( NODE( *root )->subTree[ LEFT ] ), key, minMax );
      if( *key == 0 )
	{
	  return( 0 );
	}
    }
  else
    {
      if( diff > 0 )
	{
	  dec = deleteNode( &( NODE( *root )->subTree[ RIGHT ] ), key, minMax );
	  if( *key == 0 )
	    {
	      return( 0 );
	    }
	}
      else
	{
	  // diff == 0; Found it! now delete it.

	  NodeDataType     origRoot = *root;

	  *key = *root;

	  
	  switch( getNodeType( *root ) )
	    {
	    case LEAF:
	      *root = 0;
	      return( 1 );

	    case RIGHT_BRANCH:
	      *root = NODE( *root )->subTree[ RIGHT ];
	      return( 1 );

	    case LEFT_BRANCH:
	      *root = NODE( *root )->subTree[ LEFT ];
	      return( 1 );

	    case TREE:

	      // This node has both a left an a right subtree so we
	      // want to move the smallest ( right most ) sub tree node
	      // to this possition (ie root) and set its subtrees to
	      // our subtrees.
	      
	      dec = deleteNode( &( NODE( *root )->subTree[ RIGHT ] ), key, -1 );

	      NODE( *key )->subTree[ LEFT ]  = NODE( *root )->subTree[ LEFT ];
	      NODE( *key )->subTree[ RIGHT ] = NODE( *root )->subTree[ RIGHT ];
	      NODE( *key )->bal	    	     = NODE( *root )->bal;

	      *root = *key;
	      *key = origRoot;
	      break;

	    default:	    // can't happen
	      ;
	    }
	}
    }

  NODE( *root )->bal -= dec;

  if( dec )
    {
      if( NODE( *root )->bal )
	{
	  return( balance( root ) );
	}
      else
	{
	  return( 1 );
	}
    }
  else
    {
      return( 0 );
    }
}

// **********************************************************************
// walkTree - execute 'walkNode' at each node in the tree
//  	Returns false if the entire tree was walked
//  	Returns true if the walk was aborted
// **********************************************************************
bool
ClassName::walkTree(
  NodeDataType	root
  )
{

  bool	abort = false;
  
  if( root == 0 )
    {
      return( abort );
    }

  if( NODE( root )->subTree[ LEFT ] != 0 )
    {
      abort = walkTree( NODE( root )->subTree[ LEFT ] );
    }

  if( abort ) return ( abort );
  
  abort = walkNode( root );
  
  if( abort ) return ( abort );
  
  if( NODE( root )->subTree[ RIGHT ] != 0 )
    {
      abort = walkTree( NODE( root )->subTree[ RIGHT ] );
    }
  
  return( abort );
}


// **********************************************************************
// walkTree - execute 'walkNode' at each node in the tree
//  	Returns false if the entire tree was walked
//  	Returns true if the walk was aborted
// **********************************************************************
bool
ClassName::walkTree(
  NodeDataType	root,
  void *    	closure
  )
{

  bool	abort = false;
  
  if( root == 0 )
    {
      return( abort );
    }

  if( NODE( root )->subTree[ LEFT ] != 0 )
    {
      abort = walkTree( NODE( root )->subTree[ LEFT ], closure );
    }

  if( abort ) return ( abort );
  
  abort = walkNode( root, closure );
  
  if( abort ) return ( abort );
  
  if( NODE( root )->subTree[ RIGHT ] != 0 )
    {
      abort = walkTree( NODE( root )->subTree[ RIGHT ], closure );
    }
  
  return( abort );
}

#if defined( AVL_ADDRESSES )
// **********************************************************************
// walkTree - execute 'walkNode' at each node in the tree
//  	Returns false if the entire tree was walked
//  	Returns true if the walk was aborted
// **********************************************************************
bool
ClassName::walkTree(
  const NodeDataType	root
  ) const
{

  bool	abort = false;
  
  if( root == 0 )
    {
      return( abort );
    }

  if( NODE( root )->subTree[ LEFT ] != 0 )
    {
      abort = walkTree( (const NodeDataType)(NODE( root )->subTree[ LEFT ]) );
    }

  if( abort ) return ( abort );
  
  abort = walkNode( root );
  
  if( abort ) return ( abort );
  
  if( NODE( root )->subTree[ RIGHT ] != 0 )
    {
      abort = walkTree( (const NodeDataType)(NODE( root )->subTree[ RIGHT ]) );
    }
  
  return( abort );
}


// **********************************************************************
// walkTree - execute 'walkNode' at each node in the tree
//  	Returns false if the entire tree was walked
//  	Returns true if the walk was aborted
// **********************************************************************
bool
ClassName::walkTree(
  const NodeDataType	root,
  void *    	closure
  ) const
{

  bool	abort = false;
  
  if( root == 0 )
    {
      return( abort );
    }

  if( NODE( root )->subTree[ LEFT ] != 0 )
    {
      abort = walkTree( (const NodeDataType)(NODE( root )->subTree[ LEFT ]), closure );
    }

  if( abort ) return ( abort );
  
  abort = walkNode( root, closure );
  
  if( abort ) return ( abort );
  
  if( NODE( root )->subTree[ RIGHT ] != 0 )
    {
      abort = walkTree( (const NodeDataType)(NODE( root )->subTree[ RIGHT ]), closure );
    }
  
  return( abort );
}

#endif /* ! defined( AVL_ADDRESSES ) */

// **********************************************************************
// dumpTree - output the structure of the tree to 'dest'
//  	Returns 'dest'
// **********************************************************************
ostream &
ClassName::dumpTree(
  const NodeDataType 	root,
  ostream & 	    	dest,
  int	    	    	level
  ) const
{
  if( root == 0 )
    {
      return( dest );
    }
  
  if( NODE( root )->subTree[ RIGHT ] != 0 )
    {
      dumpTree( NODE( root )->subTree[ RIGHT ], dest, level + 1 );
    }

  dumpNode( dest, root, level ) << '\n';
  
  if( NODE( root )->subTree[ LEFT ] != 0 )
    {
      dumpTree( NODE( root )->subTree[ LEFT ], dest, level + 1 );
    }

  return( dest );
}

ostream &
ClassName::dumpNode(
  ostream & 	    	dest,
  const NodeDataType 	root,
  int	    	    	level
  ) const
{
  if( root == 0 )
    {
      return( dest );
    }
  
  dest << setw( (level * 5) + 1 ) << root << ' '
       << level;

  long destFmtFlags = dest.setf(ios::showpos);

  dest << ':' << NODE( root )->bal << ' ';

  dest.flags( destFmtFlags );
  
  dest << ' ' << setw(6) << NODE( root )->subTree[ RIGHT ]
       << ' ' << setw(6) << NODE( root )->subTree[ LEFT ];

  dest << " key: ";
  dumpKey( dest, root );

  return( dest );
}

// **********************************************************************
// destroyTree - remove all nodes from the tree.
//  	    	 calls destroy node for each node so any memory
//  	    	 associated with the node can be released.
// **********************************************************************
void
ClassName::destroyTree(
  NodeDataType *    root
  )
{

  if( root == 0 )
    {
      return;
    }

  if( NODE( *root )->subTree[ LEFT ] != 0 )
    {
      destroyTree( &(NODE( *root )->subTree[ LEFT ]) );
    }

  if( NODE( *root )->subTree[ RIGHT ] != 0 )
    {
      destroyTree( &(NODE( *root )->subTree[ RIGHT ]) );
    }

  destroyNode( *root );
  *root = 0;
}

// **********************************************************************
// destroyTree - remove all nodes from the tree.
//  	    	 calls destroy node for each node so any memory
//  	    	 associated with the node can be released.
// **********************************************************************
void
ClassName::destroyTree(
  NodeDataType *    root,
  void *    	    closure
  )
{

  if( root == 0 )
    {
      return;
    }

  if( NODE( *root )->subTree[ LEFT ] != 0 )
    {
      destroyTree( &(NODE( *root )->subTree[ LEFT ]), closure );
    }

  if( NODE( *root )->subTree[ RIGHT ] != 0 )
    {
      destroyTree( &(NODE( *root )->subTree[ RIGHT ]), closure );
    }

  destroyNode( *root, closure );
  *root = 0;
}

// **********************************************************************
// getNodeType - return the NodeType of 'root'
//  	Returns one of NULL_NODE, TREE, LEFT_BRANCH, RIGHT_BRANCH or LEAF.
// **********************************************************************
ClassName::NodeType
ClassName::getNodeType( NodeDataType  root )
{

  if( root == 0 )
    {
      return( NULL_NODE );
    }
  else
    {
      if( NODE( root )->subTree[LEFT] != 0 )
	{
	  if( NODE( root )->subTree[RIGHT] != 0 )
	    {
	      return( TREE );
	    }
	  else
	    {
	      return( LEFT_BRANCH );
	    }
	}
      else
	{
	  if( NODE( root )->subTree[RIGHT] != 0 )
	    {
	      return( RIGHT_BRANCH );
	    }
	  else
	    {
	      return( LEAF );
	    }
	}
    }
}


// **********************************************************************
// rotateTwice - rebalance the subtree at root by rotating the nodes.
//
//  This explaination is for dir == RIGHT
//
//  rotate twice repossitions the nodes int the tree by changing both
//  the value at 'root' (*root) and it's subtrees. In this explination
//  ROOT will always be the original root passed in.
//
//  	root	    	    	    = ROOT->LEFT->RIGHT
//  	ROOT->LEFT  	    	    = ROOT->LEFT->RIGHT->RIGHT
//  	ROOT->LEFT->RIGHT->RIGHT    = ROOT
//  	ROOT->RIGHT 	    	    = ROOT->LEFT->RIGHT->LEFT
//  	ROOT->LEFT->RIGHT->RIGHT    = ROOT->LEFT
//
//  I know it confusing! just look at it for a while and it will
//  eventually make sence. It took me a few days to understand it too.
// **********************************************************************
void
ClassName::rotateTwice(
  NodeDataType *    root,
  SubTree   	    dir
  )
{
  SubTree   	    other   	= (dir == RIGHT) ? LEFT : RIGHT;
  NodeDataType	    origRoot 	= *root;
  NodeDataType	    origOther	= NODE( *root )->subTree[ other ];

  *root = NODE( origOther )->subTree[ dir ];

  NODE( origRoot )->subTree[ other ]	= NODE( *root )->subTree[ dir ];
  NODE( *root )->subTree[ dir ]	    	= origRoot;

  NODE( origOther )->subTree[ dir ] 	= NODE( *root )->subTree[ other ];
  NODE( *root )->subTree[ other ]   	= origOther;

  NODE( NODE( *root )->subTree[ LEFT ] )->bal	= -max( NODE( *root )->bal, (short)0 );
  NODE( NODE( *root )->subTree[ RIGHT ] )->bal	= -min( NODE( *root )->bal, (short)0 );
  NODE( *root )->bal  	    	    	    	= 0;

}

// **********************************************************************
// rotateOnce - rebalance the subtree at root by rotating the nodes.
//
//  This explaination is for dir == RIGHT
//
//  rotate once repossitions the nodes int the tree by changing both
//  the value at 'root' (*root) and it's subtrees. In this explination
//  ROOT will always be the original root passed in.
//
//  	root	    	    	    = ROOT->LEFT
//  	ROOT->LEFT  	    	    = ROOT->RIGHT
//  	ROOT->LEFT->RIGHT   	    = ROOT
//
// **********************************************************************
short
ClassName::rotateOnce(
  NodeDataType *    root,
  SubTree   	    dir
  )
{
  SubTree   	other 	    = (dir == RIGHT) ? LEFT : RIGHT;
  NodeDataType	origRoot    = *root;

  bool	    	unChanged;

  unChanged = ( NODE( NODE( *root )->subTree[ other ] )->bal ) ? false : true;

  *root = NODE( *root )->subTree[ other ];

  NODE( origRoot )->subTree[ other ] 	= NODE( *root )->subTree[ dir ];
  NODE( *root )->subTree[ dir ]     	= origRoot;

#if !defined( NEW_WAY )
  NODE( origRoot )->bal = -( (dir == LEFT) ?
			     --( NODE( *root )->bal ) :
			     ++( NODE( *root )->bal ) );
#else
  NODE( *root )->bal += (( dir == LEFT ) ? -1 : 1 );
  
  NODE( origRoot )->bal = NODE( *root )->bal * -1;
#endif
  
  return( unChanged );
}

// **********************************************************************
// balance - rebalance the subtree as needed.
//  	Returns change in balance.
// **********************************************************************
short
ClassName::balance(
  NodeDataType *    root
  )
{

  bool	    special = false;

  if( NODE( *root )->bal < LEFT_HEAVY )
    {
      if( NODE( NODE( *root )->subTree[ LEFT ] )->bal == RIGHT_HEAVY )
	{
	  rotateTwice( root, RIGHT );
	}
      else
	{
	  special = rotateOnce( root, RIGHT );
	}
    }
  else
    {
      if( NODE( *root )->bal > RIGHT_HEAVY )
	{
	  if( NODE( NODE( *root )->subTree[ RIGHT ] )->bal == LEFT_HEAVY )
	    {
	      rotateTwice( root, LEFT );
	    }
	  else
	    {
	      special = rotateOnce( root, LEFT );
	    }
	}
      else
	{
	  special = true;
	}
    }
  return( (special) ? 0 : 1 );
}

//
// This software is a modificed version of Brad Appleton's
// 'c' avl library.
//
// It is public domain.
//
