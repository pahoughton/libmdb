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
// Revision 2.3  1997/09/17 16:56:09  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.2  1997/04/04 01:36:54  houghton
// Changed include stddef to stddef.h
//
// Revision 2.1  1995/11/10 12:42:36  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:36  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:51  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <StlUtilsConfig.hh>

#include <stddef.h>

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
