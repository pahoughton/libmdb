//
// File:        RBTreeBase.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for RBTreeBase
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/08/97 03:01
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "RBTreeBase.hh"
#include <Str.hh>

#if defined( MDB_DEBUG )
#include "RBTreeBase.ii"
#endif

MDB_VERSION(
  RBTreeBase,
  "$Id$");

const unsigned long RBTreeBase::rbTreeVersion = 0x52665401; // RBT01

RBTreeBase::RBTreeBase( ChunkMgr & chunkMgr, Loc headerInitialLoc )
  : mgr( chunkMgr ),
    headerLoc( headerInitialLoc )
{
  if( ! headerLoc )
    {
      if( ! (headerLoc = mgr.alloc( sizeof( RBTreeHeader ) ) ) )
	return;
      
      header().version	    = rbTreeVersion;
      header().count	    = 0;
      root()		    = 0;
      first()		    = headerLoc;
      last()		    = headerLoc;
    }
  else
    {
      if( header().version != rbTreeVersion )
	{
	  setError( E_VERSION );
	  return;
	}
    }      
}

RBTreeBase::~RBTreeBase( void )
{
}

RBTreeBase::Loc
RBTreeBase::insertNode(
  NodeBase & currentNode,
  NodeBase & parentNode,
  NodeBase & node
  )
{
  ++header().count;

  if( parentNode.loc() == headerLoc ||
      currentNode.loc() ||
      lessKey( node.loc(), parentNode.loc() ) )
    {
      parentNode.left() = node.loc();
      if( parentNode.loc() == headerLoc )
	{
	  root() = node.loc();
	  last() = node.loc();
	}
      else
	{
	  if( parentNode.loc() == first() )
	    {
	      first() = node.loc();
	    }
	}
    }
  else
    {
      parentNode.right() = node.loc();
      if( parentNode.loc() == last() )
	{
	  last() = node.loc();
	}
    }

  node.set( parentNode.loc(), 0, 0, Red );
  currentNode = node.loc();
	  
  while( currentNode.loc() != root() &&
	 parent( currentNode ).color() == Red )
    {	      
      //               n
      //           n       n
      //         c   n   n   n

      if( currentNode.parent() == parent( currentNode.parent() ).left() )
	{
	  NodeBase  p( mgr, parent( currentNode.parent() ).right() );

	  if( p.color() == Red )
	    {
	      parent( currentNode ).color() = Black;
	      p.color() = Black;
	      parent( currentNode.parent() ).color() = Red;
	      currentNode = parent( currentNode ).parent();
	    }
	  else
	    {
	      //             n
	      //         n       c
	      
	      if( currentNode.loc() == parent( currentNode ).right() )
		{
		  currentNode = currentNode.parent();
		  rotate_left( currentNode.loc() );
		}
	      parent( currentNode ).color() = Black;
	      parent( currentNode.parent() ).color() = Red;
	      rotate_right( parent( currentNode.parent() ).loc() );
	    }
	}
      else
	{
	  NodeBase p( mgr, parent( currentNode.parent() ).left() );

	  if( p.color() == Red )
	    {
	      parent( currentNode ).color() = Black;
	      p.color() = Black;
	      parent( currentNode.parent() ).color() = Red;
	      currentNode = parent( currentNode ).parent();
	    }
	  else
	    {
	      if( currentNode.loc() == parent( currentNode ).left() )
		{
		  currentNode = currentNode.parent();
		  rotate_right( currentNode.loc() );
		}
	      parent( currentNode ).color() = Black;
	      parent( currentNode.parent() ).color() = Red;
	      rotate_left( parent( currentNode.parent() ).loc() );
	    }
	}
    }

  NodeBase rootNode( mgr, root() );
  rootNode.color() = Black;
  
  return( node.loc() );
}


RBTreeBase::Loc
RBTreeBase::insert( Loc nodeLoc )
{
  
  NodeBase  node( mgr, nodeLoc );
  NodeBase  parentNode( mgr, headerLoc );
  NodeBase  currentNode( mgr, root() );

  bool	    smaller = true;

  while( currentNode.loc() )
    {
      parentNode = currentNode.loc();
	  
      smaller = lessKey( node.loc(), currentNode.loc() );

      currentNode = ( smaller ? currentNode.left() : currentNode.right() );
    }

  NodeBase  prevParentNode( mgr, parentNode.loc() );

  if( smaller )
    {
      if( parentNode.loc() == first() )
	{
	  return( insertNode( currentNode, parentNode, node ) );
	}
      else
	{
	  prevParentNode.prev();
	}
    }

  if( lessKey( prevParentNode.loc(), node.loc() ) )
    return( insertNode( currentNode, parentNode, node ) );

  return( 0 );
}
  
bool
RBTreeBase::erase( Loc loc )
{
  NodeBase node( mgr, loc );
  NodeBase one( mgr, loc );
  NodeBase two( mgr, loc );

  if( ! one.left() )
    {
      two = one.right();
    }
  else
    {
      if( ! one.right() )
	{
	  two = one.left();
	}
      else
	{
	  one = one.right();
	  while( one.left() )
	    {
	      one = one.left();
	    }
	  two = one.right();
	}
    }

  
  if( one.loc() != node.loc() )
    {
      // relink one inplace of node
      
      left( node ).parent() = one;
      one.left() = node.left();

      if( one != node.right() )
	{
	  two.parent() = one.parent(); // possibly two == 0
	  parent( one ).left() = two;  // one must be a left child
	  one.right() = node.right();
	  right( node ).parent() = one;
	}
      else
	{
	  two.parent() = one; // needed in case two == 0
	}

      if( node.loc() == root() )
	{
	  root() = one;
	}
      else
	{
	  if( parent( node ).left() == node.loc() )
	    {
	      parent( node ).left() = one;
	    }
	  else
	    {
	      parent( node ).right() = one;
	    }
	}

      one.parent() = node.parent();

      ::swap( one.color(), node.color() );
      
      one = node.loc();
    }
  else
    {
      // one == node
      two.parent() = one.parent();  // possibly two == 0
      if( node.loc() == root() )
	{
	  root() = two.loc();
	}
      else
	{
	  if( parent( node ).left() == node.loc() )
	    {
	      parent( node ).left() = two;
	    }
	  else
	    {
	      parent( node ).right() = two;
	    }
	}
      if( node.loc() == first() )
	{
	  if( ! node.right() )
	    {
	      // node.left() must be 0 also
	      first() = node.parent();
	      // makes first() == header if node.loc() == root()
	    }
	  else
	    {
	      first() = minimum( two );
	    }
	}
      if( node.loc() == last() )
	{
	  if( ! node.left() )
	    {
	      last() = node.parent();
	    }
	  else
	    {
	      last() = maximum( two );
	    }
	}
    }

  if( one.color() != Red )
    {
      while( two.loc() != root() && two.color() == Black )
	{
	  if( two.loc() == parent( two ).left() )
	    {
	      NodeBase tmp( mgr, parent( two ).right() );

	      if( tmp.color() == Red )
		{
		  tmp.color() = Black;
		  parent( two ).color() = Red;
		  rotate_left( two.parent() );
		  tmp = parent( two ).right();
		}
	      if( left( tmp ).color() == Black &&
		  right( tmp ).color == Black )
		{
		  tmp.color() = Red;
		  two = two.parent();
		}
	      else
		{
		  if( right( tmp ).color() == Black )
		    {
		      left( tmp ).color() = Black;
		      tmp.color() = Red;
		      rotate_right( tmp.loc() );
		      tmp = parent( two ).rigth();
		    }
		  tmp.color() = parent( two ).color();
		  parent( two ).color() = Black;
		  right( tmp ).color() = Black;
		  rotate_left( two.parent() );
		  break;
		}
	    }
	  else
	    {
	      NodeBase tmp( mgr, parent( two ).left() );

	      if( tmp.color() == Red )
		{
		  tmp.color() = Black;
		  parent( two ).color = Red;
		  rotate_right( two.parent() );
		  tmp = parent( two ).left();
		}
	      if( right( tmp ).color() == Black &&
		  left( tmp ).color() == Black )
		{
		  tmp.color() = Red;
		  two = two.parent();
		}
	      else
		{
		  if( left( tmp ).color() == Black )
		    {
		      right( tmp ).color() = Black;
		      tmp.color() = Red;
		      rotate_left( tmp.loc() );
		      tmp = parent( two ).left();
		    }
		  tmp.color() = parent( two ).color();
		  parent( two ).color() = Black;
		  left( tmp ).color() = Black;
		  rotate_right( two.parent() );
		  break;
		}
	    }
	}

      two.color = Black;
    }

  -- header().count;
}

	    
void
RBTreeBase::rotate_right( Loc loc )
{
  NodeBase node( mgr, loc );
  NodeBase nLeft( mgr, node.left() );

  node.left() = nLeft.right();

  if( nLeft.right() != 0 )
    right( nLeft ).parent() = node.loc();

  nLeft.parent() = node.parent();

  if( node.loc() == root() )
    {
      root() = nLeft.loc();
    }
  else
    {
      if( node.loc() == parent( node ).right() )
	parent( node ).right() = nLeft.loc();
      else
	parent( node ).left() = nLeft.loc();
    }
  nLeft.right() = node.loc();
  node.parent() = nLeft.loc();
}

void
RBTreeBase::rotate_left( Loc loc )
{
  NodeBase node( mgr, loc );
  NodeBase nRight( mgr, node.right() );

  node.right() = nRight.left();

  if( nRight.left() != 0 )
    left( nRight ).parent() = node.loc();

  nRight.parent() = node.parent();

  if( node.loc() == root() )
    {
      root() = nRight.loc();
    }
  else
    {
      if( node.loc() == parent( node ).left() )
	parent( node ).left() = nRight.loc();
      else
	parent( node ).right() = nRight.loc();
    }
  nRight.left() = node.loc();
  node.parent() = nRight.loc();
}
      
      
    
  
	  
		      
	  
		      
  
bool
RBTreeBase::good( void ) const
{
  return( true );
}

const char *
RBTreeBase::error( void ) const
{
  static Str errStr;

  errStr = RBTreeBase::getClassName();

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
RBTreeBase::getClassName( void ) const
{
  return( "RBTreeBase" );
}

const char *
RBTreeBase::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
RBTreeBase::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << RBTreeBase::getClassName() << ":\n"
	 << RBTreeBase::getVersion() << '\n';

  if( ! RBTreeBase::good() )
    dest << prefix << "Error: " << RBTreeBase::error() << '\n';
  else
    dest << prefix << "Good" << '\n';


  return( dest );
}

bool
RBTreeBase::setError( ErrorNum err )
{
  errorNum = err;
  return( good() );
}

// Revision Log:
//
// $Log$
// Revision 2.1  1997/06/05 11:29:13  houghton
// Initial Version.
//
//
