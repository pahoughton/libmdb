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
    perfInfoFnV( "rbset.perf" ),
    mapTypeV( "mdd" ),
    tableFnV( "../data/TestTable.tbl" ),
    tableModeV( ios::in ),
    createV( false ),
    allocSizeV( 1 ),
    qtyV( 1000000 ),
    orderV("asc"),
    insertV( false ),
    findV( true )
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

  status &= argStr( perfInfoFnV,
		    "perf info file",
		    "perf" );
  
  status &= argStr( mapTypeV,
		    "map type",
		    "mt" );

  status &= argStr( tableFnV,
		    "table file name",
		    "fn" );

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

  status &= argULong( allocSizeV,
		      "alloc size",
		      "alloc" );

  status &= argLong( qtyV,
		     "quantity",
		     "q");

  status &= argStr( orderV,
		    "order",
		    "ord" );

  status &= argBool( insertV,
		     "insret recs",
		     "i" );

  status &= argBool( findV,
		     "find recs",
		     "f" );
  
  // status &= argStr( varV, "desc", "argid", "envVar" );

  return( status );
}

// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/21 10:29:40  houghton
// Initial version.
//
//
