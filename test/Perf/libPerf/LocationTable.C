//
// File:        LocationTable.C
// Desc:        
//              
//
// Locationor:      Paul Houghton x2309 - (houghton@shoe.wiltel.com)
// Created:     01/27/95 14:11 
//
// Revision History:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:40  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 12:52:04  houghton
// Initial Version (work in progess).
//
// Revision 1.6  1995/09/27  15:16:41  ichudov
// Committed changes after struggling with CVS.
//
// Revision 1.4.2.2  1995/08/29  23:36:35  ichudov
// Production release of ZT.
//
// Revision 1.4  1995/07/20  14:21:57  ichudov
// DAVLs; Fixed precision for tolls.
//

static const char * RcsId =
"$Id$";

#include "LocationTable.hh"

LocationTable *LocationTable::pGlobalNpaNxxTable = 0;
LocationTable *LocationTable::pGlobalDialPlanTable = 0;


//
//              This software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1994 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//
