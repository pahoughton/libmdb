#ifndef _Perf_hh_
#define _Perf_hh_
//
// File:        Perf.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/10/97 06:53
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <TimeIt.hh>

void
LogPerfData(
  ostream &	    dest,
  const char *	    appName,
  long		    passNum,
  long		    passes,
  const char *	    mapType,
  long		    initSize,
  long		    allocSize,
  long		    recSize,
  const char *	    className,
  const char *	    method,
  const char *	    order,
  long		    startSize,
  long		    quantity,
  const TimeIt &    timer
  );


struct Rec_4
{
  long	k;
  char	v[4];
};

struct Rec_128
{
  long	k;
  char	v[128];
};

struct Rec_512
{
  long	k;
  char	v[512];
};

struct Rec_1024
{
  long	k;
  char	v[1024];
};


inline
bool
operator < ( const Rec_4 & a, const Rec_4 & b )
{
  return( a.k < b.k );
}

inline
bool
operator < ( const Rec_128 & a, const Rec_128 & b )
{
  return( a.k < b.k );
}

inline
bool
operator < ( const Rec_512 & a, const Rec_512 & b )
{
  return( a.k < b.k );
}

inline
bool
operator < ( const Rec_1024 & a, const Rec_1024 & b )
{
  return( a.k < b.k );
}

struct HashRec_4
{
  inline long operator () ( const Rec_4 & key ) const {
    return( key.k );
  };
};

struct HashRec_128
{
  inline long operator () ( const Rec_128 & key ) const {
    return( key.k );
  };
};

struct HashRec_512
{
  inline long operator () ( const Rec_512 & key ) const {
    return( key.k );
  };
};

struct HashRec_1024
{
  inline long operator () ( const Rec_1024 & key ) const {
    return( key.k );
  };
};

//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	Perf	class
//
//  Constructors:
//
//  	Perf( );
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
//  	    Return the name of this class (i.e. Perf )
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
//  	operator <<( ostream & dest, const Perf & src );
//
//	istream &
//	operator >> ( istream & src, Perf & dest );
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
// Revision 1.2  1997/07/25 13:51:52  houghton
// Changned LogPerfData() args.
// Bug-Fix: operator < was missing return type.
//
// Revision 1.1  1997/07/13 11:36:39  houghton
// Initial Version.
//
//
#endif // ! def _Perf_hh_ 

