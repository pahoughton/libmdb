#ifndef _DavlTreeOffsetBase_hh_
#define _DavlTreeOffsetBase_hh_
//
// File:        DavlTreeOffsetBase.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     01/10/95 10:41
//
// Revision History:
//
// 
// $Log$
// Revision 1.5  1995/11/05 16:32:29  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:37  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <ClueConfig.hh>

#include <AvlTreeOffsetBase.hh>
#include <climits>

class DavlTreeOffsetBase : public AvlTreeOffsetBase
{

public:

protected:

public:

  // these really should be protected, but the AIX
  // compiler barfs
  
  struct HistNode : Node
  {
    off_t   hist;
  };

  struct HistData
  {
    time_t  when;
    off_t   next;
    bool    deleted;
  };

protected:
  
  DavlTreeOffsetBase();

  caddr_t   	getBaseData( void );
  caddr_t 	setBaseData( void * newBase );
  
  void	    	initHist( off_t histOffset );
  void 	    	insertHist( off_t * firstHist, off_t newHist );
  off_t	    	findHist( off_t histOffset, time_t when );  

  bool	    	walkTree( off_t root, time_t when );
  bool	    	walkTree( off_t root, time_t when, void * closure );
  
  bool	    	walkTreeHist( off_t root, time_t when );
  bool	    	walkTreeHist( off_t root, time_t when, void * closure );

  bool	    	walkKeyHist( off_t node, time_t when );
  bool	    	walkKeyHist( off_t node, time_t when, void * closure );
  
  void 	    	trimTree( off_t * root, time_t when );
  void 	    	trimTree( off_t * root, time_t when, void * closure );
  
  virtual bool 	walkHistAction( off_t root, off_t hist ) = 0;
  virtual bool 	walkHistAction( off_t root, off_t hist, void * closure ) = 0;

  virtual bool 	walkAllHistAction( off_t root, off_t hist ) = 0;
  virtual bool 	walkAllHistAction( off_t root, off_t hist, void * closure ) = 0;

  virtual bool	walkKeyHistAction( off_t root, off_t hist ) = 0;
  virtual bool  walkKeyHistAction( off_t root, off_t hist, void * closure ) = 0;
  
  virtual void	trimHistAction( off_t root, off_t hist ) = 0;
  virtual void	trimHistAction( off_t root, off_t hist, void * closure ) = 0;

  virtual void	destroyHistAction( off_t root, off_t hist ) = 0;
  virtual void	destroyHistAction( off_t root, off_t hist, void * closure ) = 0;

private:

  HistData *	hist( off_t data );
  HistNode * 	node( off_t root );
  
  bool	 	walkNode( off_t	root );  
  bool	 	walkNode( off_t	root, void * closure );

  short	    	trimNode( off_t * root, time_t when );
  short	    	trimNode( off_t * root, time_t when, void * closure );
  
  void	    	destroyNode( off_t root );
  void	    	destroyNode( off_t root, void * closure );

  DavlTreeOffsetBase( const DavlTreeOffsetBase & copyFrom );
  DavlTreeOffsetBase & operator=( const DavlTreeOffsetBase & assignFrom );

  caddr_t   baseDataAddr;

  bool	    walkHist;
  time_t    walkWhen;
  
  bool 	    trimDelete;
};


//
// Inline methods
//

inline
DavlTreeOffsetBase::DavlTreeOffsetBase( void )
{
  baseDataAddr = 0;
}

inline
caddr_t 
DavlTreeOffsetBase::getBaseData( void )
{
  return( baseDataAddr );
}

inline
caddr_t
DavlTreeOffsetBase::setBaseData( void * newBase )
{
  caddr_t    oldBase = baseDataAddr;
  baseDataAddr = (caddr_t)newBase;
  return( oldBase );
}

inline
DavlTreeOffsetBase::HistData *
DavlTreeOffsetBase::hist( off_t data )
{
  return( (HistData *)( baseDataAddr + data ) );
}

inline
DavlTreeOffsetBase::HistNode *
DavlTreeOffsetBase::node( off_t root )
{
  return( (HistNode *)( getBase() + root ) );
}

inline
void
DavlTreeOffsetBase::initHist( off_t histOffset )
{
  hist(histOffset)->when = 0;
  hist(histOffset)->deleted = 0;
  hist(histOffset)->next = 0;
}

inline
off_t
DavlTreeOffsetBase::findHist(
  off_t	    histOffset,
  time_t    when
  )
{
  off_t h = histOffset;
  for( ; h != 0; h = hist(h)->next )
    {
      if( hist(h)->when <= when )
	{
	  if( hist(h)->deleted )
	    {
	      return( 0 );
	    }
	  return( h );
	}
    }
  return( h );
}

inline
bool
DavlTreeOffsetBase::walkTree( off_t root, time_t when )
{
  walkWhen = when;
  walkHist = false;

  return( AvlTreeOffsetBase::walkTree( root ) );
}

inline
bool
DavlTreeOffsetBase::walkTree( off_t root, time_t when, void * closure )
{
  walkWhen = when;
  walkHist = false;

  return( AvlTreeOffsetBase::walkTree( root, closure ) );
}

inline
bool
DavlTreeOffsetBase::walkTreeHist( off_t root, time_t when )
{
  walkWhen = when;
  walkHist = true;

  return( AvlTreeOffsetBase::walkTree( root ) );
}

inline
bool
DavlTreeOffsetBase::walkTreeHist( off_t root, time_t when, void * closure )
{
  walkWhen = when;
  walkHist = true;

  return( AvlTreeOffsetBase::walkTree( root, closure ) );
}




#endif // ! def _DavlTreeOffsetBase_hh_ 
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
