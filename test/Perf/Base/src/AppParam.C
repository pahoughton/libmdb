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
  : Param( argc, argv, ver, true, "ERROR | WARN | INFO" ),
    perfLogFnV( "perf.info" )
{
  parseArgs();
}

AppParam::~AppParam( void )
{
}

bool
AppParam::parseArgs( int mainArgc, char * mainArgv[] )
{
  bool status = Param::parseArgs( mainArgc, mainArgv );

  return( status ? parseArgs() : status );
}

bool
AppParam::parseArgs( void )
{
  bool status = Param::parseArgs();

  status &= argStr( perfLogFnV,
		    "perf info output file name",
		    "o" );

  return( status );
}

// Revision Log:
//
// $Log$
// Revision 2.2  2000/05/27 14:31:56  houghton
// Port: Sun CC 5.0.
//
// Revision 2.1  1997/09/21 19:22:04  houghton
// Changed version to 2
//
// Revision 1.2  1997/07/14 10:50:45  houghton
// Added perfLogFn arg.
//
// Revision 1.1  1997/07/13 11:36:39  houghton
// Initial Version.
//
//
