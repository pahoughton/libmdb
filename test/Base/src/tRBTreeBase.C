//
// File:        tRBTreeBase.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for tRBTreeBase
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/08/97 12:45
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <LibTest.hh>

bool tRBTreeBase01( LibTest & tester );

bool tRBTreeBase( LibTest & tester )
{
  TESTP( tRBTreeBase01( tester ) );
  
  
  return( true );
}


// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:00  houghton
// Changed version to 2
//
// Revision 1.1  1997/06/05 11:30:12  houghton
// Initial Version.
//
//
