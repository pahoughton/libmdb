#ifndef _MultiMemOffset_hh_
#define _MultiMemOffset_hh_
//
// File:        MultiMemOffset.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     01/09/95 09:47
//
// Revision History:
//
// 
// $Log$
// Revision 1.2  1995/11/05 16:23:55  houghton
// Added Old Clue classes
//
// Revision 1.1  1995/02/13  16:08:51  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <iostream>
#include <cstddef>
#include <sys/types.h>

class MultiMemOffset
{

public:

  MultiMemOffset();

  virtual ~MultiMemOffset() {};

  virtual off_t	    getMem( size_t size );
  virtual void 	    freeMem( off_t offset );

  virtual void *    getAddr( off_t offset );
  virtual void *    getBase( void );

  virtual ostream & 	getStats( ostream & dest ) const;

  virtual const char * 	getClassName( void ) const;
  virtual bool	        good( void ) const;
  virtual const char *	error( void ) const;

  friend inline ostream & operator<<( ostream & dest, const MultiMemOffset & mmo );
  
protected:

private:

  MultiMemOffset( const MultiMemOffset & copyFrom );
  MultiMemOffset & operator=( const MultiMemOffset & assignFrom );

  int	oserrno;
  
};

extern MultiMemOffset MultiMemOffsetMalloc;

//
// Inline methods
//

inline
MultiMemOffset::MultiMemOffset( void )
{
  oserrno = 0;
}


inline
ostream &
operator<<( ostream & dest, const MultiMemOffset & mmo )
{
  return( mmo.getStats( dest ) );
}



#endif // ! def _MultiMemOffset_hh_ 
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

