//
// File:        DeltaNpaNxx.C
// Desc:        
//
//  
//  
// Locationor:      Paul Houghton x2309 - (houghton@shoe.wiltel.com)
// Created:     02/06/95 06:59 
//
// Revision History:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:21  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 12:52:42  houghton
// Initial Version (work in progess).
//
// Revision 1.1  1995/08/25  22:13:01  ichudov
// New module added.
//
// Revision 1.1  1995/07/28  14:40:00  ichudov
// A new DAVL version of LocationDelta.
//
//
static const char * RcsId =
"$Id$";

#include "DeltaNpaNxx.hh"

Bool
DeltaNpaNxx::delta( RecType type,
		  LocationRec & location,
		  DateTime & effDate )
{
  why.reset();

  if( ! checkTables() )
    {
      return( FALSE );
    }

  switch( type )
    {
      // since this is a davl, there is no difference between
      // an add and a change
    case ADD:
    case CHANGE:
      return( add( location, effDate ) );

    case DEL:
      return( del( location, effDate ) );

    default:
      errorVal = E_BAD_TYPE;
      why << "Unknown mod type! (" << type << ')';
      return( FALSE );
    }
}

Bool
DeltaNpaNxx::add( LocationRec & location, DateTime & effDate )
{

  // this will slow things down a little, but
  // it's better than storing a bunch of duplicate
  // data.

  off_t found = locationTable->find( location.getKey(), effDate.getTimeT() );

  Bool effDateIsFake = FALSE;

  if( locationTable->find( location.getKey() ) && 
      effDate.getTimeT() == 0 ) // !!!!
    {
      effDate.setTimeT( today.getTimeT() );
      effDateIsFake = TRUE;
    }


  if( found )
    {
      LocationRec foundLocation( location.getKeyAddr(), 
                                 locationTable->getDataAddr( found ) );
      time_t	foundEffDate = locationTable->getWhen( found );

      if( (effDateIsFake /* || (foundEffDate == effDate.getTimeT()) */ ) &&
	  foundLocation == location )
	{
	  // there is already a record in the table that is
	  // EXACTLY the same as this one, don't add it

	  why << "Record exists";
	  return( FALSE );
	}
    }

  // add return = 0 is successful

  if( locationTable->add( location.getKey(), effDate.getTimeT(), 
                          location.getData() ) )
    {
      errorVal = E_ADD;
      return( FALSE );
    }

  return( TRUE );
}
  
Bool
DeltaNpaNxx::del( LocationRec & location, DateTime & effDate )
{
  if( locationTable->find( location.getKey() ) && 
      effDate.getTimeT() == 0 ) // !!!!
    {
      effDate.setTimeT( today.getTimeT() );
    }

  if( ! locationTable->del( location.getKey(), effDate.getTimeT() ) )
    {
      errorVal = E_DEL;
      return( FALSE );
    }

  return( TRUE );
}

// getClassName - return the name of this class
const char *
DeltaNpaNxx::getClassName( void ) const
{
  return( "DeltaNpaNxx" );
}

Bool
DeltaNpaNxx::checkTables( void )
{
  if( ! locationTable )
    {
      errorVal = E_NOLOCATION;
      return( FALSE );
    }

  if( ! locationTable->good() )
    {
      errorVal = E_LOCATION;
      return( FALSE );
    }

  return( TRUE );
}  

// good - return TRUE if no detected errors
Bool
DeltaNpaNxx::good( void ) const
{
  return( errorVal == E_OK );
}

const char * DeltaNpaNxx::ErrorStrings[] =
{
  ": Ok",
  ": bad record type",
  ": can't add record",
  ": can't del record",
  ": no location table",
  ": LocationTable error: ",
  0
};
  

// error - return a string describing the current state
const char *
DeltaNpaNxx::error( void ) const
{
  static Str errStr;
  errStr.reset();

  errStr << getClassName();

  if( good() )
    {
       errStr << ": Ok";
    }
  else
    {
      if( errorVal > E_OK && errorVal < E_UNDEFINED )
	{
	  errStr << ErrorStrings[ errorVal ] << ' ';

	  if( errorVal == E_LOCATION )
	    {
	      errStr << locationTable->error();
	    }
	}
      else
	{
	  errStr << ": unknown error";
	}
    }

  return( errStr.cstr() );
}



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
