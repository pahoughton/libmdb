#ifndef _MdbVersion_hh_
#define _MdbVersion_hh_
//
// File:        MdbVersion.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/08/97 06:26
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <ClassVersion.hh>

#define MDB_PRJ_NAME	    "libMdb"
#define MDB_PRJ_VER	    VERSION
#define MDB_PRJ_VER_NUM     0x01010101L
#define MDB_PRJ_VER_TAG     "ALPHA_1_01"

// Macros
#define MDB_VERSION( name_, verId_ )					      \
CLASS_VERSION( name_,							      \
	       verId_,							      \
	       MDB_PRJ_NAME,						      \
	       MDB_PRJ_VER,						      \
	       MDB_PRJ_VER_NUM,					      \
	       MDB_PRJ_VER_TAG )
				   
#define MDB_FUNCT_VERSION( name_, verId_ )				      \
const char VERID_##name_[] =						      \
"@(#) " MDB_PRJ_NAME " - " MDB_PRJ_VER "\n"				      \
"    " #name_ " - " __FILE__ " - Compiled: " __DATE__ " "__TIME__ "\n"	      \
"    CVS Tag: " MDB_PRJ_VER_TAG "\n"					      \
"    " verId_


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MdbVersion	class
//
//  Constructors:
//
//  	MdbVersion( );
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
//  	    Return the name of this class (i.e. MdbVersion )
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
//  	operator <<( ostream & dest, const MdbVersion & src );
//
//	istream &
//	operator >> ( istream & src, MdbVersion & dest );
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
// Revision 2.2  1997/09/19 12:33:36  houghton
// Changed to use version info from Makefile.
//
// Revision 2.1  1997/06/05 11:29:13  houghton
// Initial Version.
//
//
#endif // ! def _MdbVersion_hh_ 

