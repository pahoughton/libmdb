//
// File:        AppParam.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for AppParam
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/10/97 06:03
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "AppParam.hh"

AppParam::AppParam( int & argc, char ** argv, const char * ver )
  : Param( argc, argv, ver, true, "ERROR | WARN | INFO" )
{
  parseArgs();
}

AppParam::~AppParam( void )
{
}

bool
AppParam::parseArgs( int argc, char * argv[] )
{
  bool status = Param::parseArgs( argc, argv );

  return( status ? parseArgs() : status );
}

bool
AppParam::parseArgs( void )
{
  bool status = Param::parseArgs();

  // status &= argStr( varV, "desc", "argid", "envVar" );

  return( status );
}

// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/13 11:36:39  houghton
// Initial Version.
//
//
