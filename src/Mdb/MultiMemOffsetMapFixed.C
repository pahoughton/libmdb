//
// File:        MultiMemOffsetMapFixed.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for MultiMemOffsetMapFixed
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     01/28/95 08:41 
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "MultiMemOffsetMapFixed.hh"
#include <Str.hh>

#if defined( MDB_DEBUG )
#include "MultiMemOffsetMapFixed.ii"
#endif

MDB_VERSION(
  MultiMemOffsetMapFixed,
  "$Id$");


MultiMemOffsetMapFixed::MultiMemOffsetMapFixed(
  const char * 	    fileName,
  ios::open_mode    mode
  )
  : mem( fileName, mode )
{
}

MultiMemOffsetMapFixed::MultiMemOffsetMapFixed(
  const char * 	    fileName,
  size_type    	    recSize,
  size_type    	    numRecs,
  MapMask	    permMask
  )
  : mem( fileName, recSize, numRecs, permMask )
{
}

MultiMemOffsetMapFixed::~MultiMemOffsetMapFixed( void )
{
}

bool
MultiMemOffsetMapFixed::good( void ) const
{
  return( mem.good() );
}

const char *
MultiMemOffsetMapFixed::error( void ) const
{
  static Str errStr;

  errStr = MultiMemOffsetMapFixed::getClassName();

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! mem.good() )
	errStr << ": " << mem.error();
      
      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
MultiMemOffsetMapFixed::getClassName( void ) const
{
  return( "MultiMemOffsetMapFixed" );
}

const char *
MultiMemOffsetMapFixed::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
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
	 << MultiMemOffsetMapFixed::getVersion() << '\n';
  
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


// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:44  houghton
// Change Major Version to 4
//
// Revision 2.6  1997/06/19 12:03:48  houghton
// Changed to be part of libMdb.
// Cleanup.
//
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
