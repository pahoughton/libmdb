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
bool	tHash( LibTest & tester );
bool	tRBTree( LibTest & tester );
bool	tDRBTree( LibTest & tester );

bool tHashTable( LibTest & tester );

// Revision Log:
//
// $Log$
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

