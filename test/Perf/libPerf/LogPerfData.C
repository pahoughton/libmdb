//
// File:        LogPerfData.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for LogPerfData
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/10/97 06:50
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "Perf.hh"
#include <LibLog.hh>
#include <TimeIt.hh>
#include <iostream>
#include <iomanip>

void
LogPerfData(
  ostream &	    dest,
  const char *	    appName,
  long		    passNum,
  long		    passes,
  const char *	    mapType,
  long		    initSize,
  long		    allocSize,
  long		    recSize,
  const char *	    className,
  const char *	    method,
  const char *	    order,
  long		    startSize,
  long		    quantity,
  const TimeIt &    timer
  )
{
  Str entry;
  
  entry.setf( ios::left, ios::adjustfield );
  entry << setw(10) << appName
	<< ' '
    ;
  
  entry.setf( ios::internal, ios::adjustfield );
  entry << setw(4) << passNum + 1  << " of " << setw(4) << passes 
	<< ' '
    ;
  
  entry.setf( ios::left, ios::adjustfield );
  entry << setw(24) << mapType
	<< ' '
    ;
  
  entry.setf( ios::internal, ios::adjustfield );
  entry << setw( 5 ) << initSize
	<< ' '
	<< setw( 4) << allocSize
	<< ' '
	<< setw( 4 ) << recSize
	<< ' '
    ;

  entry.setf( ios::left, ios::adjustfield );
  entry << setw( 24 ) << className
	<< ' '
	<< setw( 10 ) << method
	<< ' '
	<< setw( 5 ) << order
	<< ' '
    ;
  
  entry.setf( ios::internal, ios::adjustfield );
  entry << setw( 8 ) << startSize
	<< ' '
	<< setw( 8 ) << quantity
	<< ' '
    ;
  
  entry.setf( ios::internal, ios::adjustfield );

  dest << entry << timer << endl;

  
  _LLg( LogLevel::Debug );

  if( _LibLog && _LibLog->willOutput( LogLevel::Debug ) )
    {
      (*_LibLog) << entry << timer << '\n' << timer.dump() << endl;
    }
      
}


// Revision Log:
//
// $Log$
// Revision 2.2  2000/05/27 14:32:04  houghton
// Port: Sun CC 5.0.
//
// Revision 2.1  1997/09/21 19:22:41  houghton
// Changed version to 2
//
// Revision 1.4  1997/07/25 13:51:18  houghton
// Reworked to ouptut more detail.
//
// Revision 1.3  1997/07/21 10:27:08  houghton
// Changed to use LibLog for debug loging.
//
// Revision 1.2  1997/07/14 10:51:17  houghton
// Added detail info output to AppDebug.
//
// Revision 1.1  1997/07/13 11:36:39  houghton
// Initial Version.
//
//
