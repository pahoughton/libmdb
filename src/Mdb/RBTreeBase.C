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
#include <StlUtilsMisc.hh>
#include <iomanip>

#if defined( MDB_DEBUG )
#include "RBTreeBase.ii"
#endif

MDB_VERSION(
  RBTreeBase,
  "$Id$");

Str RBTreeBase::errStr;

#if defined( RBT_TEST )

static long	ins_FIRST = 0;
static long	ins_NOT_FIRST = 0;
static long	ins_NOT_SMALLER = 0;
static long	ins_DUP_FAIL = 0;

static long	insNode_P_HEADER = 0;
static long	insNode_N_LT_P = 0;
static long	insNode_P_FIRST = 0;
static long	insNode_P_LAST = 0;
static long	insNode_N_GT_P = 0;
static long	insNode_BPPR_RED = 0;
static long	insNode_B_EQ_BPR = 0;
static long	insNode_BPPR_BLACK = 0;
static long	insNode_BPPL_RED = 0;
static long	insNode_B_EQ_BPL = 0;
static long	insNode_BPPL_BLACK = 0;

static long	rr_NLR = 0;
static long	rr_N_EQ_ROOT = 0;
static long	rr_NPR = 0;
static long	rr_NPL = 0;

static long	rl_NRL = 0;
static long	rl_N_EQ_ROOT = 0;
static long	rl_NPL = 0;
static long	rl_NPR = 0;

static long	erase_NO_LEFT = 0;
static long	erase_NO_RIGHT = 0;
static long	erase_LEFT_RIGHT = 0;

static long	erase_YNZ_YNZR_NX = 0;
static long	erase_YNZ_YNZR_X = 0;
static long	erase_YNZ_YZR = 0;
static long	erase_YNZ_Z_EQ_ROOT = 0;
static long	erase_YNZ_Z_EQ_ZPL = 0;
static long	erase_YNZ_Z_NE_ZPL = 0;
static long	erase_YZ_X = 0;
static long	erase_YZ_NX = 0;
static long	erase_YZ_Z_EQ_ROOT = 0;
static long	erase_YZ_Z_EQ_ZPL = 0;
static long	erase_YZ_Z_NE_ZPL = 0;
static long	erase_YZ_Z_EQ_F_NZR = 0;
static long	erase_YZ_Z_EQ_F_ZR = 0;
static long	erase_YZ_Z_EQ_L_NZL = 0;
static long	erase_YZ_Z_EQ_L_ZL = 0;
static long	erase_Y_BLACK = 0;
static long	erase_Y_RED = 0;
static long	erase_X_EQ_XPL_TRED = 0;
static long	erase_X_EQ_XPL_TB = 0;
static long	erase_X_EQ_XPL_TLB_TRB = 0;
static long	erase_X_EQ_XPL_TRB_TL = 0;
static long	erase_X_EQ_XPL_TRB_NTL = 0;
static long	erase_X_EQ_XPL_TR = 0;
static long	erase_X_EQ_XPL_NTR = 0;
static long	erase_X_NE_XPL_TRED = 0;
static long	erase_X_NE_XPL_TB = 0;
static long	erase_X_NE_XPL_TLB_TRB = 0;
static long	erase_X_NE_XPL_TLB_TR = 0;
static long	erase_X_NE_XPL_TLB_NTR = 0;
static long	erase_X_NE_XPL_TL = 0;
static long	erase_X_NE_XPL_NTL = 0;


#define RBT_TEST_FUNCT( x )	++ x;  // { cout << #x << endl; ++ x; }
#else
#define RBT_TEST_FUNCT( x )
#endif

const unsigned long RBTreeBase::rbTreeVersion = 0x52665401; // RBT01

RBTreeBase::RBTreeBase(
  MultiMemOffset *	memMgr,
  unsigned short	treeKey,
  bool			create
  )
  : mgr( memMgr ),
    errorNum( E_OK )
{
  if( ! mgr || ! mgr->good() )
    return;
  
  if( create )
    {
      if( ! (headerLoc = mgr->allocate( sizeof( RBTreeHeader ) ) ) )
	return;

      header().version	    = rbTreeVersion;
      header().count	    = 0;
      header().color	    = Red;
      root()		    = 0;
      first()		    = headerLoc;
      last()		    = headerLoc;
      
      if( ! mgr->setNewKey( treeKey, headerLoc ) )
	{
	  errorNum = E_BADTREEKEY;
	}
    }
  else
    {
      if( ! (headerLoc = mgr->getKey( treeKey ) ) )
	return;

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
  const NodeBase & currentNode,
  NodeBase & parentNode,
  NodeBase & node
  )
{
  ++header().count;

  if( parentNode.loc() == headerLoc ||
      currentNode.loc() ||
      lessKey( node.loc(), parentNode.loc() ) )
    {
      //    parent is header ( i.e. empty tree )
      // or inserting a dup key
      // or node < parent
      
      parentNode.left() = node.loc();  // if header, this is first
      if( parentNode.loc() == headerLoc )
	{
	  RBT_TEST_FUNCT( insNode_P_HEADER )
	  root() = node.loc();
	  last() = node.loc();
	}
      else
	{
	  RBT_TEST_FUNCT( insNode_N_LT_P );
	  if( parentNode.loc() == first() )
	    {
	      RBT_TEST_FUNCT( insNode_P_FIRST );
	      first() = node.loc();
	    }
	}
    }
  else
    {
      //     parent != header
      // and not a dup
      // and node > parent
      parentNode.right() = node.loc();
      
      if( parentNode.loc() == last() )
	{
	  RBT_TEST_FUNCT( insNode_P_LAST );
	  last() = node.loc();
	}
#if defined( RBT_TEST )
      else
	{
	  RBT_TEST_FUNCT( insNode_N_GT_P );
	}
#endif
    }

  node.set( parentNode.loc(), 0, 0, Red );

  NodeBase  bal( *mgr, node.loc() );
	  
  while( bal.loc() != root() &&
	 parent( bal ).color() == Red )
    {
      if( bal.parent() == parent( bal.parent() ).left() )
	{
	  //     
	  //             n         n
	  //           p    or   p
	  //         b             b

	  NodeBase  bppr( *mgr, parent( bal.parent() ).right() );

	  if( bppr.loc() && bppr.color() == Red )
	    {
	      RBT_TEST_FUNCT( insNode_BPPR_RED );
	      
	      parent( bal ).color() = Black;
	      bppr.color() = Black;
	      parent( bal.parent() ).color() = Red;
	      bal = parent( bal ).parent();
	    }
	  else
	    {
	      if( bal.loc() == parent( bal ).right() )
		{
		  //             n
		  //         n       c
		  RBT_TEST_FUNCT( insNode_B_EQ_BPR );
		  
		  bal = bal.parent();
		  rotate_left( bal.loc() );
		}
#if defined( RBT_TEST )
	      else
		{
		  RBT_TEST_FUNCT( insNode_BPPR_BLACK );
		}
#endif
	      parent( bal ).color() = Black;
	      parent( bal.parent() ).color() = Red;
	      rotate_right( parent( bal.parent() ).loc() );
	    }
	}
      else
	{
	  NodeBase bppl( *mgr, parent( bal.parent() ).left() );

	  if( bppl.loc() && bppl.color() == Red )
	    {
	      RBT_TEST_FUNCT( insNode_BPPL_RED );
	      
	      parent( bal ).color() = Black;
	      bppl.color() = Black;
	      parent( bal.parent() ).color() = Red;
	      bal = parent( bal ).parent();
	    }
	  else
	    {
	      
	      if( bal.loc() == parent( bal ).left() )
		{
		  RBT_TEST_FUNCT( insNode_B_EQ_BPL );
		  
		  bal = bal.parent();
		  rotate_right( bal.loc() );
		}
#if defined( RBT_TEST )
	      else
		{
		  RBT_TEST_FUNCT( insNode_BPPL_BLACK );
		}
#endif
	      parent( bal ).color() = Black;
	      parent( bal.parent() ).color() = Red;
	      rotate_left( parent( bal.parent() ).loc() );
	    }
	}
    }

  NodeBase rootNode( *mgr, root() );
  rootNode.color() = Black;
  
  return( node.loc() );
}


RBTreeBase::Loc
RBTreeBase::insert( Loc nodeLoc )
{
  
  NodeBase  node( *mgr, nodeLoc );
  NodeBase  parentNode( *mgr, headerLoc );
  NodeBase  currentNode( *mgr, root() );

  bool	    smaller = true;

  while( currentNode.loc() )
    {
      parentNode = currentNode.loc();
	  
      smaller = lessKey( node.loc(), currentNode.loc() );

      currentNode = ( smaller ? currentNode.left() : currentNode.right() );
    }

  NodeBase  prevParentNode( *mgr, parentNode.loc() );

  if( smaller )
    {
      if( parentNode.loc() == first() )
	{
	  RBT_TEST_FUNCT( ins_FIRST );
	  return( insertNode( currentNode, parentNode, node ) );
	}
      else
	{
	  RBT_TEST_FUNCT( ins_NOT_FIRST );
	  prevParentNode.prev();
	}
    }

  if( lessKey( prevParentNode.loc(), node.loc() ) )
    {
      RBT_TEST_FUNCT( ins_NOT_SMALLER );
      return( insertNode( currentNode, parentNode, node ) );
    }
  else
    {
      RBT_TEST_FUNCT( ins_DUP_FAIL );
      return( prevParentNode.loc() );
    }
}
  
bool
RBTreeBase::erase( Loc loc )
{
  NodeBase z( *mgr, loc );
  NodeBase y( *mgr, loc );
  NodeBase x( *mgr, 0 );
  NodeBase xp( *mgr, 0 );

  -- header().count;
  
  if( ! y.left() )
    {
      RBT_TEST_FUNCT( erase_NO_LEFT );
      x = y.right();
    }
  else
    {
      if( ! y.right() )
	{
	  RBT_TEST_FUNCT( erase_NO_RIGHT );
	  x = y.left();
	}
      else
	{
	  RBT_TEST_FUNCT( erase_LEFT_RIGHT );
	  for( y = y.right(); y.left(); y = y.left() );
	  x = y.right();
	}
    }

  if( y.loc() != z.loc() )
    {
      left( z ).parent() = y.loc();
      y.left() = z.left();
      
      if( y.loc() != z.right() )
	{
	  xp = y.parent();
	  if( x.loc() )
	    {
	      RBT_TEST_FUNCT( erase_YNZ_YNZR_NX );
	      x.parent() = y.parent();
	    }
#if defined( RBT_TEST )
	  else
	    {
	      RBT_TEST_FUNCT( erase_YNZ_YNZR_X );
	    }
#endif
	  parent( y ).left() = x.loc();
	  y.right() = z.right();
	  right( z ).parent() = y.loc();
	}
      else
	{
	  RBT_TEST_FUNCT( erase_YNZ_YZR );
	  xp = y.loc();
	}
      
      if( root() == z.loc() )
	{
	  RBT_TEST_FUNCT( erase_YNZ_Z_EQ_ROOT );
	  root() = y.loc();
	}
      else
	{
	  if( parent( z ).left() == z.loc() )
	    {
	      RBT_TEST_FUNCT( erase_YNZ_Z_EQ_ZPL );
	      parent( z ).left() = y.loc();
	    }
	  else
	    {
	      RBT_TEST_FUNCT( erase_YNZ_Z_NE_ZPL );
	      parent( z ).right() = y.loc();
	    }
	}

      y.parent() = z.parent();
      ::swap( y.color(), z.color() );
      y = z.loc();
    }
  else
    {
      xp = y.parent();
      if( x.loc() )
	{
	  RBT_TEST_FUNCT( erase_YZ_X );
	  x.parent() = y.parent();
	}
#if defined( RBT_TEST )
      else
	{
	  RBT_TEST_FUNCT( erase_YZ_NX );
	}
#endif
      
      if( z.loc() == root() )
	{
	  RBT_TEST_FUNCT( erase_YZ_Z_EQ_ROOT );
	  root() = x.loc();
	}
      else
	{
	  if( parent( z ).left() == z.loc() )
	    {
	      RBT_TEST_FUNCT( erase_YZ_Z_EQ_ZPL );
	      parent( z ).left() = x.loc();
	    }
	  else
	    {
	      RBT_TEST_FUNCT( erase_YZ_Z_NE_ZPL );
	      parent( z ).right() = x.loc();
	    }
	}
      
      if( z.loc() == first() )
	{
	  if( ! z.right() )
	    {
	      RBT_TEST_FUNCT( erase_YZ_Z_EQ_F_NZR );
	      first() = z.parent();
	    }
	  else
	    {
	      RBT_TEST_FUNCT( erase_YZ_Z_EQ_F_ZR );
	      first() = minimum( x ).loc();
	    }
	}
      if( z.loc() == last() )
	{
	  if( ! z.left() )
	    {
	      RBT_TEST_FUNCT( erase_YZ_Z_EQ_L_NZL );
	      last() = z.parent();
	    }
	  else
	    {
	      RBT_TEST_FUNCT( erase_YZ_Z_EQ_L_ZL );
	      last() = maximum( x ).loc();
	    }
	}
    }

  if( y.color() != Red )
    {
      RBT_TEST_FUNCT( erase_Y_BLACK );
      while( x.loc() != root() && ( x.loc() == 0 || x.color() == Black ) )
	{
	  if( x.loc() == xp.left() )
	    {
	      NodeBase tmp( *mgr, xp.right() );
	      if( tmp.color() == Red )
		{
		  RBT_TEST_FUNCT( erase_X_EQ_XPL_TRED );
		  tmp.color() = Black;
		  xp.color() = Red;
		  rotate_left( xp.loc() );
		  tmp = xp.right();
		}
#if defined( RBT_TEST )
	      else
		{
		  RBT_TEST_FUNCT( erase_X_EQ_XPL_TB );
		}
#endif
	      
	      if( ( tmp.left() == 0 || left( tmp ).color() == Black ) &&
		  ( tmp.right() == 0 || right( tmp ).color() == Black ) )
		{
		  RBT_TEST_FUNCT( erase_X_EQ_XPL_TLB_TRB );
		  tmp.color() = Red;
		  x = xp.loc();
		  xp = xp.parent();
		}
	      else
		{
		  if( tmp.right() == 0 || right( tmp ).color() == Black )
		    {
		      if( tmp.left() )
			{
			  RBT_TEST_FUNCT( erase_X_EQ_XPL_TRB_TL );
			  left( tmp ).color() = Black;
			}
#if defined( RBT_TEST )
		      else
			{
			  RBT_TEST_FUNCT( erase_X_EQ_XPL_TRB_NTL );
			}
#endif
		      tmp.color() = Red;
		      rotate_right( tmp.loc() );
		      tmp = xp.right();
		    }
		  tmp.color() = xp.color();
		  xp.color() = Black;
		  if( tmp.right() )
		    {
		      RBT_TEST_FUNCT( erase_X_EQ_XPL_TR );
		      right( tmp ).color() = Black;
		    }
#if defined( RBT_TEST )
		  else
		    {
		      RBT_TEST_FUNCT( erase_X_EQ_XPL_NTR );
		    }
#endif
		  rotate_left( xp.loc() );
		  break;
		}
	    }
	  else
	    {
	      NodeBase tmp( *mgr, xp.left() );
	      if( tmp.color() == Red )
		{
		  RBT_TEST_FUNCT( erase_X_NE_XPL_TRED );
		  tmp.color() = Black;
		  xp.color() = Red;
		  rotate_right( xp.loc() );
		  tmp = xp.left();
		}
#if defined( RBT_TEST )
	      else
		{
		  RBT_TEST_FUNCT( erase_X_NE_XPL_TB );
		}
#endif
	      if( (tmp.right() == 0 || right( tmp ).color() == Black ) &&
		  (tmp.left() == 0 || left( tmp ).color() == Black ) )
		{
		  RBT_TEST_FUNCT( erase_X_NE_XPL_TLB_TRB );
		  tmp.color() = Red;
		  x = xp.loc();
		  xp = xp.parent();
		}
	      else
		{
		  if( tmp.left() == 0 || left( tmp ).color() == Black )
		    {
		      if( tmp.right() )
			{
			  RBT_TEST_FUNCT( erase_X_NE_XPL_TLB_TR );
			  right( tmp ).color() = Black;
			}
#if defined( RBT_TEST )
		      else
			{
			  RBT_TEST_FUNCT( erase_X_NE_XPL_TLB_NTR );
			}
#endif
		      tmp.color() = Red;
		      rotate_left( tmp.loc() );
		      tmp = xp.left();
		    }
		  tmp.color() = xp.color();
		  xp.color() = Black;
		  if( tmp.left() )
		    {
		      RBT_TEST_FUNCT( erase_X_NE_XPL_TL );
		      left( tmp ).color() = Black;
		    }
#if defined( RBT_TEST )
		  else
		    {
		      RBT_TEST_FUNCT( erase_X_NE_XPL_NTL );
		    }
#endif
		  rotate_right( xp.loc() );
		  break;
		}
	    }
	}
      if( x.loc() )
	x.color() = Black;
    }
#if defined( RBT_TEST )
  else
    {
      RBT_TEST_FUNCT( erase_Y_RED );
    }
#endif
  
  return( true );
}
	  
#if defined( FIXME )
  NodeBase node( *mgr, loc );
  NodeBase one( *mgr, loc );
  NodeBase two( *mgr, loc );
  NodeBase bal( *mgr, loc );
  
  if( ! node.left() )
    {
      RBT_TEST_FUNCT( ++ erase_NO_LEFT );
      two = node.right();
    }
  else
    {
      if( ! node.right() )
	{
	  RBT_TEST_FUNCT( ++ erase_NO_RIGHT );
	  two = node.left();
	}
      else
	{
	  RBT_TEST_FUNCT( ++ erase_LEFT_RIGHT );
	  for( one = node.right(); one.left(); one = one.left() );
	  two = one.right();
	}
    }

  
  if( one.loc() != node.loc() )
    {
      // node had a left and a right
      // relink one inplace of node
      RBT_TEST_FUNCT( ++ erase_LR_UNLINK );
      
      left( node ).parent() = one.loc();
      one.left() = node.left();

      if( one.loc() != node.right() )
	{
	  // we went down left more that one node.
	  RBT_TEST_FUNCT( ++ erase_N1NR );
	  if( two.loc() )
	    { 
	      RBT_TEST_FUNCT( ++ erase_N1NRT );
	      two.parent() = one.parent();
	    }
	  else
	    {
	      bal = one.loc();
	    }
	  parent( one ).left() = two.loc();  // one must be a left child
	  one.right() = node.right();
	  right( node ).parent() = one.loc();
	}
      else
	{
	  RBT_TEST_FUNCT( ++ erase_1NR );
	  if( two.loc() )
	    {
	      RBT_TEST_FUNCT( ++ erase_1NRT );
	      two.parent() = one.loc(); 
	    }
	  else
	    {
	      if( one.left() )
		bal = one.left();
	      else
		bal = one.right();
	    }
	}

      if( node.loc() == root() )
	{
	  RBT_TEST_FUNCT( ++ erase_NRT );
	  root() = one.loc();
	}
      else
	{
	  if( parent( node ).left() == node.loc() )
	    {
	      RBT_TEST_FUNCT( ++ erase_PLN );
	      parent( node ).left() = one.loc();
	    }
	  else
	    {
	      RBT_TEST_FUNCT( ++ erase_NPLN );
	      parent( node ).right() = one.loc();
	    }
	}

      one.parent() = node.parent();

      ::swap( one.color(), node.color() );
      
      one = node.loc();
    }
  else
    {
      // one == node
      if( two.loc() )
	{
	  RBT_TEST_FUNCT( ++ erase_ORTT );
	  two.parent() = one.parent();
	}
      else
	{
	  bal = one.loc();
	}
      
      if( node.loc() == root() )
	{
	  RBT_TEST_FUNCT( ++ erase_ORT );
	  root() = two.loc();
	}
      else
	{
	  if( parent( node ).left() == node.loc() )
	    {
	      RBT_TEST_FUNCT( ++ erase_OPLN );
	      parent( node ).left() = two.loc();
	    }
	  else
	    {
	      RBT_TEST_FUNCT( ++ erase_NOPLN );
	      parent( node ).right() = two.loc();
	    }
	}
      
      if( node.loc() == first() )
	{
	  if( ! node.right() )
	    {
	      RBT_TEST_FUNCT( ++ erase_NFNR );
	      // node.left() must be 0 also
	      first() = node.parent();
	      // makes first() == header if node.loc() == root()
	    }
	  else
	    {
	      RBT_TEST_FUNCT( ++ erase_NFR );
	      if( two.loc() )
		first() = minimum( two ).loc();
	      else
		first() = headerLoc;
	    }
	}
      
      if( node.loc() == last() )
	{
	  if( ! node.left() )
	    {
	      RBT_TEST_FUNCT( ++ erase_NENL );
	      last() = node.parent();
	    }
	  else
	    {
	      RBT_TEST_FUNCT( ++ erase_NEL );
	      if( two.loc() )
		last() = maximum( two ).loc();
	      else
		last() = headerLoc;
	    }
	}
    }

  if( one.color() != Red )
    {
      RBT_TEST_FUNCT( ++ erase_1CR );
      while( two.loc() != root() && two.color() == Black )
	{
	  if( two.loc() == parent( two ).left() )
	    {
	      NodeBase tmp( *mgr, parent( two ).right() );

	      if( tmp.color() == Red )
		{
		  RBT_TEST_FUNCT( ++ erase_TPLTMCR );
		  tmp.color() = Black;
		  parent( two ).color() = Red;
		  rotate_left( two.parent() );
		  tmp = parent( two ).right();
		}
	      
	      if( left( tmp ).color() == Black &&
		  right( tmp ).color() == Black )
		{
		  RBT_TEST_FUNCT( ++ erase_TPLLCBRCB );
		  tmp.color() = Red;
		  two = two.parent();
		}
	      else
		{
		  RBT_TEST_FUNCT( ++ erase_TPLNLCBRCB );
		  if( right( tmp ).color() == Black )
		    {
		      RBT_TEST_FUNCT( ++ erase_TPLTRCB );
		      left( tmp ).color() = Black;
		      tmp.color() = Red;
		      rotate_right( tmp.loc() );
		      tmp = parent( two ).right();
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
	      NodeBase tmp( *mgr, parent( two ).left() );

	      if( tmp.color() == Red )
		{
		  RBT_TEST_FUNCT( ++ erase_TMCR );
		  tmp.color() = Black;
		  parent( two ).color() = Red;
		  rotate_right( two.parent() );
		  tmp = parent( two ).left();
		}
	      
	      if( right( tmp ).color() == Black &&
		  left( tmp ).color() == Black )
		{
		  RBT_TEST_FUNCT( ++ erase_RCBLCB );
		  tmp.color() = Red;
		  two = two.parent();
		}
	      else
		{
		  RBT_TEST_FUNCT( ++ erase_NRCBLCB );
		  if( left( tmp ).color() == Black )
		    {
		      RBT_TEST_FUNCT( ++ erase_TLCB );
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

      two.color() = Black;
    }

  -- header().count;
  return( true );
}
#endif

	    
void
RBTreeBase::rotate_right( Loc loc )
{
  NodeBase node( *mgr, loc );
  NodeBase nLeft( *mgr, node.left() );

  node.left() = nLeft.right();

  if( nLeft.right() != 0 )
    {
      RBT_TEST_FUNCT( rr_NLR );
      right( nLeft ).parent() = node.loc();
    }

  nLeft.parent() = node.parent();

  if( node.loc() == root() )
    {
      RBT_TEST_FUNCT(  rr_N_EQ_ROOT );
      root() = nLeft.loc();
    }
  else
    {
      if( node.loc() == parent( node ).right() )
	{
	  RBT_TEST_FUNCT( rr_NPR );
	  parent( node ).right() = nLeft.loc();
	}
      else
	{
	  RBT_TEST_FUNCT( rr_NPL );
	  parent( node ).left() = nLeft.loc();
	}
    }
  
  nLeft.right() = node.loc();
  node.parent() = nLeft.loc();
}

void
RBTreeBase::rotate_left( Loc loc )
{
  NodeBase node( *mgr, loc );
  NodeBase nRight( *mgr, node.right() );

  node.right() = nRight.left();

  if( nRight.left() != 0 )
    {
      RBT_TEST_FUNCT( rl_NRL );
      left( nRight ).parent() = node.loc();
    }

  nRight.parent() = node.parent();

  if( node.loc() == root() )
    {
      RBT_TEST_FUNCT( rl_N_EQ_ROOT );
      root() = nRight.loc();
    }
  else
    {
      if( node.loc() == parent( node ).left() )
	{
	  RBT_TEST_FUNCT( rl_NPL );
	  parent( node ).left() = nRight.loc();
	}
      else
	{
	  RBT_TEST_FUNCT( rl_NPR );
	  parent( node ).right() = nRight.loc();
	}
    }
  nRight.left() = node.loc();
  node.parent() = nRight.loc();
}
  
bool
RBTreeBase::good( void ) const
{
  return( mgr && mgr->good() && errorNum == E_OK );
}

const char *
RBTreeBase::error( void ) const
{
  static Str myErrStr;

  errStr = RBTreeBase::getClassName();

  if( good() )
    {
      myErrStr << ": ok";
    }
  else
    {
      size_t eSize = myErrStr.size();

      if( mgr )
	{
	  if( ! mgr->good() )
	    myErrStr << ": " << mgr->error();
	}
      else
	{
	  myErrStr << ": no mgr";
	}

      switch( errorNum )
	{
	case E_OK:
	  break;

	case E_VERSION:
	  myErrStr << ": bad version: '" << header().version
		   << "' expected: '" << rbTreeVersion << "'";
	  break;

	case E_BADTREEKEY:
	  myErrStr << ": invalid treeKey (already in use).";
	  break;

	default:
	  myErrStr << ": errorNum(" << (int)errorNum << ')';
	  break;
	}

      if( eSize == myErrStr.size() )
        myErrStr << ": unknown error";
    }

  return( myErrStr.c_str() );
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

  
  if( mgr )
    {
      Str pre;
      pre = prefix;
      pre << "mgr:";
      mgr->dumpInfo( dest, pre, false );
    }
  else
    {
      dest << prefix << "mgr:NONE" << '\n';
    }

  if( headerLoc )
    {
      dest << prefix << "headerLoc:       " << headerLoc << '\n'
	   << prefix << "version:         " << header().version << '\n'
	   << prefix << "count:           " << header().count << '\n'
	   << prefix << "first:           " << first() << '\n'
	   << prefix << "last:            " << last() << '\n'
	;
    }
  else
    {
      dest << prefix << "No header!" << '\n';
    }

#if defined( RBT_TEST )
  dest << '\n'
       << prefix << "ins_FIRST:              " << ins_FIRST << '\n'
       << prefix << "ins_NOT_FIRST:          " << ins_NOT_FIRST << '\n'
       << prefix << "ins_NOT_SMALLER:        " << ins_NOT_SMALLER << '\n'
       << prefix << "ins_DUP_FAIL:           " << ins_DUP_FAIL << '\n'
       << prefix << "insNode_P_HEADER:       " << insNode_P_HEADER << '\n'
       << prefix << "insNode_N_LT_P:         " << insNode_N_LT_P << '\n'
       << prefix << "insNode_P_FIRST:        " << insNode_P_FIRST << '\n'
       << prefix << "insNode_P_LAST:         " << insNode_P_LAST << '\n'
       << prefix << "insNode_N_GT_P:         " << insNode_N_GT_P << '\n'
       << prefix << "insNode_BPPR_RED:       " << insNode_BPPR_RED << '\n'
       << prefix << "insNode_B_EQ_BPR:       " << insNode_B_EQ_BPR << '\n'
       << prefix << "insNode_BPPR_BLACK:     " << insNode_BPPR_BLACK << '\n'
       << prefix << "insNode_BPPL_RED:       " <<insNode_BPPL_RED  << '\n'
       << prefix << "insNode_B_EQ_BPL:       " << insNode_B_EQ_BPL << '\n'
       << prefix << "insNode_BPPL_BLACK:     " << insNode_BPPL_BLACK << '\n'
       << prefix << "rr_NLR:                 " << rr_NLR << '\n'
       << prefix << "rr_N_EQ_ROOT:           " << rr_N_EQ_ROOT << '\n'
       << prefix << "rr_NPR:                 " << rr_NPR << '\n'
       << prefix << "rr_NPL:                 " << rr_NPL << '\n'
       << prefix << "rl_NRL:                 " << rl_NRL << '\n'
       << prefix << "rl_N_EQ_ROOT:           " << rl_N_EQ_ROOT << '\n'
       << prefix << "rl_NPL:                 " << rl_NPL << '\n'
       << prefix << "rl_NPR:                 " << rl_NPR << '\n'
       << prefix << "erase_NO_LEFT:          " << erase_NO_LEFT << '\n'
       << prefix << "erase_NO_RIGHT:         " << erase_NO_RIGHT << '\n'
       << prefix << "erase_LEFT_RIGH:        " << erase_LEFT_RIGHT << '\n'
       << prefix << "erase_YNZ_YNZR_NX:      " << erase_YNZ_YNZR_NX << '\n'
       << prefix << "erase_YNZ_YNZR_X:       " << erase_YNZ_YNZR_X << '\n'
       << prefix << "erase_YNZ_YZR:          " << erase_YNZ_YZR << '\n'
       << prefix << "erase_YNZ_Z_EQ_ROOT:    " << erase_YNZ_Z_EQ_ROOT << '\n'
       << prefix << "erase_YNZ_Z_EQ_ZPL:     " << erase_YNZ_Z_EQ_ZPL << '\n'
       << prefix << "erase_YNZ_Z_NE_ZPL:     " << erase_YNZ_Z_NE_ZPL << '\n'
       << prefix << "erase_YZ_X:             " << erase_YZ_X << '\n'
       << prefix << "erase_YZ_NX:            " << erase_YZ_NX << '\n'
       << prefix << "erase_YZ_Z_EQ_ROOT:     " << erase_YZ_Z_EQ_ROOT << '\n'
       << prefix << "erase_YZ_Z_EQ_ZPL:      " << erase_YZ_Z_EQ_ZPL << '\n'
       << prefix << "erase_YZ_Z_NE_ZPL:      " << erase_YZ_Z_NE_ZPL << '\n'
       << prefix << "erase_YZ_Z_EQ_F_NZR:    " << erase_YZ_Z_EQ_F_NZR << '\n'
       << prefix << "erase_YZ_Z_EQ_F_ZR:     " << erase_YZ_Z_EQ_F_ZR << '\n'
       << prefix << "erase_YZ_Z_EQ_L_NZL:    " << erase_YZ_Z_EQ_L_NZL << '\n'
       << prefix << "erase_YZ_Z_EQ_L_ZL:     " << erase_YZ_Z_EQ_L_ZL << '\n'
       << prefix << "erase_Y_BLACK:          " << erase_Y_BLACK << '\n'
       << prefix << "erase_Y_RED:            " << erase_Y_RED << '\n'
       << prefix << "erase_X_EQ_XPL_TRED:    " << erase_X_EQ_XPL_TRED << '\n'
       << prefix << "erase_X_EQ_XPL_TB:      " << erase_X_EQ_XPL_TB << '\n'
       << prefix << "erase_X_EQ_XPL_TLB_TRB: " << erase_X_EQ_XPL_TLB_TRB << '\n'
       << prefix << "erase_X_EQ_XPL_TRB_TL:  " << erase_X_EQ_XPL_TRB_TL << '\n'
       << prefix << "erase_X_EQ_XPL_TRB_NTL: " << erase_X_EQ_XPL_TRB_NTL << '\n'
       << prefix << "erase_X_EQ_XPL_TR:      " << erase_X_EQ_XPL_TR << '\n'
       << prefix << "erase_X_EQ_XPL_NTR:     " << erase_X_EQ_XPL_NTR << '\n'
       << prefix << "erase_X_NE_XPL_TRED:    " << erase_X_NE_XPL_TRED << '\n'
       << prefix << "erase_X_NE_XPL_TB:      " << erase_X_NE_XPL_TB << '\n'
       << prefix << "erase_X_NE_XPL_TLB_TRB: " <<erase_X_NE_XPL_TLB_TRB  << '\n'
       << prefix << "erase_X_NE_XPL_TLB_TR:  " << erase_X_NE_XPL_TLB_TR << '\n'
       << prefix << "erase_X_NE_XPL_TLB_NTR: " << erase_X_NE_XPL_TLB_NTR << '\n'
       << prefix << "erase_X_NE_XPL_TL:      " <<erase_X_NE_XPL_TL  << '\n'
       << prefix << "erase_X_NE_XPL_NTL:     " <<erase_X_NE_XPL_NTL  << '\n'
    ;
#endif
    
  return( dest );

}

ostream &
RBTreeBase::dumpRBTree( ostream & dest, const DumpMethods & meth ) const
{

  dest << "Header: " << setw( 4 ) << headerLoc
	   << (( header().color == Red ) ? ":R" : ":b" )
	   << setw(5) << header().parent
	   << setw(5) << header().left
	   << setw(5) << header().right
	   << '\n'
	;

  if( root() == 0 )
    return( dest );
  
  ConstNodeBase	    node( *mgr, root() );
  long		    level = 0;

  
  for( ; node.loc() != first() ; )
    {
      ConstNodeBase parentNode( *mgr, node.parent() );

      if( node.color() == Red && parentNode.parent() == node.loc() )
	{
	  ++ level;
	  node = node.right();
	}
      else
	{
	  if( node.left() )
	    {
	      ConstNodeBase leftNode( *mgr, node.left() );
	      while( leftNode.right() )
		{
		  ++ level;
		  leftNode = leftNode.right();
		}
	      ++ level;
	      node = leftNode.loc();
	    }
	  else
	    {
	      while( node.loc() == parentNode.left() )
		{
		  -- level;
		  node = parentNode.loc();
		  parentNode = parentNode.parent();
		}
	      -- level;
	      node = parentNode.loc();
	    }
	}
    }

  for( ; node.loc() != headerLoc; )
    {
      dest << setw( 4 ) //(level * 2) + 4 )
	   << node.loc()
	   << (( node.color() == Red ) ? ":R" : ":b" )
	   << setw(2) << level
	   << setw(5) << node.parent()
	   << setw(5) << node.left()
	   << setw(5) << node.right()
	   << ' '
	;
      meth.dumpNode( dest, node.loc() ) << '\n';
      
      if( node.right() )
	{
	  ++ level;
	  for( node = node.right(); node.left(); node = node.left() )
	    ++ level;
	}
      else
	{
	  ConstNodeBase parentNode( *mgr, node.parent() );

	  while( node.loc() == parentNode.right() )
	    {
	      -- level;
	      node = parentNode.loc();
	      parentNode = parentNode.parent();
	    }

	  if( node.right() != parentNode.loc() )
	    {
	      -- level;
	      node = parentNode.loc();
	    }
	}
    }

  return( dest );
}

bool
RBTreeBase::testNode( ostream & dest, const ConstNodeBase & node ) const
{
  
  if( node.left() && ! lessKey( node.left(), node.loc() ) )
    {
      dest << "Error: " << node.left() << ' ' << node.loc() << endl;
      return( false );
    }
	
  if( node.right() && ! lessKey( node.loc(), node.right() ) )
    {
      dest << "Error: " << node.loc() << ' ' << node.right() << endl;
      return( false );
    }

  bool	isRed = ( node.right() || node.left() ?
		  bool( node.color() == Red ) : false );

  {
    ConstNodeBase  p( *mgr, node.loc() );
    
    for( ; p.parent() != headerLoc; p = p.parent() )
      {
	// dest << p.loc() << ":" << ((p.color() == Red) ? "R " : "b " );
	if( isRed && parent( p ).color() == Red )
	  {
	    dest << "\nN(" << p.loc() << ") & p(" << p.parent()
		 << ") both RED." << endl;
	    return( false );
	  }
	isRed = parent( p ).color() == Red;
      }
    // if( p.loc() != node.loc() )
    //  dest << p.loc() << ":" << ((p.color() == Red) ? "R " : "b " ) << endl;

    if( p.loc() != root() )
      {
	dest << "Top parent not ROOT!" << endl;
	return( false );
      }
  }
  long lMaxLevel = 0;
  long lLevel = 0;
  long rMaxLevel = 0;
  long rLevel = 0;
  
  if( node.left() )
    {
      ConstNodeBase	l( *mgr, node.loc() );

      for( ; l.left(); l = l.left() )
	++ lLevel;

      lMaxLevel = lLevel;
      // now at smallest node of sub tree;
      
      for( ; l.loc() != node.loc(); )
	{
	  if( l.right() )
	    {
	      ++ lLevel;
	      for( l = l.right(); l.left(); l = l.left() )
		++ lLevel;
	      lMaxLevel = max( lMaxLevel, lLevel );
	    }
	  else
	    {
	      ConstNodeBase p( *mgr, l.parent() );

	      while( l.loc() == p.right() )
		{
		  -- lLevel;
		  l = p.loc();
		  p = p.parent();
		}

	      if( l.right() != p.loc() )
		{
		  -- lLevel;
		  l = p.loc();
		}
	    }
	}
    }

  if( node.right() )
    {
      ConstNodeBase	r( *mgr, node.loc() );

      for( ; r.right(); r = r.right() )
	++ rLevel;

      rMaxLevel = rLevel;
      //  now at largest node of subtree
      for( ; r.loc() != node.loc(); )
	{
	  if( r.left() )
	    {
	      ++ rLevel;
	      for( r = r.left(); r.right(); r = r.right() )
		++ rLevel;
	      rMaxLevel = max( rMaxLevel, rLevel );
	    }
	  else
	    {
	      ConstNodeBase p( *mgr, r.parent() );

	      while( r.loc() == p.left() )
		{
		  -- rLevel;
		  r = p.loc();
		  p = p.parent();
		}
	      -- rLevel;
	      r = p.loc();
	    }
	}
    }

  if( abs( rMaxLevel - lMaxLevel ) > 4 )
    {
      dest << "TestNode: " << setw(4) << node.loc()
	   << " d: " << setw(3) << abs( rMaxLevel - lMaxLevel )
	   << " l: " << lMaxLevel
	   << " r: " << rMaxLevel
	   << endl;
      return( false );
    }
  return( true );
}

bool
RBTreeBase::testTree( ostream & dest ) const
{
  ConstNodeBase	    node( *mgr, first() );
  ConstNodeBase	    prev( *mgr, 0 );
  
  size_type	    cnt = 0;

  if( header().count == 0 )
    {
      if( first() != headerLoc )
	{
	  dest << "Empty: first not header: " << first() << endl;
	  return( false );
	}
      if( last() != headerLoc )
	{
	  dest << "Empty: last not header: " << last() << endl;
	  return( false );
	}

      if( root() )
	{
	  dest << "Empty: root not 0: " << root() << endl;
	  return( false );
	}

      return( true );
    }
  
  if( node.left() )
    {
      dest << "First has left" << endl;
      return( false );
    }
  
  for( ; node.loc() != headerLoc ; node.next() )
    {
      if( ! testNode( dest, node ) )
	return( false );

      if( prev.loc() )
	{
	  if( ! lessKey( prev.loc(), node.loc() ) )
	    {
	      dest << "Prev Error: " << node.loc()
		   << " " << prev.loc() << endl;
	      return( false );
	    }
	  prev = node.loc();
	}
      
      ++ cnt;
    }

  if( cnt != size() )
    {
      dest << "Size: " << size() << " count: " << cnt << endl;
      return( false );
    }
  
  return( true );
}

bool
RBTreeBase::allTested( void ) const
{
  bool	ret = true;

#if defined( RBT_TEST )
  if( ins_FIRST == 0 ) { ret = false; }
  if( ins_NOT_FIRST == 0 ) { ret = false; }
  if( ins_NOT_SMALLER == 0 ) { ret = false; }
  if( ins_DUP_FAIL == 0 ) { ret = false; }
  
  if( insNode_P_HEADER == 0 ) { ret = false; }
  if( insNode_N_LT_P == 0 ) { ret = false; }
  if( insNode_P_FIRST == 0 ) { ret = false; }
  if( insNode_P_LAST == 0 ) { ret = false; }
  if( insNode_N_GT_P == 0 ) { ret = false; }
  if( insNode_BPPR_RED == 0 ) { ret = false; }
  if( insNode_B_EQ_BPR == 0 ) { ret = false; }
  if( insNode_BPPR_BLACK == 0 ) { ret = false; }
  if( insNode_BPPL_RED == 0 ) { ret = false; }
  if( insNode_B_EQ_BPL == 0 ) { ret = false; }
  if( insNode_BPPL_BLACK == 0 ) { ret = false; }
  
  if( rr_NLR == 0 ) { ret = false; }
  if( rr_N_EQ_ROOT == 0 ) { ret = false; }
  if( rr_NPR == 0 ) { ret = false; }
  if( rr_NPL == 0 ) { ret = false; }
  
  if( rl_NRL == 0 ) { ret = false; }
  if( rl_N_EQ_ROOT == 0 ) { ret = false; }
  if( rl_NPL == 0 ) { ret = false; }
  if( rl_NPR == 0 ) { ret = false; }
  
  if( erase_NO_LEFT == 0 ) { ret = false; }
  if( erase_NO_RIGHT == 0 ) { ret = false; }
  if( erase_LEFT_RIGHT == 0 ) { ret = false; }
  
  if( erase_YNZ_YNZR_NX == 0 ) { ret = false; }
  if( erase_YNZ_YNZR_X == 0 ) { ret = false; }
  if( erase_YNZ_YZR == 0 ) { ret = false; }
  if( erase_YNZ_Z_EQ_ROOT == 0 ) { ret = false; }
  if( erase_YNZ_Z_EQ_ZPL == 0 ) { ret = false; }
  if( erase_YNZ_Z_NE_ZPL == 0 ) { ret = false; }
  if( erase_YZ_X == 0 ) { ret = false; }
  if( erase_YZ_NX == 0 ) { ret = false; }
  if( erase_YZ_Z_EQ_ROOT == 0 ) { ret = false; }
  if( erase_YZ_Z_EQ_ZPL == 0 ) { ret = false; }
  if( erase_YZ_Z_NE_ZPL == 0 ) { ret = false; }
  if( erase_YZ_Z_EQ_F_NZR == 0 ) { ret = false; }
  if( erase_YZ_Z_EQ_F_ZR == 0 ) { ret = false; }
  if( erase_YZ_Z_EQ_L_NZL == 0 ) { ret = false; }
  if( erase_YZ_Z_EQ_L_ZL == 0 ) { ret = false; }
  if( erase_Y_BLACK == 0 ) { ret = false; }
  if( erase_Y_RED == 0 ) { ret = false; }
  if( erase_X_EQ_XPL_TRED == 0 ) { ret = false; }
  if( erase_X_EQ_XPL_TB == 0 ) { ret = false; }
  if( erase_X_EQ_XPL_TLB_TRB == 0 ) { ret = false; }
  if( erase_X_EQ_XPL_TRB_TL == 0 ) { ret = false; }
  if( erase_X_EQ_XPL_TRB_NTL == 0 ) { ret = false; }
  if( erase_X_EQ_XPL_TR == 0 ) { ret = false; }
  if( erase_X_EQ_XPL_NTR == 0 ) { ret = false; }
  if( erase_X_NE_XPL_TRED == 0 ) { ret = false; }
  if( erase_X_NE_XPL_TB == 0 ) { ret = false; }
  if( erase_X_NE_XPL_TLB_TRB == 0 ) { ret = false; }
  if( erase_X_NE_XPL_TLB_TR == 0 ) { ret = false; }
  if( erase_X_NE_XPL_TLB_NTR == 0 ) { ret = false; }
  if( erase_X_NE_XPL_TL == 0 ) { ret = false; }
  if( erase_X_NE_XPL_NTL == 0 ) { ret = false; }
#endif
  
  return( ret );
}

void
RBTreeBase::resetTested( void ) const
{

#if defined( RBT_TEST )
  ins_FIRST = 0;
  ins_NOT_FIRST = 0;
  ins_NOT_SMALLER = 0;
  ins_DUP_FAIL = 0;

  insNode_P_HEADER = 0;
  insNode_N_LT_P = 0;
  insNode_P_FIRST = 0;
  insNode_P_LAST = 0;
  insNode_N_GT_P = 0;
  insNode_BPPR_RED = 0;
  insNode_B_EQ_BPR = 0;
  insNode_BPPR_BLACK = 0;
  insNode_BPPL_RED = 0;
  insNode_B_EQ_BPL = 0;
  insNode_BPPL_BLACK = 0;

  rr_NLR = 0;
  rr_N_EQ_ROOT = 0;
  rr_NPR = 0;
  rr_NPL = 0;

  rl_NRL = 0;
  rl_N_EQ_ROOT = 0;
  rl_NPL = 0;
  rl_NPR = 0;

  erase_NO_LEFT = 0;
  erase_NO_RIGHT = 0;
  erase_LEFT_RIGHT = 0;

  erase_YNZ_YNZR_NX = 0;
  erase_YNZ_YNZR_X = 0;
  erase_YNZ_YZR = 0;
  erase_YNZ_Z_EQ_ROOT = 0;
  erase_YNZ_Z_EQ_ZPL = 0;
  erase_YNZ_Z_NE_ZPL = 0;
  erase_YZ_X = 0;
  erase_YZ_NX = 0;
  erase_YZ_Z_EQ_ROOT = 0;
  erase_YZ_Z_EQ_ZPL = 0;
  erase_YZ_Z_NE_ZPL = 0;
  erase_YZ_Z_EQ_F_NZR = 0;
  erase_YZ_Z_EQ_F_ZR = 0;
  erase_YZ_Z_EQ_L_NZL = 0;
  erase_YZ_Z_EQ_L_ZL = 0;
  erase_Y_BLACK = 0;
  erase_Y_RED = 0;
  erase_X_EQ_XPL_TRED = 0;
  erase_X_EQ_XPL_TB = 0;
  erase_X_EQ_XPL_TLB_TRB = 0;
  erase_X_EQ_XPL_TRB_TL = 0;
  erase_X_EQ_XPL_TRB_NTL = 0;
  erase_X_EQ_XPL_TR = 0;
  erase_X_EQ_XPL_NTR = 0;
  erase_X_NE_XPL_TRED = 0;
  erase_X_NE_XPL_TB = 0;
  erase_X_NE_XPL_TLB_TRB = 0;
  erase_X_NE_XPL_TLB_TR = 0;
  erase_X_NE_XPL_TLB_NTR = 0;
  erase_X_NE_XPL_TL = 0;
  erase_X_NE_XPL_NTL = 0;
#endif
  
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
// Revision 4.1  2001/07/27 00:57:44  houghton
// Change Major Version to 4
//
// Revision 2.8  2001/07/27 00:47:31  houghton
// *** empty log message ***
//
// Revision 2.7  1999/07/17 08:39:37  houghton
// Improved Error output.
//
// Revision 2.6  1997/10/01 14:03:29  houghton
// Chaged to reserve 'keys' set.
// Changed to use portable multi platform types.
//
// Revision 2.5  1997/08/18 10:24:10  houghton
// Port(Sun5): had to add static errStr to be used by template sub
//     classes (DBTree). The sun compiler gets a dup symbol error.
//
// Revision 2.4  1997/07/25 13:48:40  houghton
// Cleanup.
//
// Revision 2.3  1997/07/14 10:40:43  houghton
// Port(AIX): added a bool constructor for the '? :' operator.
//
// Revision 2.2  1997/07/13 11:33:36  houghton
// Cleanup.
// Changed to use MultiMemOffset.
// Added testing & debuging methods().
// Major rework of erase().
//
// Revision 2.1  1997/06/05 11:29:13  houghton
// Initial Version.
//
//
