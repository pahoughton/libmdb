//
// File:        MultiMemOffsetMapFixed.C
// Desc:        
//              
//
// Author:      Paul Houghton x2309 - (houghton@shoe.wiltel.com)
// Created:     01/28/95 08:41 
//
// Revision History:
//
// $Log$
// Revision 2.1  1995/11/10 12:42:38  houghton
// Change to Version 2
//
// Revision 1.1  1995/02/13  16:08:52  houghton
// New Style Avl an memory management. Many New Classes
//
//
static const char * RcsId =
"$Id$";

#include "MultiMemOffsetMapFixed.hh"
#include "Str.hh"

const char *
MultiMemOffsetMapFixed::error( void ) const
{
  static Str errStr;
  errStr.reset();

  errStr << MultiMemOffsetMapFixed::getClassName();
  
  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {
      errStr << ": " << mem.error();
    }
  return( errStr.cstr() );
}


ostream &
MultiMemOffsetMapFixed::getStats( ostream & dest ) const
{
  dest << MultiMemOffsetMapFixed::getClassName() << ": stats" << endl
       << "    Status:   " << error() << endl
       << "    Using:    MapMemFixedDynamic" << endl
    ;
  
  return( mem.getStats( dest ) );
}



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
