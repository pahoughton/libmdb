//
// File:        MultiMemOffset.C
// Desc:        
//              
//
// Author:      Paul Houghton x2309 - (houghton@shoe.wiltel.com)
// Created:     01/28/95 08:25 
//
// Revision History:
//
// $Log$
// Revision 1.1  1995/02/13 16:08:51  houghton
// New Style Avl an memory management. Many New Classes
//
//
static const char * RcsId =
"$Id$";

#include "MultiMemOffset.hh"
#include "Str.hh"

MultiMemOffset MultiMemOffsetMalloc;

const char *
MultiMemOffset::error( void ) const
{
  static Str errStr;
  errStr.reset();

  errStr << MultiMemOffset::getClassName();
  
  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {
      errStr << ": " << strerror( oserrno );
    }
  return( errStr.cstr() );
}


ostream &
MultiMemOffset::getStats( ostream & dest ) const
{
  dest << MultiMemOffset::getClassName() << ": stats" << '\n'
       << "    Status:  " << error() << '\n'
       << "    Using:   malloc" << endl
    ;
  
  return( dest );
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

