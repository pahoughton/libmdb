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
}


// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/13 11:36:39  houghton
// Initial Version.
//
//
