//
// File:        MultiMemOffset.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for MultiMemOffset
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     01/28/95 08:25 
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "MultiMemOffset.hh"
#include <Str.hh>

#if defined( MDB_DEBUG )
#include "MultiMemOffset.ii"
#endif

MDB_VERSION(
  MultiMemOffset,
  "$Id$");

const MultiMemOffset::Loc MultiMemOffset::badLoc( 0 );

MultiMemOffset::MultiMemOffset( void )
{
}

MultiMemOffset::~MultiMemOffset( void )
{
}

bool
MultiMemOffset::good( void ) const
{
  return( true );
}

const char *
MultiMemOffset::error( void ) const
{
  static Str errStr;

  errStr = MultiMemOffset::getClassName();

  if( good() )
    {
      errStr << ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
MultiMemOffset::getClassName( void ) const
{
  return( "MultiMemOffset" );
}

const char *
MultiMemOffset::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
MultiMemOffset::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << MultiMemOffset::getClassName() << ":\n"
	 << MultiMemOffset::getVersion() << '\n';

  if( ! MultiMemOffset::good() )
    dest << prefix << "Error: " << MultiMemOffset::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  return( dest );
}

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.7  1997/07/13 11:30:23  houghton
// Cleanup.
// Removed code for allocate & release (now pure virtual).
//
// Revision 2.6  1997/06/19 13:35:51  houghton
// Changed location to be a const method.
//
// Revision 2.5  1997/06/19 12:03:11  houghton
// Changed to be part of libMdb.
// Cleanup.
// Changed void * and off_t to Addr and Loc.
//
// Revision 2.4  1997/04/04 20:50:34  houghton
// Cleanup.
//
// Revision 2.3  1997/03/13 02:40:53  houghton
// Added getOffset.
// Added dumpInfo.
//
// Revision 2.2  1997/03/07 11:50:41  houghton
// Add getBase() const.
//
// Revision 2.1  1995/11/10 12:42:37  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:36  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:51  houghton
// New Style Avl an memory management. Many New Classes
//
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

