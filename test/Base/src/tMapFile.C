//
// File:        tMapFile.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tMapFile
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 08:03
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "TestConfig.hh"
#include <LibTest.hh>

bool tMapFile01( LibTest & tester );
bool tMapFile02( LibTest & tester );
bool tMapFile03( LibTest & tester );

bool
tMapFile( LibTest & tester )
{
  TESTP( tMapFile01( tester ) );
  TESTP( tMapFile02( tester ) );
  TESTP( tMapFile03( tester ) );

  return( true );
}

    
// Revision Log:
//
// $Log$
// Revision 2.1  1997/07/16 16:38:55  houghton
// Initial Version (work in progress).
//
//
