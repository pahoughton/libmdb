#ifndef _Insert_hh_
#define _Insert_hh_
//
// File:        Insert.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/23/97 10:03
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
#include "PerfRBSet.hh"
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

#if !defined( INSERT_FUNCT )
#error need INSERT_FUNCT defined
#endif

bool
INSERT_FUNCT( TABLENAME & table, ostream & perfDest, long passNum )
{
  long first;
  long last;
  REC	r;
  TABLENAME::size_type	startSize = table.size();

  first = startSize;
  last = first + App->qty();
      
  TimeIt    timer;
  
  if( App->order() == "asc" )
    {
      timer.start();

      for( long k = first; k < last ; ++ k )
	{
	  r.k = k;
	  if( ! table.insert( r ).second )
	    {
	      AppError << "insert '" << k << "' failed." << endl;
	      return( false );
	    }
	}
      
      timer.stop();
    }
  
  if( App->order() == "rev" )
    {
      timer.start();

      for( long k = last - 1; k >= first ; -- k )
	{
	  r.k = k;
	  if( ! table.insert( r ).second )
	    {
	      AppError << "insert '" << k << "' failed." << endl;
	      return( false );
	    }
	}

      timer.stop();
    }


  if( App->order() == "rand" )
    {
      vector< long >	keys;
      
      for( long k = first; k < last ; ++ k )
	{
	  keys.push_back( k );
	}
      
      random_shuffle( keys.begin(), keys.end() );
      
      timer.start();
      
      for( vector< long >::iterator them = keys.begin();
	   them != keys.end();
	   ++ them )
	{
	  r.k = (*them);
	  if( ! table.insert( r ).second )
	    {
	      AppError << "insert '" << r.k << "' failed." << endl;
	      return( false );
	    }
	}
      
      timer.stop();
    }

  if( table.size() != (startSize + App->qty() ) )
    {
      AppError << "bad size: " << table.size() << " q: " << App->qty() << endl;
      return( false );
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
	       "insert",
	       App->order(),
	       startSize,
	       App->qty(),
	       timer );

  return( true );
}

//
// Revision Log:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:30  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/25 13:42:23  houghton
// Initial Version.
//
//
//
// Local Variables:
// mode:c++
// End:
#endif // ! def _Insert_hh_ 

