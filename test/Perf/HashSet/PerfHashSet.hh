#ifndef _PerfRBSet_hh_
#define _PerfRBSet_hh_
//
// File:        PerfHashSet.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     07/20/97 09:19
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
#include <HashSet.hh>
#include <Perf.hh>

typedef HashSet< Rec_4, HashRec_4, less< Rec_4 > >	    Table_4;
typedef HashSet< Rec_128, HashRec_128, less< Rec_128 > >    Table_128;
typedef HashSet< Rec_512, HashRec_512, less< Rec_512 > >    Table_512;
typedef HashSet< Rec_1024, HashRec_1024, less< Rec_1024 > > Table_1024;


//
// Revision Log:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:16  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/25 13:41:46  houghton
// Initial Version.
//
//
//
#endif // ! def _PerfRBSet_hh_ 

