//
// File:        AppParam.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for AppParam
//  
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     05/05/99 06:11
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "AppParam.hh"

AppParam::AppParam(
  int &		mainArgc,
  char **	mainArgv,
  const char *	mainVer
  )
  : Param( mainArgc, mainArgv, mainVer, true, "ERROR | WARN | INFO" )
{
  parseArgs();
}

AppParam::~AppParam( void )
{
}

bool
AppParam::parseArgs( int pArgc, char * pArgv[] )
{
  bool status = Param::parseArgs( pArgc, pArgv );

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
// Revision 4.1  2001/07/27 00:57:42  houghton
// Change Major Version to 4
//
// Revision 1.1  1999/06/10 11:47:09  houghton
// Initial Version.
//
//
