#ifndef _Find_hh_
#define _Find_hh_
//
// File:        Find.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/23/97 12:40
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
#include <TimeIt.hh>

// need the following defines

#if !defined( FUNCT_NAME )
#error need FUNCT_NAME defined
#endif

#if !defined( REC )
#error need REC defined
#endif

#if !defined( TABLENAME )
#error need TABLENAME defined
#endif

#if !defined( FIND_FUNCT )
#error need FIND_FUNCT defined
#endif

bool
FIND_FUNCT( TABLENAME & table, ostream & perfDest, long passNum )
{
  long first;
  long last;
  long findQty;
  
  TimeIt timer;
  REC r;

  if( App->order() == "all" )
    {
      first = 0;
      last = table.size();
      findQty = last - first;
      
      timer.start();
      
      for( long k = first; k < last ; ++ k )
	{
	  r.k = k;
	  if( table.find( r ) == table.end() )
	    {
	      AppError << "find '" << k << "' failed." << endl;
	      exit( 1 );
	    }
	}
      
      timer.stop();
    }
  
  if( App->order() == "asc" )
    {
      first = 0;
      last = App->qty();
      findQty = App->qty();
      
      timer.start();
      
      for( long k = first; k < last ; ++ k )
	{
	  r.k = k;
	  if( table.find( r ) == table.end() )
	    {
	      AppError << "find '" << k << "' failed." << endl;
	      exit( 1 );
	    }
	}
      
      timer.stop();
    }

  if( App->order() == "rev" )
    {
      first = table.size() - App->qty();
      last = table.size();
      
      findQty = App->qty();

      timer.start();
      
      for( long k = first; k < last ; ++ k )
	{
	  r.k = k;
	  if( table.find( r ) == table.end() )
	    {
	      AppError << "find '" << k << "' failed." << endl;
	      exit( 1 );
	    }
	}
      
      timer.stop();
    }
  

  if( App->order() == "rand" )
    {
      first = 0;
      last = table.size();

      findQty = min( (long)table.size(), App->qty() );
      
      vector< long >	keys;
      
      for( long k = first; k < last ; ++ k )
	{
	  keys.push_back( k );
	}
      
      random_shuffle( keys.begin(), keys.end() );

      if( findQty < table.size() )
	keys.erase( keys.begin() + findQty, keys.end() );

      if( keys.size() != findQty )
	{
	  AppError << "bad size: " << keys.size() << " q: " << findQty << endl;
	  return( false );
	}

      timer.start();

      for( vector< long >::iterator them = keys.begin();
	   them != keys.end();
	   ++ them )
	{
	  r.k = (*them);
	  if( table.find( r ) == table.end() )
	    {
	      AppError << "find '" << r.k << "' failed." << endl;
	      exit( 1 );
	    }
	}
      
      timer.stop();
    }
  
  LogPerfData( perfDest,
	       App->appName(),
	       passNum,
	       App->passes(),
	       App->mapType(),
	       App->initSize(),
	       App->allocSize(),
	       Table_4::getNodeSize(),
	       table.getClassName(),
	       "find",
	       App->order(),
	       table.size(),
	       findQty,
	       timer );

  return( true );
}

//
// Revision Log:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:10  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/25 13:41:40  houghton
// Initial Version.
//
//
// Local Variables:
// mode:c++
// End:
#endif // ! def _Find_hh_ 

