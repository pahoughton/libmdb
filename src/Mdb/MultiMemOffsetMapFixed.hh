#ifndef _MultiMemOffsetMapFixed_hh_
#define _MultiMemOffsetMapFixed_hh_
//
// File:        MultiMemOffsetMapFixed.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     01/09/95 10:07
//
// Revision History:
//
// 
// $Log$
// Revision 2.1  1995/11/10 12:42:39  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:38  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:52  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <ClueConfig.hh>

#include <MultiMemOffset.hh>
#include <MapMemFixedDynamic.hh>

class MultiMemOffsetMapFixed : public MultiMemOffset
{

public:

  MultiMemOffsetMapFixed( const char * 	    fileName,
			  ios::open_mode    mode = ios::in );

  MultiMemOffsetMapFixed( const char *	    fileName,
			  size_t    	    recSize,
			  size_t    	    numRec = 0 );

  virtual ~MultiMemOffsetMapFixed( void ) { };

  virtual off_t	    getMem( size_t size = 0 );
  virtual void 	    freeMem( off_t offset );

  virtual void *    getAddr( off_t offset );
  virtual void *    getBase( void );

  MapMemFixedDynamic &     getMap( void );

  virtual ostream & 	getStats( ostream & dest ) const;

  virtual const char *	getClassName( void ) const;
  virtual bool	    	good( void ) const;
  virtual const char *	error( void ) const;

  friend inline ostream & operator<<( ostream & dest, const MultiMemOffsetMapFixed & mmo );
  
protected:

private:

  MultiMemOffsetMapFixed( const MultiMemOffsetMapFixed & copyFrom );
  MultiMemOffsetMapFixed & operator=( const MultiMemOffsetMapFixed & assignFrom );

  MapMemFixedDynamic	    mem;
  
};


//
// Inline methods
//

inline
MultiMemOffsetMapFixed::MultiMemOffsetMapFixed(
  const char * 	    fileName,
  ios::open_mode    mode
  )
  : mem( fileName, mode )
{
  ;
}

inline
MultiMemOffsetMapFixed::MultiMemOffsetMapFixed(
  const char * 	    fileName,
  size_t    	    recSize,
  size_t    	    numRecs
  )
  : mem( fileName, recSize, numRecs )
{
  ;
}

inline
off_t
MultiMemOffsetMapFixed::getMem( size_t size )
{
  return( mem.getMem( size ) );
}

inline
void 
MultiMemOffsetMapFixed::freeMem( off_t offset )
{
  mem.freeMem( offset );
}

inline
void *
MultiMemOffsetMapFixed::getAddr( off_t offset )
{
  return( mem.getAddr( offset ) );
}

inline
void *
MultiMemOffsetMapFixed::getBase( void )
{
  return( mem.getBase() );
}

inline
MapMemFixedDynamic &
MultiMemOffsetMapFixed::getMap( void )
{
  return( mem );
}

inline
const char *
MultiMemOffsetMapFixed::getClassName( void ) const
{
  return( "MultiMemOffsetMapFixed" );
}

inline
bool
MultiMemOffsetMapFixed::good( void ) const
{
  return( mem.good() );
}

inline
ostream &
operator<<( ostream & dest, const MultiMemOffsetMapFixed & mmof )
{
  return( mmof.getStats( dest ) );
}

#endif // ! def _MultiMemOffsetMapFixed_hh_ 
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
