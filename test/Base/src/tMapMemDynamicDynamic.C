//
// File:        tMapMemDynamicDynamic.C
// Project:	Mdb
// Desc:        
//
//  Test for MapMemDynamicDynamic class
//
// Source Header Version: 1.9
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/27/97 06:19
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "TestConfig.hh"
#include "LibTest.hh"

bool	tMapMemDynamicDynamic01( LibTest & tester );
bool	tMapMemDynamicDynamic02( LibTest & tester );

bool
tMapMemDynamicDynamic( LibTest & tester )
{
  TESTP( tMapMemDynamicDynamic01( tester ) );
  TESTP( tMapMemDynamicDynamic02( tester ) );

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/07/11 17:39:14  houghton
// Initial Version.
//
//
