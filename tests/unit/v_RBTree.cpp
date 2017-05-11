//
// File:        tRBTree.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tRBTree
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/03/97 08:40
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <LibTest.hh>

bool	tRBTree01( LibTest & tester );
bool	tRBTree02( LibTest & tester );
bool	tRBTree03( LibTest & tester );

bool
tRBTree( LibTest & tester )
{
  // test 01 is the same as 02, but generates bunches of debuging output.
  // TESTP( tRBTree01( tester ) );
  TESTP( tRBTree02( tester ) );
  TESTP( tRBTree03( tester ) );

  return( true );
}


// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.2  1997/07/14 10:48:56  houghton
// Added tRBTree01 test (but it is commented out).
//
// Revision 2.1  1997/07/11 17:39:31  houghton
// Initial Version.
//
//