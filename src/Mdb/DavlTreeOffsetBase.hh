#ifndef _DavlTreeOffsetBase_hh_
#define _DavlTreeOffsetBase_hh_
//
// File:        DavlTreeOffsetBase.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     01/10/95 10:41
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <MdbConfig.hh>
#include <AvlTreeOffsetBase.hh>
#include <climits>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class DavlTreeOffsetBase : public AvlTreeOffsetBase
{

public:

  typedef long		EffDate;

  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  
  static const ClassVersion version;

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

  virtual Addr   	getBaseData( void ) = 0;
  virtual const Addr	getBaseData( void ) const = 0;
  
  inline void	initHist( Loc histOffset );
  void 	    	insertHist( Loc * firstHist, Loc newHist );
  inline Loc	findHist( Loc histOffset, EffDate when ) const;  

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
  virtual bool 	walkHistAction( Loc root, Loc hist, void * closure ) const = 0;

  virtual bool 	walkAllHistAction( Loc root, Loc hist ) = 0;
  virtual bool 	walkAllHistAction( Loc root, Loc hist, void * closure ) = 0;
  virtual bool 	walkAllHistAction( Loc root, Loc hist, void * closure ) const = 0;

  virtual bool	walkKeyHistAction( Loc root, Loc hist ) = 0;
  virtual bool  walkKeyHistAction( Loc root, Loc hist, void * closure ) = 0;
  
  virtual void	trimHistAction( Loc root, Loc hist ) = 0;
  virtual void	trimHistAction( Loc root, Loc hist, void * closure ) = 0;

  virtual void	destroyHistAction( Loc root, Loc hist ) = 0;
  virtual void	destroyHistAction( Loc root, Loc hist, void * closure ) = 0;

private:

  inline HistData *	hist( Loc data );
  inline HistNode * 	node( Loc root );
  
  inline const HistData *	hist( Loc data ) const;
  inline const HistNode * 	node( Loc root ) const;
  
  bool	 	walkNode( Loc	root );  
  bool	 	walkNode( Loc	root, void * closure );
  bool	 	walkNode( Loc	root, void * closure ) const;

  short	    	trimNode( Loc * root, EffDate when );
  short	    	trimNode( Loc * root, EffDate when, void * closure );
  
  void	    	destroyNode( Loc root );
  void	    	destroyNode( Loc root, void * closure );

  DavlTreeOffsetBase( const DavlTreeOffsetBase & copyFrom );
  DavlTreeOffsetBase & operator=( const DavlTreeOffsetBase & assignFrom );

  bool	    walkHist;
  EffDate   walkWhen;
  
  bool 	    trimDelete;
};


#if !defined( inline )
#include <DavlTreeOffsetBase.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	DavlTreeOffsetBase	class
//
//  Constructors:
//
//  	DavlTreeOffsetBase( );
//
//  Destructors:
//
//  Public Interface:
//
//	virtual ostream &
//	write( ostream & dest ) const;
//	    write the data for this class in binary form to the ostream.
//
//	virtual istream &
//	read( istream & src );
//	    read the data in binary form from the istream. It is
//	    assumed it stream is correctly posistioned and the data
//	    was written to the istream with 'write( ostream & )'
//
//	virtual ostream &
//	toStream( ostream & dest ) const;
//	    output class as a string to dest (used by operator <<)
//
//	virtual istream &
//	fromStream( istream & src );
//	    Set this class be reading a string representation from
//	    src. Returns src.
//
//  	virtual Bool
//  	good( void ) const;
//  	    Return true if there are no detected errors associated
//  	    with this class, otherwise false.
//
//  	virtual const char *
//  	error( void ) const;
//  	    Return a string description of the state of the class.
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. DavlTreeOffsetBase )
//
//  	virtual const char *
//  	getVersion( bool withPrjVer = true ) const;
//  	    Return the version string of this class.
//
//	virtual ostream &
//	dumpInfo( ostream & dest, const char * prefix, bool showVer );
//	    output detail info to dest. Includes instance variable
//	    values, state info & version info.
//
//	static const ClassVersion version
//	    Class and project version information. (see ClassVersion.hh)
//
//  Protected Interface:
//
//  Private Methods:
//
//  Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const DavlTreeOffsetBase & src );
//
//	istream &
//	operator >> ( istream & src, DavlTreeOffsetBase & dest );
//
// Example:
//
// See Also:
//
// Files:
//
// Documented Ver:
//
// Tested Ver:
//
// Copyright:
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
// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.5  1997/07/19 10:22:02  houghton
// Bug-Fix: changed caddr_t to Addr.
//
// Revision 2.4  1997/07/13 11:09:27  houghton
// Cleanup.
// Added const walk methods.
//
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
#endif // ! def _DavlTreeOffsetBase_hh_ 
