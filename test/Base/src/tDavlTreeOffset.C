//
// File:        tDavlTreeOffset.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDavlTreeOffset
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/11/97 11:22
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <LibTest.hh>

bool	tDavlTreeOffset01( LibTest & tester );
// bool	tDavlTreeOffset02( LibTest & tester );

bool
tDavlTreeOffset( LibTest & tester )
{
  TESTP( tDavlTreeOffset01( tester ) );
  // TESTP( tDavlTreeOffset02( tester ) );

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 2.1  1997/07/11 17:38:59  houghton
// Initial Version.
//
//
