#ifndef _MdbConfig_hh_
#define _MdbConfig_hh_
//
// File:        MdbConfig.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/08/97 06:24
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <StlUtilsConfig.hh>
#include <MdbVersion.hh>

#if defined( STLUTILS_HAVE_LONG_LONG )
#define MDB_HAVE_LONG_LONG 1
#endif

#define MDB_U16_T	    STLUTILS_U16_T
#define MDB_S16_T	    STLUTILS_S16_T
#define MDB_U32_T	    STLUTILS_U32_T
#define MDB_S32_T	    STLUTILS_S32_T

#if defined( MDB_HAVE_LONG_LONG )
#define MDB_U64_T	    STLUTILS_U64_T
#define MDB_S64_T	    STLUTILS_S64_T
#endif

#define MDB_TYPE_SIZE	    MDB_U32_T
#define MDB_TYPE_LOC	    MDB_S32_T
#define MDB_TYPE_ADDR	    char *

//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MdbConfig	class
//
//  Constructors:
//
//  	MdbConfig( );
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
//  	    Return the name of this class (i.e. MdbConfig )
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
//  	operator <<( ostream & dest, const MdbConfig & src );
//
//	istream &
//	operator >> ( istream & src, MdbConfig & dest );
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
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.5  1999/05/09 17:32:18  houghton
// Added long long support.
//
// Revision 2.4  1997/10/01 14:02:57  houghton
// Chaged so that 'keys' have to be reserved to be set.
// Increased the number of keys from 16 to 32.
// Changed to use portable multi platform types.
//
// Revision 2.3  1997/09/17 16:56:08  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.2  1997/07/13 11:28:07  houghton
// Added MDB_TYPE_SIZE, MDB_TYPE_LOC & MDB_TYPE_ADDR.
//
// Revision 2.1  1997/06/05 11:29:12  houghton
// Initial Version.
//
//
#endif // ! def _MdbConfig_hh_ 

