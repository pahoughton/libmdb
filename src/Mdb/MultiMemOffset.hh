#ifndef _MultiMemOffset_hh_
#define _MultiMemOffset_hh_
//
// File:        MultiMemOffset.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     01/09/95 09:47
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
#include <MapMemDynamic.hh>
#include <cstddef>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class MultiMemOffset
{

public:

  typedef MapMemDynamic::Loc	    Loc;
  typedef MapMemDynamic::size_type  size_type;
  typedef void *		    Addr;
  
  MultiMemOffset( void );
  
  virtual ~MultiMemOffset( void );
  
  virtual Loc		allocate( size_type size );
  virtual void		release( Loc offset );

  virtual Addr		address( Loc offset );
  virtual const Addr	address( Loc loc ) const;
  
  virtual Loc		location( void * addr );
  
  virtual void *	getBase( void );
  virtual const void *	getBase( void ) const;

  virtual ostream & 	getStats( ostream & dest ) const;


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

  MultiMemOffset( const MultiMemOffset & from );
  MultiMemOffset & operator =( const MultiMemOffset & from );

  int	osErrno;
  
};

extern MultiMemOffset MultiMemOffsetMalloc;

#if !defined( inline )
#include <MultiMemOffset.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MultiMemOffset	class
//
//  Constructors:
//
//  	MultiMemOffset( );
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
//  	    Return the name of this class (i.e. MultiMemOffset )
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
//  	operator <<( ostream & dest, const MultiMemOffset & src );
//
//	istream &
//	operator >> ( istream & src, MultiMemOffset & dest );
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
// Revision 2.4  1997/06/19 12:03:14  houghton
// Changed to be part of libMdb.
// Cleanup.
// Changed void * and off_t to Addr and Loc.
//
// Revision 2.3  1997/03/13 02:40:59  houghton
// Added getOffset.
// Added dumpInfo.
//
// Revision 2.2  1997/03/07 11:50:26  houghton
// Add getBase() const.
//
// Revision 2.1  1995/11/10 12:42:37  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:37  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:51  houghton
// New Style Avl an memory management. Many New Classes
//
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
#endif // ! def _MultiMemOffset_hh_ 

