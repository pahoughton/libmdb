#ifndef _TestFunctProto_hh_
#define _TestFunctProto_hh_
//
// File:        TestFunctProto.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/08/97 12:43
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//


bool	tMapFile( LibTest & tester );
bool	tMapMemDynamicFixed( LibTest & tester );
bool	tMapMemDynamicDynamic( LibTest & tester );
bool	tAvlTreeOffset( LibTest & tester );
bool	tDavlTreeOffset( LibTest & tester );
bool	tHashSet( LibTest & tester );
bool	tDashSet( LibTest & tester );
bool	tRBTree( LibTest & tester );
bool	tRBSet( LibTest & tester );
bool	tDRBTree( LibTest & tester );

bool tHashTable( LibTest & tester );

bool	tMapMemDlist( LibTest & tester );

// Revision Log:
//
// $Log$
// Revision 2.2  1998/10/23 13:22:09  houghton
// Added MapMemDList test.
//
// Revision 2.1  1997/09/21 19:21:59  houghton
// Changed version to 2
//
// Revision 1.6  1997/07/25 13:49:27  houghton
// Changed: Dash was renamed to DashSet.
// Changed: Hash was renamed to HashSet.
//
// Revision 1.5  1997/07/22 19:45:46  houghton
// Added Dash test.
//
// Revision 1.4  1997/07/16 16:38:25  houghton
// Added MapFile & DRBTree tests.
//
// Revision 1.3  1997/07/13 11:35:05  houghton
// Added many test.
//
// Revision 1.2  1997/06/27 12:16:04  houghton
// Added tMapMemDynamicDynamic.
//
// Revision 1.1  1997/06/05 11:30:11  houghton
// Initial Version.
//
//
#endif // ! def _TestFunctProto_hh_ 

