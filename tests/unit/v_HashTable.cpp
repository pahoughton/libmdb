//
// File:        tHashTable.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tHashTable
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/19/97 08:53
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <LibTest.hh>

bool tHashTable01( LibTest & tester );

bool tHashTable( LibTest & tester )
{
  TESTP( tHashTable01( tester ) );
  
  return( true );
}

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:00  houghton
// Changed version to 2
//
// Revision 1.1  1997/06/05 11:30:11  houghton
// Initial Version.
//
//
