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

bool	tHash01( LibTest & tester );

bool
tHash( LibTest & tester )
{
  TESTP( tHash01( tester ) );

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 2.1  1997/07/11 17:39:07  houghton
// Initial Version.
//
//
