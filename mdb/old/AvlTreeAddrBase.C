//
// File:        AvlTreeAddrBase.C
// Desc:        
//              
//  All the source for the Avl methods is in AvlTreeMethods.INC
//
//  This was done because both AvlTreeAddrBase and AvlTreeOffsetBase
//  use the exact same alogrithims. The only differnces are the
//  ClassName, Node data type, and the fact that AvlTreeOffsetBase needs
//  to convert a node into an address (a node in that class is an offset).
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     12/14/94 07:17 
//
// Revision History:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:42  houghton
// Change Major Version to 4
//
// Revision 2.2  1995/12/04 11:19:08  houghton
// Changed to eliminate compile warnings.
//
// Revision 2.1  1995/11/10  12:41:59  houghton
// Change to Version 2
//
// Revision 1.1  1995/02/13  16:08:30  houghton
// New Style Avl an memory management. Many New Classes
//
//

const char AvlTreeAddrBaseRcsId[] =
"$Id$";

#include "AvlTreeAddrBase.hh"

#define ClassName   	AvlTreeAddrBase
#define NodeDataType	AvlTreeAddrBase::Node *
#define NODE( n )   	( n )

#define AVL_ADDRESSES	// allows const walks (offset cant do this)

#include "AvlTreeMethods.INC"

//
// This software is a modificed version of Brad Appleton's
// 'c' avl library.
//
// It is public domain.
//
