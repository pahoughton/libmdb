#ifndef _MultiMemOffsetMapDynamic_hh_
#define _MultiMemOffsetMapDynamic_hh_
//
// File:        MultiMemOffsetMapDynamic.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/30/97 07:03
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
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif

class MultiMemOffsetMapDynamic : public MultiMemOffset
{

public:

  MultiMemOffsetMapDynamic( MapMemDynamic * mapMemMgr,
			    bool	    delMemMgr = false );

  virtual ~MultiMemOffsetMapDynamic( void );

  virtual Loc		allocate( size_type size );
  virtual void		release( Loc loc );

  virtual Addr		getBase( void );
  virtual const Addr	getBase( void ) const;

  virtual bool		reserveKey( unsigned short key );
  virtual bool		setNewKey( unsigned short key, KeyValue value );
  
  virtual bool		setKey( unsigned short key, KeyValue value );
  virtual KeyValue	getKey( unsigned short key ) const;
  
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

  MultiMemOffsetMapDynamic( const MultiMemOffsetMapDynamic & from );
  MultiMemOffsetMapDynamic & operator =( const MultiMemOffsetMapDynamic & from );

  MapMemDynamic *   mem;
  bool		    delMem;
  
};

#if !defined( inline )
#include <MultiMemOffsetMapDynamic.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MultiMemOffsetMapDynamic	class
//
//  Constructors:
//
//  	MultiMemOffsetMapDynamic( );
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
//  	    Return the name of this class (i.e. MultiMemOffsetMapDynamic )
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
//  	operator <<( ostream & dest, const MultiMemOffsetMapDynamic & src );
//
//	istream &
//	operator >> ( istream & src, MultiMemOffsetMapDynamic & dest );
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
// Revision 2.2  1997/10/01 14:03:05  houghton
// Chaged so that 'keys' have to be reserved to be set.
// Increased the number of keys from 16 to 32.
// Changed to use portable multi platform types.
//
// Revision 2.1  1997/07/11 17:37:41  houghton
// Initial Version.
//
//
#endif // ! def _MultiMemOffsetMapDynamic_hh_ 

