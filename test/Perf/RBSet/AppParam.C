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

AppParam::AppParam(
  int &		mainArgc,
  char **	mainArgv,
  const char *	mainVer
  )
  : Param( mainArgc, mainArgv, mainVer, true, "ERROR | WARN | INFO" ),
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
  Str mode;
  
  status &= argStr( mode,
		    "table mode",
		    "mode" );

  if( mode.size() )
    {
      tableModeV = ios::in;
      if( mode.find( "out" ) != Str::npos )
	tableModeV = (ios::open_mode)(tableModeV|ios::out);
    }

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
// Revision 1.2  1997/07/25 13:52:19  houghton
// Added args for new style performance test.
//
// Revision 1.1  1997/07/21 10:29:40  houghton
// Initial version.
//
//
