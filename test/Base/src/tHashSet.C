//
// File:        tHash.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tHash
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/02/97 05:56
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <LibTest.hh>

bool	tHashSet01( LibTest & tester );

bool
tHashSet( LibTest & tester )
{
  TESTP( tHashSet01( tester ) );

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 2.2  1997/07/25 13:49:41  houghton
// Changed: Dash was renamed to DashSet.
// Changed: Hash was renamed to HashSet.
//
// Revision 2.1  1997/07/11 17:39:07  houghton
// Initial Version.
//
//
