//
// File:        AppParam.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for AppParam
//  
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     07/20/97 09:57
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "AppParam.hh"
#include <StlUtilsUtils.hh>

AppParam::AppParam(
  int &		mainArgc,
  char **	mainArgv,
  const char *	mainVer
  )
  : Param( mainArgc,
	   mainArgv,
	   mainVer,
	   true,
	   "ERROR | WARN | INFO | DEBUG",
	   true ),
    passesV( 1 ),
    tableDirV( "../data" ),
    mapTypeV( "mdd" ),
    recSizeV( 4 ),
    tableModeV( ios::in ),
    createV( false ),
    initSizeV( 0 ),
    allocSizeV( 1 ),
    qtyV( 1000000 ),
    orderV("asc"),
    insertV( false ),
    findV( false ),
    eraseV( false ),
    iterateV( false )
{
  perfInfoFnV = appName();
  perfInfoFnV += ".perf";
  
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

  status &= argStr( perfInfoFnV,
		    "perf info file",
		    "perf" );

  status &= argLong( passesV,
		     "number of times to run test",
		     "p" );
  
  status &= argStr( tableDirV,
		    "table directory name",
		    "dir" );

  status &= argStr( mapTypeV,
		    "map type",
		    "mt" );

  status &= argLong( recSizeV,
		     "record size: 4, 128, 512, 1024)",
		     "rs" );
  
  Str modeStr( IosOpenModeToString( tableModeV ) );
  
  status &= argStr( modeStr,
		    "table mode",
		    "mode" );

  tableModeV = IosOpenModeFromString( modeStr );

  status &= argBool( createV,
		     "create table",
		     "create" );

  status &= argULong( initSizeV,
		      "initial size to allocate",
		      "is" );
  
  status &= argULong( allocSizeV,
		      "expand size",
		      "alloc" );

  status &= argLong( qtyV,
		     "quantity",
		     "q");

  status &= argStr( orderV,
		    "order",
		    "ord" );

  status &= argFlag( insertV,
		     "insret recs",
		     "i" );

  status &= argFlag( findV,
		     "find recs",
		     "f" );
  
  status &= argFlag( eraseV,
		     "erase recs",
		     "e" );
  
  status &= argFlag( iterateV,
		     "iterate recs",
		     "iter" );
  
  if( insertV || eraseV )
    tableModeV = (ios::open_mode)(ios::in|ios::out);
  
  // status &= argStr( varV, "desc", "argid", "envVar" );

  return( status );
}

// Revision Log:
//
// $Log$
// Revision 1.4  1997/09/17 16:56:20  houghton
// Changed for new library rename to StlUtils
//
// Revision 1.3  1997/07/28 16:51:41  houghton
// Changed to use default log file.
// Changed to use IosModeToString() and IosModeFromString().
//
// Revision 1.2  1997/07/25 13:52:19  houghton
// Added args for new style performance test.
//
// Revision 1.1  1997/07/21 10:29:40  houghton
// Initial version.
//
//
