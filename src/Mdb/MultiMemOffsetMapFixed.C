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
// Revision 2.5  1997/06/18 14:16:03  houghton
// Rework to use allocate and release.
//
// Revision 2.4  1997/04/04 20:50:37  houghton
// Cleanup.
//
// Revision 2.3  1997/03/13 02:41:24  houghton
// Cleanup dumpInfo output.
//
// Revision 2.2  1997/03/07 11:51:18  houghton
// Add getBase() const.
// Added dumpInfo.
//
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
      errStr << ": ok";
    }
  else
    {
      errStr << ": " << mem.error();
    }
  return( errStr.cstr() );
}


ostream &
MultiMemOffsetMapFixed::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << MultiMemOffsetMapFixed::getClassName() << ":\n"
	 << RcsId << "\n";
  
  if( ! MultiMemOffsetMapFixed::good() )
    dest << prefix << "Error: " << MultiMemOffsetMapFixed::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << "mem:";

  mem.dumpInfo( dest, pre, false );

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
