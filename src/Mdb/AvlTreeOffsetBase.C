//
// File:        AvlTreeOffsetBase.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for AvlTreeOffsetBase
//
//  The source for the Avl methods is in AvlTreeMethods.INC
//
//  This was done because both AvlTreeAddrBase and AvlTreeOffsetBase
//  use the exact same alogrithims. The only differnces are the
//  ClassName, Node data type, and the fact that AvlTreeOffsetBase needs
//  to convert a node into an address.
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     12/13/94 09:49 
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "AvlTreeOffsetBase.hh"

#if defined( MDB_DEBUG )
#include "AvlTreeOffsetBase.ii"
#endif

MDB_VERSION(
  AvlTreeOffsetBase,
  "$Id$");


#define ClassName   	AvlTreeOffsetBase
#define NodeDataType	off_t
#define NODE( n )   	node( n )

#include "AvlTreeMethods.INC"

AvlTreeOffsetBase::AvlTreeOffsetBase( void )
{
}

AvlTreeOffsetBase::AvlTreeOffsetBase(
  const AvlTreeOffsetBase & CLUE_UNUSED( from )
  )
{
}

const char *
AvlTreeOffsetBase::getClassName( void ) const
{
  return( "AvlTreeOffsetBase" );
}

const char *
AvlTreeOffsetBase::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}

// Revision Log:
//
// $Log$
// Revision 2.3  1997/07/13 11:01:49  houghton
// Cleanup.
//
// Revision 2.2  1995/12/04 11:19:17  houghton
// Changed to eliminate compile warnings.
//
// Revision 2.1  1995/11/10  12:42:03  houghton
// Change to Version 2
//
// Revision 1.1  1995/02/13  16:08:32  houghton
// New Style Avl an memory management. Many New Classes
//
