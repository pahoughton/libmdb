//
// File:        tMapBatch.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapBatch
//  
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     06/29/00 10:00
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "TestConfig.hh"
#include <LibTest.hh>

bool tMapBatch01( LibTest & tester );

bool
tMapBatch( LibTest & tester )
{
  TESTP( tMapBatch01( tester ) );

  return( true );
}

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 1.1  2000/07/31 13:06:58  houghton
// Initial Version.
//
//
