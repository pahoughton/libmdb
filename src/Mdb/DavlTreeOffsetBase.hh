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
// Revision 2.3  1997/06/25 12:54:15  houghton
// Added virtual getDataBase().
// Removed dataBaseAddr.
//
// Revision 2.2  1997/06/19 12:00:50  houghton
// Changed off_t to Loc
//
// Revision 2.1  1995/11/10 12:42:12  houghton
// Change to Version 2
//
// Revision 1.5  1995/11/05  16:32:29  houghton
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

  typedef long		EffDate;
  
protected:

public:

  // these really should be protected, but the AIX
  // compiler barfs
  
  struct HistNode : Node
  {
    Loc   hist;
  };

  struct HistData
  {
    EffDate when;
    Loc	    next;
    long    deleted;
  };

protected:
  
  DavlTreeOffsetBase();

  virtual caddr_t   	getBaseData( void ) = 0;
  virtual const caddr_t getBaseData( void ) const = 0;
  
  void	    	initHist( Loc histOffset );
  void 	    	insertHist( Loc * firstHist, Loc newHist );
  Loc	    	findHist( Loc histOffset, EffDate when );  

  bool	    	walkTree( Loc root, EffDate when );
  bool	    	walkTree( Loc root, EffDate when, void * closure );
  
  bool	    	walkTreeHist( Loc root, EffDate when );
  bool	    	walkTreeHist( Loc root, EffDate when, void * closure );

  bool	    	walkKeyHist( Loc node, EffDate when );
  bool	    	walkKeyHist( Loc node, EffDate when, void * closure );
  
  void 	    	trimTree( Loc * root, EffDate when );
  void 	    	trimTree( Loc * root, EffDate when, void * closure );
  
  virtual bool 	walkHistAction( Loc root, Loc hist ) = 0;
  virtual bool 	walkHistAction( Loc root, Loc hist, void * closure ) = 0;

  virtual bool 	walkAllHistAction( Loc root, Loc hist ) = 0;
  virtual bool 	walkAllHistAction( Loc root, Loc hist, void * closure ) = 0;

  virtual bool	walkKeyHistAction( Loc root, Loc hist ) = 0;
  virtual bool  walkKeyHistAction( Loc root, Loc hist, void * closure ) = 0;
  
  virtual void	trimHistAction( Loc root, Loc hist ) = 0;
  virtual void	trimHistAction( Loc root, Loc hist, void * closure ) = 0;

  virtual void	destroyHistAction( Loc root, Loc hist ) = 0;
  virtual void	destroyHistAction( Loc root, Loc hist, void * closure ) = 0;

private:

  HistData *	hist( Loc data );
  HistNode * 	node( Loc root );
  
  bool	 	walkNode( Loc	root );  
  bool	 	walkNode( Loc	root, void * closure );

  short	    	trimNode( Loc * root, EffDate when );
  short	    	trimNode( Loc * root, EffDate when, void * closure );
  
  void	    	destroyNode( Loc root );
  void	    	destroyNode( Loc root, void * closure );

  DavlTreeOffsetBase( const DavlTreeOffsetBase & copyFrom );
  DavlTreeOffsetBase & operator=( const DavlTreeOffsetBase & assignFrom );

  bool	    walkHist;
  EffDate    walkWhen;
  
  bool 	    trimDelete;
};


//
// Inline methods
//

inline
DavlTreeOffsetBase::DavlTreeOffsetBase( void )
{
}

inline
DavlTreeOffsetBase::HistData *
DavlTreeOffsetBase::hist( Loc data )
{
  return( (HistData *)( getBaseData() + data ) );
}

inline
DavlTreeOffsetBase::HistNode *
DavlTreeOffsetBase::node( Loc root )
{
  return( (HistNode *)( getBase() + root ) );
}

inline
void
DavlTreeOffsetBase::initHist( Loc histOffset )
{
  hist(histOffset)->when = 0;
  hist(histOffset)->deleted = 0;
  hist(histOffset)->next = 0;
}

inline
DavlTreeOffsetBase::Loc
DavlTreeOffsetBase::findHist(
  Loc	    histOffset,
  EffDate    when
  )
{
  Loc h = histOffset;
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
DavlTreeOffsetBase::walkTree( Loc root, EffDate when )
{
  walkWhen = when;
  walkHist = false;

  return( AvlTreeOffsetBase::walkTree( root ) );
}

inline
bool
DavlTreeOffsetBase::walkTree( Loc root, EffDate when, void * closure )
{
  walkWhen = when;
  walkHist = false;

  return( AvlTreeOffsetBase::walkTree( root, closure ) );
}

inline
bool
DavlTreeOffsetBase::walkTreeHist( Loc root, EffDate when )
{
  walkWhen = when;
  walkHist = true;

  return( AvlTreeOffsetBase::walkTree( root ) );
}

inline
bool
DavlTreeOffsetBase::walkTreeHist( Loc root, EffDate when, void * closure )
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
