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

#include <LibLog.hh>
#include <TimeIt.hh>
#include <iostream>
#include <iomanip>

void
LogPerfData(
  ostream &	    dest,
  const char *	    mapType,
  const char *	    className,
  const char *	    method,
  const char *	    order,
  long		    initAlloc,
  long		    recSize,
  long		    quantity,
  const TimeIt &    timer
  )
{
  dest.setf( ios::left, ios::adjustfield );
  dest << setw(20) << mapType
       << " "
       << setw(20) << className
       << " "
    ;
  
  dest.setf( ios::internal, ios::adjustfield );
  dest << setw( 5 ) << initAlloc
       << " "
       << setw( 4 ) << recSize
       << " "
       << setw( 8 ) << quantity
       << " "
    ;
  dest.setf( ios::left, ios::adjustfield );
  dest << setw(10) << method
       << " "
       << setw(5) <<  order
       << " "
    ;
  dest.setf( ios::internal, ios::adjustfield );
  dest << timer
       << endl;

  _LLg( LogLevel::Debug );

  if( _LibLog && _LibLog->willOutput( LogLevel::Debug ) )
    {
      (*_LibLog).setf( ios::left, ios::adjustfield );
      (*_LibLog) << setw(20) << mapType
		 << " "
		 << setw(20) << className
		 << " "
	;
      (*_LibLog).setf( ios::internal, ios::adjustfield );
      (*_LibLog) << setw( 5 ) << initAlloc
		 << " "
		 << setw( 4 ) << recSize
		 << " "
		 << setw( 8 ) << quantity
		 << " "
	;
      (*_LibLog).setf( ios::left, ios::adjustfield );
      (*_LibLog) << setw(10) << method
		 << " "
		 << setw(5) <<  order
		 << " "
	;
      (*_LibLog).setf( ios::internal, ios::adjustfield );
      (*_LibLog) << timer.dump()
		 << endl;
    }
      
}


// Revision Log:
//
// $Log$
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
