//
// File:        tDash.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDash
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/22/97 08:37
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <LibTest.hh>

bool	tDash01( LibTest & tester );
bool	tDash02( LibTest & tester );
bool	tDash03( LibTest & tester );
bool	tDash04( LibTest & tester );

bool
tDash( LibTest & tester )
{
  TESTP( tDash01( tester ) );
  // TESTP( tDash02( tester ) );
  // TESTP( tDash03( tester ) );
  // TESTP( tDash04( tester ) );
  
  return( true );
}

// Revision Log:
//
// $Log$
// Revision 2.1  1997/07/22 19:47:02  houghton
// Initial Version.
//
//
