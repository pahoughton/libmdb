//
// File:        LocationRec.C
// Desc:        
//
//  
//  
// Author:      Igor Chudov @ work - (ichudov@shoe.wiltel.com)
// Created:     08/19/95 15:24 
//
// Revision History:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:47  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:39  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 12:52:04  houghton
// Initial Version (work in progess).
//
// Revision 1.9  1996/04/29  15:46:32  ichudov
// Changes related to Phase IIa of Rating Split.
//
// Revision 1.8  1996/02/07  17:27:09  ichudov
// Rating Split Phase I.
//
// Revision 1.7  1995/11/07  19:06:23  rjankir
// Added GeoToChar
//
// Revision 1.6  1995/09/27  15:16:38  ichudov
// Committed changes after struggling with CVS.
//
// Revision 1.4.2.6  1995/08/29  23:36:33  ichudov
// Production release of ZT.
//
// Revision 1.4.2.3  1995/08/21  20:00:18  ichudov
// More reconciling with Mr. Lin.
//
// Revision 1.4.2.2  1995/08/21  02:30:13  ichudov
// Reconciling my and David's zero tol versions.
//
// Revision 1.4.2.1  1995/08/19  16:28:55  bcooper
// Development versions of libRating for Zero Tolerance/Pristine
// 	-- find call start time on 800# calls from terminating side of call
// 	-- rate 14-digit auth codes
//
// Revision 1.4  1994/12/30  21:14:42  rjankir
// Constructor which takes telenum, should null geo. Other wise record is
// valid. Fixed it.
//
// Revision 1.3  1994/08/16  13:04:16  houghton
// Fix Id vars for ident
//
// Revision 1.2  1994/06/23  13:51:59  houghton
// Latests version of rating - added descriptions
// add sds call type, added e800 calltype
//
static const char * RcsId =
"$Id$";

#include "LocationRec.hh"

LocationRec::NameGeo 
const
LocationRec::geos[] = {
  // IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  // SHOULD BE IN ALPHABETICAL ORDER!
  // Otherwise bsearch won't work correctly.
  // You have been warned.
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  { "ALA", LOC_ALA },
  { "CAL", LOC_CAL },
  { "CAN", LOC_CAN },
  { "CAR", LOC_CAR },
  { "HAW", LOC_HAW },
  { "ILA", LOC_INTRALATA },
  { "ILE", LOC_INTERLATA },
  { "INA", LOC_INTRA },
  { "INE", LOC_INTER },
  { "INT", LOC_INTL },
  { "MEX", LOC_MEX },
  { "PUE", LOC_PUE },
  { "US",  LOC_US },
  { "VIR", LOC_VIR },
  { 0, 0 }
};


extern "C"
int
compareGeos( const LocationRec::NameGeo * geo1, 
             const LocationRec::NameGeo * geo2 )
{
  return( strcmp( geo1->name, geo2->name ) );
}

Ret_Status 
LocationRec::transfer( Connection & conn,
                       MessageSize_t * nbytes,
                       Direction dir )
{
  conn.transferUINT32( key->location, nbytes, dir );
  conn.transfer( data->name, sizeof( data->name ), nbytes, dir );
  conn.transferUINT16( data->geo, nbytes, dir );
  conn.transferUINT16( data->regionId, nbytes, dir );
  conn.transfer( data->regionAbbr, sizeof( data->regionAbbr ), 
                 nbytes, dir );
  conn.transferUINT16( data->lbd.generic, nbytes, dir );
  conn.transferUINT16( data->vert, nbytes, dir );
  conn.transferUINT16( data->horz, nbytes, dir );
  conn.transferINT32( data->TZOffset, nbytes, dir );
  return( conn.transferBool  ( data->honorsDST, nbytes, dir ) );
}

Geo_t
LocationRec::makeGeo( const char * catRgn )
// Makes geo by simple rules such as "CAN" --> LOC_CAN
{

  for( int i = 0; geos[i].name != 0; i++ )
    {
      if( !strcmp( geos[i].name, catRgn ) ) 
        {
          return( geos[i].geo );
        }
    }
  
  return( LOC_UNDEFINED );
}

const char * 
LocationRec::GeoToChar ( int geo )
{
  for( int i = 0; geos[i].name != 0; i++ )
    {
      if( geos[i].geo == geo )
        {
          return( geos[i].name );
        }
    }
 
  return( "UND" );
}

//
//              This software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1995 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//
