#ifndef _TestConfig_hh_
#define _TestConfig_hh_
//
// File:        TestConfig.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/18/97 06:19
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <MdbConfig.hh>

#if defined( TEST_DATA_DIR )
#error test data dir defined
#endif

#define TEST_DATA_DIR	"../data"

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:44  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:21:45  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 10:34:40  houghton
// Initial Version.
//
//
#endif // ! def _TestConfig_hh_ 

