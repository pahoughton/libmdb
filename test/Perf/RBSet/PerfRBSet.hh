#ifndef _PerfRBSet_hh_
#define _PerfRBSet_hh_
//
// File:        PerfRBSet.hh
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
#include <RBSet.hh>
#include <Perf.hh>

typedef RBSet< Rec_4, less< Rec_4 > >	    Table_4;
typedef RBSet< Rec_128, less< Rec_128 > >   Table_128;
typedef RBSet< Rec_512,	less< Rec_512 > >   Table_512;
typedef RBSet< Rec_1024, less< Rec_1024 > > Table_1024;


//
// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/21 10:29:40  houghton
// Initial version.
//
//
#endif // ! def _PerfRBSet_hh_ 

