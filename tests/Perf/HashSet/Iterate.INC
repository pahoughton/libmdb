#ifndef _Iterate_hh_
#define _Iterate_hh_
//
// File:        Iterate.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/24/97 06:54
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

#if !defined( ITERATE_FUNCT )
#error need ITERATE_FUNCT defined
#endif

bool
ITERATE_FUNCT( TABLENAME & table, ostream & perfDest, long passNum )
{
  long iterateQty = 0;
  TimeIt timer;
  REC r;

  if( App->order() == "all" ||
      App->order() == "asc" )
    {
      iterateQty = table.size();
      
      timer.start();

      for( TABLENAME::iterator them = table.begin();
	   them != table.end();
	   ++ them );

      timer.stop();
    }
  
  if( App->order() == "rev" )
    {
      iterateQty = table.size();
      
      timer.start();

      for( TABLENAME::reverse_iterator them = table.rbegin();
	   them != table.rend();
	   ++ them );

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
	       "iterate",
	       App->order(),
	       table.size(),
	       iterateQty,
	       timer );

  return( true );
}

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:14  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/25 13:41:43  houghton
// Initial Version.
//
//
// Local Variables:
// mode:c++
// End:
#endif // ! def _Iterate_hh_ 

