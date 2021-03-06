#ifndef __MdbMemMap_hh_
#define __MdbMemMap_hh_
//
// File:        _MdbMemMap.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul Houghton 719-527-7834 - (paul.houghton@wcom.com)
// Created:     07/18/97 06:59
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <sys/mman.h>
#include <sys/stat.h>

//
// linux has an else construct on MAP_FIXED, so it does
// not have a MAP_VARIABLE. I define it here as 0 so it will be ignored
// 
#define MAP_VARIABLE	0

// I can not find a prototype for getpagesize anywhere.
// extern "C" size_t getpagesize( void );


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	_MdbMemMap	class
//
//  Constructors:
//
//  	_MdbMemMap( );
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
//  	    Return the name of this class (i.e. _MdbMemMap )
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
//  	operator <<( ostream & dest, const _MdbMemMap & src );
//
//	istream &
//	operator >> ( istream & src, _MdbMemMap & dest );
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
// Revision 4.1  2001/07/27 00:57:44  houghton
// Change Major Version to 4
//
// Revision 2.2  1999/07/17 08:40:21  houghton
// Changed: removed prototype for getpagesize.
//
// Revision 2.1  1997/09/21 19:21:41  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 10:32:24  houghton
// Initial Version.
//
//
#endif // ! def __MdbMemMap_hh_ 

