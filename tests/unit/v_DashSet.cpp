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

bool	tDashSet01( LibTest & tester );
bool	tDashSet02( LibTest & tester );
bool	tDashSet03( LibTest & tester );
bool	tDashSet04( LibTest & tester );

bool
tDashSet( LibTest & tester )
{
  TESTP( tDashSet01( tester ) );
  // TESTP( tDash02( tester ) );
  // TESTP( tDash03( tester ) );
  // TESTP( tDash04( tester ) );
  
  return( true );
}

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 2.2  1997/07/25 13:49:35  houghton
// Changed: Dash was renamed to DashSet.
// Changed: Hash was renamed to HashSet.
//
// Revision 2.1  1997/07/22 19:47:02  houghton
// Initial Version.
//
//
