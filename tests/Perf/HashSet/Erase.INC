#ifndef _Erase_hh_
#define _Erase_hh_
//
// File:        Erase.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/24/97 06:40
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

#if !defined( ERASE_FUNCT )
#error need ERASE_FUNCT defined
#endif

bool
ERASE_FUNCT( TABLENAME & table, ostream & perfDest, long passNum )
{
  long first;
  long last;
  long eraseQty;
  
  REC	r;
  TABLENAME::size_type	startSize = table.size();

  TimeIt    timer;
  
  if( App->order() == "asc" )
    {
      first = 0;
      last = first + App->qty();

      eraseQty = App->qty();
      
      timer.start();

      for( long k = first; k < last ; ++ k )
	{
	  r.k = k;
	  if( ! table.erase( r ) )
	    {
	      AppError << "erase '" << k << "' failed." << endl;
	      return( false );
	    }
	}
      
      timer.stop();
    }
  
  if( App->order() == "rev" )
    {
      last = startSize;
      first = last - App->qty();

      eraseQty = App->qty();
      
      timer.start();

      for( long k = last - 1; k >= first ; -- k )
	{
	  r.k = k;
	  if( ! table.erase( r ) )
	    {
	      AppError << "insert '" << k << "' failed." << endl;
	      return( false );
	    }
	}

      timer.stop();
    }


  if( App->order() == "rand" )
    {
      first = 0;
      last = table.size();

      eraseQty = min( (long)table.size(), App->qty() );
      
      vector< long >	keys;
      
      for( long k = first; k < last ; ++ k )
	{
	  keys.push_back( k );
	}
      
      random_shuffle( keys.begin(), keys.end() );

      if( eraseQty < table.size() )
	keys.erase( keys.begin() + eraseQty, keys.end() );

      if( keys.size() != eraseQty )
	{
	  AppError << "bad size: " << keys.size()
		   << " q: " << eraseQty << endl;
	  return( false );
	}

      timer.start();

      for( vector< long >::iterator them = keys.begin();
	   them != keys.end();
	   ++ them )
	{
	  r.k = (*them);
	  if( table.erase( r ) )
	    {
	      AppError << "erase '" << r.k << "' failed." << endl;
	      exit( 1 );
	    }
	}
      
      timer.stop();
    }

  if( table.size() != (startSize - eraseQty ) )
    {
      AppError << "bad size: " << table.size() << " q: " << eraseQty << endl;
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
	       "erase",
	       App->order(),
	       startSize,
	       eraseQty,
	       timer );

  return( true );
}
//
// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:08  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/25 13:41:38  houghton
// Initial Version.
//
//
// Local Variables:
// mode:c++
// End:
#endif // ! def _Erase_hh_ 

