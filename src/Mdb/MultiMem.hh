#ifndef _MultiMem_hh_
#define _MultiMem_hh_
//
// File:        MultiMem.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     05/13/95 16:01
//
// Revision History:
//
// 
// $Log$
// Revision 1.2  1995/11/05 16:23:53  houghton
// Added Old Clue classes
//
// Revision 1.1  1995/02/13  16:08:51  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <stddef>

class MultiMem
{

public:

  MultiMem() { };
  virtual ~MultiMem() { };
  
  virtual void *    getMem( size_t size );
  virtual void 	    freeMem( void * buffer );
  
protected:

private:

  MultiMem( const MultiMem & copyFrom );
  MultiMem & operator=( const MultiMem & assignFrom );

};

extern MultiMem MultiMemMalloc;

//
// Inline methods
//




#endif // ! def _MultiMem_hh_ 
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
