//
// File:        AppParam.C
// Desc:        
//
//  
//  
// Locationor:      Igor Chudov - (ichudov@shoe.wiltel.com)
// Created:     06/30/95 16:05 
//
// Revision History:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:19  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 12:52:41  houghton
// Initial Version (work in progess).
//
// Revision 1.1  1995/08/25  22:12:58  ichudov
// New module added.
//
//
static const char * RcsId =
"$Id$";

#include "AppParam.hh"





// getClassName - return the name of this class
const char *
AppParam::getClassName( void ) const
{
  return( "AppParam" );
}

// good - return TRUE if no detected errors
Bool
AppParam::good( void ) const
{
  
  if( !AppParam_PARENT::good() )
    {
      return( FALSE );
    }

  return( TRUE );
}

// error - return a string describing the current state
const char *
AppParam::error( void ) const
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
      errStr << ": Error in command line parameters";
    }

  return( errStr.cstr() );
}



//
//              This software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1995 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//
