//
// File:        AvlTreeOffsetBase.C
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
// Created:     12/13/94 09:49 
//
// Revision History:
//
// $Log$
// Revision 2.2  1995/12/04 11:19:17  houghton
// Changed to eliminate compile warnings.
//
// Revision 2.1  1995/11/10  12:42:03  houghton
// Change to Version 2
//
// Revision 1.1  1995/02/13  16:08:32  houghton
// New Style Avl an memory management. Many New Classes
//
//
const char AvlTreeOffsetBaseRcsId[] =
"$Id$";

#include "AvlTreeOffsetBase.hh"

#define ClassName   	AvlTreeOffsetBase
#define NodeDataType	off_t
#define NODE( n )   	node( n )

#include "AvlTreeMethods.INC"

//
// This software is a modificed version of Brad Appleton's
// 'c' avl library.
//
// It is public domain.
//
