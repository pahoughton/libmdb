//
// File:        tAvlTreeOffset.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tAvlTreeOffset
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/30/97 10:20
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <LibTest.hh>

bool	tAvlTreeOffset01( LibTest & tester );
bool	tAvlTreeOffset02( LibTest & tester );

bool
tAvlTreeOffset( LibTest & tester )
{
  TESTP( tAvlTreeOffset01( tester ) );
  TESTP( tAvlTreeOffset02( tester ) );

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 2.1  1997/07/11 17:38:50  houghton
// Initial Version.
//
//
