//
// File:        DavlTreeOffsetBase.C
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     01/13/95 07:14 
//
// Revision History:
//
// $Log$
// Revision 2.3  1997/06/19 12:00:46  houghton
// Changed off_t to Loc
//
// Revision 2.2  1995/12/04 11:19:17  houghton
// Changed to eliminate compile warnings.
//
// Revision 2.1  1995/11/10  12:42:10  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:28  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:36  houghton
// New Style Avl an memory management. Many New Classes
//
//
const char DavlTreeOffsetBaseRcsId[] =
"$Id$";

#include "DavlTreeOffsetBase.hh"

void
DavlTreeOffsetBase::insertHist( Loc * firstHist, Loc newHist )
{
  Loc * h = firstHist;
  for( ;
      *h != 0 && hist( *h )->when > hist( newHist )->when;
      h = &( hist(*h)->next ) );
  
  hist( newHist )->next = *h;
  *h = newHist;
}

bool
DavlTreeOffsetBase::walkKeyHist( Loc root, EffDate when )
{
  for( Loc h = node(root)->hist;
      h != 0;
      h = hist( h )->next )
    {
      if( hist(h)->when < when )
	{
	  if( walkKeyHistAction( root, h ) )
	    {
	      return( true );
	    }
	}
    }
  return( false );
}

bool
DavlTreeOffsetBase::walkKeyHist( Loc root, EffDate when, void * closure )
{
  for( Loc h = node(root)->hist;
      h != 0;
      h = hist( h )->next )
    {
      if( hist(h)->when < when )
	{
	  if( walkKeyHistAction( root, h, closure ) )
	    {
	      return( true );
	    }
	}
    }
  return( false );
}

  
void
DavlTreeOffsetBase::trimTree( Loc * root, EffDate when )
{

  //
  // I hate using globals, but it's the only solution I could
  // come up with!
  //
  
  trimDelete = false;

  //
  // this STINKS! I have to keep triming the tree until
  // I have trimmed the entire tree without deleting a
  // node
  //
  for(;;)
    {
      trimNode( root, when );
      if( trimDelete == false )
	{
	  return;
	}
      trimDelete = false;
    }
}
  
void
DavlTreeOffsetBase::trimTree( Loc * root, EffDate when, void * closure )
{

  //
  // I hate using globals, but it's the only solution I could
  // come up with!
  //
  
  trimDelete = false;

  //
  // this STINKS! I have to keep triming the tree until
  // I have trimmed the entire tree without deleting a
  // node
  //
  for(;;)
    {
      trimNode( root, when, closure );
      if( trimDelete == false )
	{
	  return;
	}
      trimDelete = false;
    }
}

bool
DavlTreeOffsetBase::walkNode( Loc root )
{
  for( Loc h = node(root)->hist;
      h != 0;
      h = hist( h )->next )
    {
      if( hist(h)->when < walkWhen )
	{
	  if( walkHist )
	    {
	      if( walkAllHistAction( root, h ) )
		{
		  return( true );
		}
	    }
	  else
	    {
	      if( ! hist(h)->deleted )
		{
		  return( walkHistAction( root, h ) );
		}
	      else
		{
		  return( false );
		}
	    }	  
	}
    }
  return( false );
}

bool
DavlTreeOffsetBase::walkNode( Loc root, void * closure )
{
  for( Loc h = node(root)->hist;
      h != 0;
      h = hist( h )->next )
    {
      if( hist(h)->when < walkWhen )
	{
	  if( walkHist )
	    {
	      if( walkAllHistAction( root, h, closure ) )
		{
		  return( true );
		}
	    }
	  else
	    {
	      if( ! hist(h)->deleted )
		{
		  return( walkHistAction( root, h, closure ) );
		}
	      else
		{
		  return( false );
		}
	    }	  
	}
    }
  return( false );
}

			  
short
DavlTreeOffsetBase::trimNode( Loc * root, EffDate when )
{

  short	    dec = 0;

  if( node(*root)->subTree[LEFT] != 0 )
    {
      dec = -trimNode( &( node(*root)->subTree[LEFT] ), when );
    }

  if( dec == 0 && node( *root )->subTree[RIGHT] != 0 )
    {
      dec = trimNode( &( node( *root )->subTree[RIGHT] ), when );
    }

  if( dec == 0 )
    {
      Loc * h = &(node( *root )->hist);
      for( ;
	  *h != 0 && hist(*h)->when > when;
	  h = &( hist(*h)->next ) );
	
      if( *h != 0 && hist(*h)->next != 0 )
	{
	  for( h = &( hist(*h)->next);
	      *h != 0; )
	    {
	      Loc * n = (& hist(*h)->next);

	      trimHistAction( *root, *h );

	      *h = 0;

	      h = n;
	    }

	  if( node( *root )->hist == 0 )
	    {
	      //
	      // All the history is gone, so we delete the
	      // node from the tree
	      //

	      trimDelete = true;
	      
	      Loc 	origRoot = *root;

	      switch( getNodeType( *root ) )
		{
		case LEAF:
		  *root = 0;
		  return( 1 );

		case RIGHT_BRANCH:
		  *root = node( *root )->subTree[ RIGHT ];
		  return( 1 );

		case LEFT_BRANCH:
		  *root = node( *root )->subTree[ LEFT ];
		  return( 1 );

		case TREE:
		{
		  Loc key 	 = *root;
		  dec = deleteNode( &( node( *root )->subTree[ RIGHT ] ), &key, -1 );

		  node( key )->subTree[ LEFT ]  = node( *root )->subTree[ LEFT ];
		  node( key )->subTree[ RIGHT ] = node( *root )->subTree[ RIGHT ];
		  node( key )->bal	    	= node( *root )->bal;
		  
		  *root = key;
		}
		  break;

		default:	    // can't happen
		  ;
		}
	      
	      trimHistAction( origRoot, 0 );
	    }
	}
    }

  if( dec )
    {
      node(*root)->bal -= dec;
      if( node(*root)->bal )
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

short
DavlTreeOffsetBase::trimNode( Loc * root, EffDate when, void * closure )
{

  short	    dec = 0;

  if( node(*root)->subTree[LEFT] != 0 )
    {
      dec = -trimNode( &( node(*root)->subTree[LEFT] ), when, closure );
    }

  if( dec == 0 && node( *root )->subTree[RIGHT] != 0 )
    {
      dec = trimNode( &( node( *root )->subTree[RIGHT] ), when, closure );
    }

  if( dec == 0 )
    {
      Loc * h = &(node( *root )->hist);
      for( ;
	  *h != 0 && hist(*h)->when > when;
	  h = &( hist(*h)->next ) );
	
      if( *h != 0 && hist(*h)->next != 0 )
	{
	  for( h = &( hist(*h)->next);
	      *h != 0; )
	    {
	      Loc * n = (& hist(*h)->next);

	      trimHistAction( *root, *h, closure );

	      *h = 0;

	      h = n;
	    }

	  if( node( *root )->hist == 0 )
	    {
	      //
	      // All the history is gone, so we delete the
	      // node from the tree
	      //

	      trimDelete = true;
	      
	      Loc origRoot = *root;

	      switch( getNodeType( *root ) )
		{
		case LEAF:
		  *root = 0;
		  return( 1 );

		case RIGHT_BRANCH:
		  *root = node( *root )->subTree[ RIGHT ];
		  return( 1 );

		case LEFT_BRANCH:
		  *root = node( *root )->subTree[ LEFT ];
		  return( 1 );

		case TREE:
		{
		  Loc key = *root;

		  dec = deleteNode( &( node( *root )->subTree[ RIGHT ] ), &key, -1 );

		  node( key )->subTree[ LEFT ]  = node( *root )->subTree[ LEFT ];
		  node( key )->subTree[ RIGHT ] = node( *root )->subTree[ RIGHT ];
		  node( key )->bal	    	= node( *root )->bal;
		  
		  *root = key;
		}
		  break;

		default:	    // can't happen
		  ;
		}
	      trimHistAction( origRoot, 0, closure );
	    }
	}
    }

  if( dec )
    {
      node(*root)->bal -= dec;
      if( node(*root)->bal )
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

void
DavlTreeOffsetBase::destroyNode( Loc root )
{
  for( Loc h = node(root)->hist;
      h != 0;
      h = hist( h )->next )
    {
      destroyHistAction( root,  h );
    }
  destroyHistAction( root, 0 );
}
	  
void
DavlTreeOffsetBase::destroyNode( Loc root, void * closure )
{
  for( Loc h = node(root)->hist;
      h != 0;
      h = hist( h )->next )
    {
      destroyHistAction( root, h, closure );
    }
  destroyHistAction( root, 0, closure );
}
	  
	  
    
  
    

  

  
  


//
//              this software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1994 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//
