//
// File:        tDRBTree.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tDRBTree
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 06:06
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <TestConfig.hh>
#include <LibTest.hh>

bool	tDRBTree01( LibTest & tester );
bool	tDRBTree02( LibTest & tester );
// bool	tDRBTree03( LibTest & tester );

bool
tDRBTree( LibTest & tester )
{
  TESTP( tDRBTree01( tester ) );
  TESTP( tDRBTree02( tester ) );

  return( true );
}

// Revision Log:
//
// $Log$
// Revision 2.2  1997/07/19 10:36:57  houghton
// Cleanup.
//
// Revision 2.1  1997/07/16 16:38:54  houghton
// Initial Version (work in progress).
//
//
