#ifndef _DeltaNpaNxx_hh_
#define _DeltaNpaNxx_hh_
//
// File:        DeltaNpaNxx.hh
// Desc:        
//
//
//  Quick Start: - short example of class usage
//
//  Data Types: - data types defined by this header
//
//  	DeltaNpaNxx	class
//
//  Constructors:
//
//  	DeltaNpaNxx( );
//
//  Destructors:
//
//  Public Interface:
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. DeltaNpaNxx )
//
//  	virtual Bool
//  	good( void ) const;
//  	    Returns true if there are no detected errors associated
//  	    with this class, otherwise FALSE.
//
//  	virtual const char *
//  	error( void ) const
//  	    Returns as string description of the state of the class.
//
//  Protected Interface:
//
//  Private Methods:
//
//  Other Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const DeltaNpaNxx & obj );
//
// Locationor:      Paul Houghton x2309 - (houghton@shoe.wiltel.com)
// Created:     02/06/95 06:34
//
// Revision History:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:47  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:21  houghton
// Changed version to 2
//
// Revision 1.2  1997/09/17 16:56:19  houghton
// Changed for new library rename to StlUtils
//
// Revision 1.1  1997/07/19 12:52:42  houghton
// Initial Version (work in progess).
//
// Revision 1.1  1995/08/25  22:13:02  ichudov
// New module added.
//
// Revision 1.1  1995/07/28  14:40:02  ichudov
// A new DAVL version of LocationDelta.
//
//

#include <StlUtils.hh>
#include <Str.hh>

#include <LocationTable.hh>
#include <DateTime.hh>

#include <Delta.hh>

class DeltaNpaNxx : public Delta
{

public:

  DeltaNpaNxx( LocationTable * location, time_t effDateOverride );

  ~DeltaNpaNxx( void );

  Bool	    	delta( RecType	    recType,
		       LocationRec &    location,
		       DateTime &  effDate );

  const char *	getWhy( void );
   
  virtual const char *	getClassName( void ) const;
  virtual Bool	    	good( void ) const;
  virtual const char * 	error( void ) const;

protected:

  Bool	    add( LocationRec & location, DateTime & effDate );

  Bool	    del( LocationRec & location, DateTime & effDate );

  Bool	    checkTables( void );
  
  static const char * ErrorStrings[];

  enum ErrorNum
  {
    E_OK,
    E_BAD_TYPE,
    E_ADD,
    E_DEL,
    E_NOLOCATION,
    E_LOCATION,
    E_UNDEFINED
  };
  
private:

  DeltaNpaNxx( const DeltaNpaNxx & copyFrom );
  DeltaNpaNxx & operator=( const DeltaNpaNxx & assignFrom );

  LocationTable *	    locationTable;

  ErrorNum  errorVal;
  Str	    why;

  DateTime  today;
  
};


//
// Inline methods
//

inline
DeltaNpaNxx::DeltaNpaNxx(
  LocationTable * 	    location,
  time_t            effDateOverride
  ) :
  today( effDateOverride )
{
  locationTable = location;
}

inline
DeltaNpaNxx::~DeltaNpaNxx( void )
{
  
}

inline
const char *
DeltaNpaNxx::getWhy( void )
{
  return( why.cstr() );
}
#endif // ! def _DeltaNpaNxx_hh_ 
//
//              This software is the sole property of
// 
//                 The Williams Complocationes, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1995 by The Williams Complocationes, Inc.
// 
//                      All Rights Reserved.  
// 
//
