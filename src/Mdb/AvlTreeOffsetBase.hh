#ifndef _AvlTreeOffsetBase_hh_
#define _AvlTreeOffsetBase_hh_
//
// File:        AvlTreeOffsetBase.hh
// Project:	Mdb
// Desc:        
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
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
//  	    	The source in this class is based on an Avl libraray
//  	    	(avl.c) written by: Brad Appleton on 03/01/89
//
// Created:     12/13/94 09:48
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

#include <AvlTreeBase.hh>

#include <Str.hh>

#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif

class AvlTreeOffsetBase : public AvlTreeBase
{

public:

  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  
  static const ClassVersion version;

  // this really should be protected, but the compiler
  // on AIX barfs!
  
  struct Node
  {
    Loc	    subTree[2];
    short   bal;
  };

protected:

  AvlTreeOffsetBase( void );
  AvlTreeOffsetBase( const AvlTreeOffsetBase & copyFrom );

  short	    	insertNode( Loc * root, Loc * newNode );
  Loc		findNode( Loc root, const void * key ) const;
  short	    	deleteNode( Loc * root,
			    const void * key,
			    Loc * delNode,
			    int minMax = 0);

  Loc	    	destroyTree( Loc root );
  Loc	    	destroyTree( Loc root, void * closure );

  bool	    	walkTree( Loc  root );
  bool	    	walkTree( Loc  root, void * closure );

  bool	    	walkTree( Loc  root, void * closure ) const;

  inline void	initNode( Loc node );

  NodeType  	getNodeType( Loc root );

  short	    	balance( Loc * root );

  // debug/test methods
  ostream &    	dumpAvlTree( const Loc	root,
			     ostream &	dest,
			     int	level = 0 ) const;

  ostream &	dumpNode( ostream & dest, const Loc root, int level ) const;
  
  virtual ostream &	dumpKey( ostream & dest, const Loc keyLoc ) const;
  
  // pure virtual methods
  
  virtual Addr		getBase( void ) = 0;
  virtual const Addr	getBase( void ) const = 0;
  
  virtual int	compareNode( const Loc one, const Loc two) = 0;
  virtual int	compareFind( const void * one, const Loc two ) const = 0;

  virtual void  destroyNode( Loc root ) = 0;
  virtual void  destroyNode( Loc root, void * closure ) = 0;

  virtual bool  walkNode( Loc root ) = 0;
  virtual bool  walkNode( Loc root, void * closure ) = 0;
  
  virtual bool  walkNode( Loc root, void * closure ) const = 0;
  
  static Str errStr;
  
private:

  inline Node *     node( Loc root ) const;
  
  void		    rotateTwice( Loc * root, SubTree dir );
  short	    	    rotateOnce( Loc * root, SubTree dir );

};

#if !defined( inline )
#include <AvlTreeOffsetBase.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
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
//  Public Interface:
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. AvlTreeOffsetBase )
//
//  	virtual const char *
//  	getVersion( bool withPrjVer = true ) const;
//  	    Return the version string of this class.
//
//	static const ClassVersion version
//	    Class and project version information. (see ClassVersion.hh)
//
//  Protected Interface:
//
//  	short
//  	insertNode( Loc * root, Loc * newNode );
//  	    Insert a new node into the tree. If 'newNode' is a
//  	    duplicate node (see compare), 'newNode' will contain
//  	    the existing node on return.
//  	    Returns the change in tree balance cause by inserting
//  	    the new node.
//
//  	Loc
//  	findNode( Loc root, Loc key );
//  	    Search the tree for a node that matches key.
//  	    Returns the address of the found node or 0 if not found.
//
//  	short
//  	deleteNode( Loc * root, Loc * key, int minMax = 0 );
//  	    Delete the node from the tree that matches 'key'. If 'minMax'
//  	    is > 0, the value of 'key' is ignored and the largest
//  	    (right most) node is deleted. If 'minMax' is < 0, the value
//  	    of 'key' is ignored and the smalles (left most) key is
//  	    deleted. 'key' is set to the the deleted node or 0 if
//  	    no match is found.
//  	    Returns the change in tree balance caused by deleting the node.
//
//  	void
//  	destroyTree( Loc * root );
//  	    Remove all nodes from the tree. A user function is call
//  	    at each node so the data tied to the node can be deleted.
//
//  	void
//  	destroyTree( Loc * root, void * closure );
//  	    Remove all nodes from the tree. A user function is call
//  	    at each node so the data tied to the node can be deleted.
//  	    'closure' is passed to the user function.
//
//  	bool
//  	walkTree( Loc root );
//  	    Walk the entire tree in accending order (left to right) and
//  	    execute a user function at each node.
//  	    Returns FALSE if the entire tree was walked. TRUE
//  	    will be returned if the walk was aborted buy 'walkNode'
//
//  	bool
//  	walkTree( Loc root, void * closure );
//  	    Walk the entire tree in accending order (left to right) and
//  	    execute a user function at each node. 'closure' is passed
//  	    to the user function.
//  	    Returns FALSE if the entire tree was walked. TRUE
//  	    will be returned if the walk was aborted buy 'walkNode'
//
//	inline
//  	void
//  	initNode( Loc node );
//  	    Initialize a new node. Sets all values to 0.
//
//  	NodeType
//  	getNodeType( Loc node );
//  	    Return the type of 'node'.
//
//  	short
//  	balance( Loc * root );
//  	    Re-balance the subTree at 'root'.
//  	    Returns the change in balance.
//
//  	ostream &
//  	dumpAvlTree( const Loc root, ostream & dest, int level = 0 ) const;
//  	    This method will output a 'graph' of the tree structure.
//  	    The graph show each node's position and balance in the tree.
//  	    Returns the ostream passed as 'dest'
//
//	ostream &
//	dumpNode( ostream & dest, const Loc root, int level ) const
//	    Output a single node of the three for 'dumpTree()'.
//  	    Returns the ostream passed as 'dest'
//
//	virtual
//	ostream &
//	dumpKey( ostream & dest, const Loc keyLoc ) const = 0
//	    This virtual should output the key value to 'dest'.
//	    It is called by dumpNode. If it is not implemented by a sub
//	    class, no key will be output.
//  	    Returns the ostream passed as 'dest'
//	
//  Protected Pure Virtual Methods:
//
//	virtual
//	Addr
//	getBase( void ) = 0
//	    This pure virtual needs to return the base address for the
//	    offsets (Loc) used thoughout the tree.
//
//	virtual
//	const Addr
//	getBase( void ) const = 0
//	    This pure virtual needs to return the base address for the
//	    offsets (Loc) used thoughout the tree.
//
//  	virutal
//  	int
//  	compareNode( const Loc one, const Loc two ) = 0;
//  	    This pure virtual function must be defined by any sub classes.
//  	    It is used to determine difference between the two nodes
//  	    passed to it.
//  	    Returns > 0 if node 'one' is greater than node 'two', = 0 if
//  	    node 'one' and 'two' are equal and < 0 if node 'one' is less
//  	    than node two.
//
//  	virutal
//  	int
//  	compareFind( const void * one, const Loc two ) = 0;
//  	    This pure virtual function must be defined by any sub classes.
//  	    It is used to determine difference between a key value and a
//	    node in the tree.
//  	    Returns > 0 if node 'one' is greater than node 'two', = 0 if
//  	    node 'one' and 'two' are equal and < 0 if node 'one' is less
//  	    than node two.
//
//  	vitual
//  	void
//  	destroyNode( Loc node ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    It is called for each node from within 'destroyTree'. It should
//  	    release all memory used by the node including the 'node' it
//  	    self.
//  	    
//  	vitual
//  	void
//  	destroyNode( Loc node, void * closure ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    It is called for each node from within 'destroyTree'. It should
//  	    release all memory used by the node including the 'node' it
//  	    self.'closure' is the value passed as closure to 'dumpTree'.
//
//  	vitual
//  	bool
//  	walkNode( Loc node ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    'walkNode' is called by 'walkTree' for each node in the tree.
//  	    Return FALSE to continue to the next node in the tree.
//  	    Return TRUE if you want to abort the walk.
//  	    
//  	vitual
//  	bool
//  	walkNode( Loc node, void * closure ) = 0;
//  	    This pure virtual function must be defined by all sub classes.
//  	    'walkNode' is called by 'walkTree' for each node in the
//  	    tree.'closure' is the value passed as closure to 'walkTree'.
//  	    Return FALSE to continue to the next node in the tree.
//  	    Return TRUE if you want to abort the walk.
//  	    
//  Private Methods:
//
//  	Node *
//  	node( Loc root );
//  	    convert an offset to a Node *;
//
//  	void
//  	rotateTwice( Loc * root, SubTree dir );
//  	    This method is called by balance to rotate the nodes twice
//  	    and balance the tree. For a more detailed explination,
//  	    see the comments in AvlTreeMethods.INC.
//
//  	short
//  	rotateOnce( Loc * root, SubTree dir );
//  	    This methode is called by balance to rotate the node and
//  	    balance the tree. For a more detailed explination,
//  	    see the comments in AvlTreeMethods.INC.
//
//  Private Methods:
//
//  Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const AvlTreeOffsetBase & src );
//
//	istream &
//	operator >> ( istream & src, AvlTreeOffsetBase & dest );
//
// Notes:
//
//  For more information on the Avl Balanced Tree Algorithims, see:
//    G.M. Adelson-Velskii and E.M. Landis. Doklady Akademia Nauk SSSR, 146,
//    (1962), 263-66; English translation in Soviet Math, 3, 1259-63
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
//	This software is a modificed version of Brad Appleton's
//	'c' avl library.
//
//	It is public domain.
//
// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:42  houghton
// Change Major Version to 4
//
// Revision 2.5  1997/07/19 10:16:42  houghton
// Bug-Fix: changed destroyTree args from Log * to Loc (see log entry in
//     AvlTreeMethods.INC for more info.
// Port(Sun5): Added static errStr to be used by both AvlTreeOffset and
//     DavlTreeOffset. The Sun5 compiler could not handle the static
//     local variable in the template class error() methods.
//
// Revision 2.4  1997/07/13 11:02:07  houghton
// Cleanup.
//
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
#endif // ! def _AvlTreeOffsetBase_hh_ 
