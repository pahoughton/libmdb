#ifndef _MdbUtils_hh_
#define _MdbUtils_hh_
//
// File:        MdbUtils.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@mci.com)
// Created:     07/02/97 06:27
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

template <class T, class U>
struct MdbIdent {
    const U & operator() (const T & x) const { return x; }
    U & operator() (T & x) { return x; }
};


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MdbUtils	class
//
//  Constructors:
//
//  	MdbUtils( );
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
//  	    Return the name of this class (i.e. MdbUtils )
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
//  	operator <<( ostream & dest, const MdbUtils & src );
//
//	istream &
//	operator >> ( istream & src, MdbUtils & dest );
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
// Revision 4.2  2003/08/09 12:43:24  houghton
// Changed ver strings.
//
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.2  1997/07/22 19:43:56  houghton
// Added non-const operator ().
//
// Revision 2.1  1997/07/11 17:37:36  houghton
// Initial Version.
//
//
#endif // ! def _MdbUtils_hh_ 

