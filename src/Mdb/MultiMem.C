//
// File:        MultiMem.C
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     01/31/95 20:19 
//
// Revision History:
//
// $Log$
// Revision 1.3  1995/11/05 16:32:35  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:50  houghton
// New Style Avl an memory management. Many New Classes
//
//
static const char * RcsId =
"$Id$";

#include "MultiMem.hh"
#include <cstdlib>

void *
MultiMem::getMem( size_t size )
{
  return( malloc( size ) );
}

void 
MultiMem::freeMem( void * buffer )
{
  free( buffer );
}

MultiMem MultiMemMalloc;

//
//              This software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1994 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//
