#ifndef _PerfTable_4_hh_
#define _PerfTable_4_hh_
//
// File:        PerfTable_4.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/23/97 09:46
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include "AppParam.hh"
#include "PerfHashSet.hh"
#include <MapMemDynamicDynamic.hh>
#include <MapMemDynamicFixed.hh>
#include <MultiMemOffsetMapDynamic.hh>

// need the following defines

#if !defined( FUNCT_NAME )
#error need FUNCT_NAME defined
#endif

#if !defined( TABLENAME )
#error need TABLENAME defined
#endif

#if !defined( INSERT_FUNCT )
#error need INSERT_FUNCT defined
#endif

#if !defined( FIND_FUNCT )
#error need FIND_FUNCT defined
#endif

#if !defined( ERASE_FUNCT )
#error need ERASE_FUNCT defined
#endif

#if !defined( ITERATE_FUNCT )
#error need ITERATE_FUNCT defined
#endif

bool INSERT_FUNCT( TABLENAME & table, ostream & perfDest, long passNum );
bool FIND_FUNCT( TABLENAME & table, ostream & perfDest, long passNum );
bool ERASE_FUNCT( TABLENAME & table, ostream & perfDest, long passNum );
bool ITERATE_FUNCT( TABLENAME & table, ostream & perfDest, long passNum );

bool
FUNCT_NAME( ostream & perfDest )
{
  
  MapMemDynamic	* mmd = 0;

  FilePath  tableFn( App->tableDir(), "PerfHashSet" );
  FilePath  indexFn( App->tableDir(), "PerfHashSet" );
  
  tableFn << '.' << App->mapType() << '.' << App->recSize() << ".data";
  indexFn << '.' << App->mapType() << '.' << App->recSize() << ".hash";
  
  if( App->mapType() == "mdd" )
    mmd = new MapMemDynamicDynamic( tableFn,
				    App->tableMode(),
				    App->create(),
				    TABLENAME::getNodeSize(),
				    App->allocSize(),
				    02 );

  if( App->mapType() == "mdf" )
    mmd = new MapMemDynamicFixed( tableFn,
				  App->tableMode(),
				  App->create(),
				  TABLENAME::getNodeSize(),
				  App->allocSize(),
				  02 );

  if( ! mmd )
    {
      AppError << "bad map type: " << App->mapType() << endl;
      exit( 1 );
    }

  if( ! mmd->good() )
    {
      AppError << mmd->error() << endl;
      exit( 1 );
    }
  
  if( App->initSize() && App->create() )
    {
      if( ! mmd->expand( App->initSize() * TABLENAME::getNodeSize() ) )
	{
	  AppError << mmd->error() << endl;
	  return( false );
	}
    }
  
  MultiMemOffsetMapDynamic  mmo( mmd, true );

  if( ! mmo.good() )
    {
      AppError << mmo.error() << endl;
      exit( 1 );
    }
  
  TABLENAME   table( &mmo, indexFn, App->tableMode(), App->create(), 02 );

  for( long p = 0; p < App->passes(); ++ p )
    {
      
      if( App->insert() )
	{
	  if( ! INSERT_FUNCT( table, perfDest, p ) )
	    return( false );
	}

      if( App->find() )
	{
	  if( ! FIND_FUNCT( table, perfDest, p ) )
	    return( false );
	}

      if( App->erase() )
	{
	  if( ! ERASE_FUNCT( table, perfDest, p ) )
	    return( FALSE );
	}

      if( App->iterate() )
	{
	  if( ! ITERATE_FUNCT( table, perfDest, p ) )
	    return( false );
	}
    }

  AppDebug << table.dump() << endl;
  
  return( true );
}

//
// Revision Log:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:16  houghton
// Changed version to 2
//
// Revision 1.2  1997/07/28 16:52:43  houghton
// Added support for initial map size.
//
// Revision 1.1  1997/07/25 13:41:46  houghton
// Initial Version.
//
//
// Local Variables:
// mode:c++
// End:
#endif // ! def _PerfTable_4_hh_ 

