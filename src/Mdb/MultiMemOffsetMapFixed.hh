#ifndef _MultiMemOffsetMapFixed_hh_
#define _MultiMemOffsetMapFixed_hh_
//
// File:        MultiMemOffsetMapFixed.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     01/09/95 10:07
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
#include <MultiMemOffset.hh>
#include <MapMemDynamicFixed.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class MultiMemOffsetMapFixed : public MultiMemOffset
{

public:

  typedef MapMemDynamic::MapMask	MapMask;
  
  MultiMemOffsetMapFixed( const char * 	    fileName,
			  ios::open_mode    mode = ios::in );

  MultiMemOffsetMapFixed( const char *	    fileName,
			  size_type    	    recSize,
			  size_type    	    numRec = 0,
			  MapMask	    permMask = 0777 );

  virtual ~MultiMemOffsetMapFixed( void );

  virtual Loc		allocate( size_type size = 0 );
  virtual void		release( Loc loc );

  virtual Addr		address( Loc offset );
  virtual const Addr	address( Loc offset ) const;
  
  virtual Loc		location( const Addr addr ) const;
  
  virtual void *	getBase( void );
  virtual const void *  getBase( void ) const;

  MapMemDynamicFixed &     getMap( void );

  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;

protected:

private:

  MultiMemOffsetMapFixed( const MultiMemOffsetMapFixed & from );
  MultiMemOffsetMapFixed & operator =( const MultiMemOffsetMapFixed & from );

  MapMemDynamicFixed	    mem;
  
};

#if !defined( inline )
#include <MultiMemOffsetMapFixed.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MultiMemOffsetMapFixed	class
//
//  Constructors:
//
//  	MultiMemOffsetMapFixed( );
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
//  	    Return the name of this class (i.e. MultiMemOffsetMapFixed )
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
//  	operator <<( ostream & dest, const MultiMemOffsetMapFixed & src );
//
//	istream &
//	operator >> ( istream & src, MultiMemOffsetMapFixed & dest );
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
// Revision Log:
//
// $Log$
// Revision 2.6  1997/06/19 13:35:52  houghton
// Changed location to be a const method.
//
// Revision 2.5  1997/06/19 12:03:51  houghton
// Changed to be part of libMdb.
// Cleanup.
//
// Revision 2.4  1997/06/18 14:15:57  houghton
// Rework to use allocate and release.
//
// Revision 2.3  1997/03/13 02:41:58  houghton
// Added getOffset.
//
// Revision 2.2  1997/03/07 11:51:08  houghton
// Add getBase() const.
// Added dumpInfo.
//
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
#endif // ! def _MultiMemOffsetMapFixed_hh_ 

