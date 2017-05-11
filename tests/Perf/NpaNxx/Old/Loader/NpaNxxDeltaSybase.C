//
// File:        NpaNxxDeltaSybase.C
// Desc:        
//
//  
//  
// Locationor:      Igor Chudov - (ichudov@shoe.wiltel.com)
// Created:     06/30/95 11:49 
//
// Revision History:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:47  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:22  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 12:52:42  houghton
// Initial Version (work in progess).
//
// Revision 1.4  1996/08/16  18:35:58  ichudov
// phase 2b
//
// Revision 1.3  1996/07/29  14:23:19  rjankir
// Added dummy entry for location 999999. Actually this
// dummy entry is to to support the dummy entry
// put in the TrunkLoader for Calls with not final and term
// trunk.
//
// Revision 1.2  1995/08/29  22:18:02  ichudov
// Caribbean loading.
//
// Revision 1.1  1995/08/25  22:13:10  ichudov
// New module added.
//
//
static const char * RcsId =
"$Id$";

#include "NpaNxxDeltaSybase.hh"
#include <LocationRec.hh>
#include <MakeDateFromSybase.hh>

static const char blanks[] = { ' ', '\t', '\n', '\r', '\0' };

static 
Bool
isBlank( const char * str )
{
  while( *str )
    {
      if( strchr( blanks, *str ) == 0 )
        {
          return( FALSE );
        }

      str++;
    }

  return( TRUE );
}

static
TZOffset_t BellcoreOffsets[] = {
    0 * SEC_PER_HOUR, // 0 = Time Zone Not Applicable
  -11 * SEC_PER_HOUR, // 1 = Samoa
  -10 * SEC_PER_HOUR, // 2 = Hawaii
  - 9 * SEC_PER_HOUR, // 3 = Alaska/Yukon
  - 8 * SEC_PER_HOUR, // 4 = Pacific
  - 7 * SEC_PER_HOUR, // 5 = Mountain
  - 6 * SEC_PER_HOUR, // 6 = Central
  - 5 * SEC_PER_HOUR, // 7 = Eastern
  - 4 * SEC_PER_HOUR, // 8 = Atlantic
  - 3.5 * SEC_PER_HOUR, // 9 = NewFoundLand 
                        // !!!! 3.5 hours difference, not even!!!
};

static
TZOffset_t
convertTZOffset( int dbOffset )
{
  return( BellcoreOffsets[ dbOffset ] );
}

Bool
NpaNxxDeltaSybase::next(void)
{
  int NPA, NXX;
  char NPAstr[6], NXXstr[6];
  char geoStr[6];
  char effDateStr[30];
  int intHonorsDST;

  char * Query =
    "select \n"
    "  npa.nbr_npa, \n"
    "  npa.nbr_nxx, \n"
    "  convert( int, npa.idx_lat ), \n"
    "  npa.nme_lcy, \n"
    "  npa.cde_rgn, \n"
    "  crgn.cat_rgn_rat, \n"
    "  cli.nbr_cor_vrt, \n"
    "  cli.nbr_cor_hor, \n"
    "  convert( char( 10 ), npa.dtx_eff_frm, 103 ), \n"
    "  convert( int, npa.cde_tmx_zon_blc ), \n"
    "  convert( int, npa.ind_dsv_blc ) \n"
    "from \n" 
    "  tdir_hom_npa npa, \n"
    "  tdir_cli     cli, \n"
    "  trgd_rat..tdir_rgn crgn \n"
    "where \n"
    "      npa.cli_swt_hom = cli.idx_cli_cde and \n"
    "      crgn.cde_rgn = npa.cde_rgn \n"
    "       ";

// cout << "Query = " << endl << Query << endl;
    
  sybErr.reset();
  
  Ret_Status ret = SybocExec(&syb, "NpaNxxDeltaSybase", 0, 0, Query,
    SYBOC_STR_FIELD( NPAstr ),
    SYBOC_STR_FIELD( NXXstr ),
    SYBOC_INT_FIELD( lata ),
    SYBOC_STR_FIELD( name ),
    SYBOC_STR_FIELD( regionAbbr ),
    SYBOC_STR_FIELD( geoStr ),
    SYBOC_INT_FIELD( vert ),
    SYBOC_INT_FIELD( horz ),
    SYBOC_STR_FIELD( effDateStr ),
    SYBOC_INT_FIELD( TZOffset ),
    SYBOC_INT_FIELD( intHonorsDST ),
    0 );

  NPA = atoi( NPAstr );
  NXX = atoi( NXXstr );

  npaNxx = NPA * 1000 + NXX;

  geo = LocationRec::makeGeo( geoStr );

  TZOffset = convertTZOffset( TZOffset );

  honorsDST = intHonorsDST; // !!!! Do not know where to get it from.
  MakeDateFromSybase( effDate, effDateStr );

  if(!checkSQL(ret))
    {
      return( FALSE );
    }

  /****************************************************** Sanity check *

  if( isBlank( name ) || isBlank( regionAbbr ) || 
      isBlank( effDateStr ) || NPA == 0 || NXX == 0 ||
      lata == 0 || vert == 0 || horz == 0 )
    {
      return( FALSE );
    }
  

  ****************************************************** Sanity check */


  ++recCount;
  return ( TRUE );
}

ostream & operator << (ostream & dest, const NpaNxxDeltaSybase & input)
{
  cout << "Input: NpaNxx = " << input.getNpaNxx()  
       << " Name =" << input.getName()
       << " Rgn =" << input.getRegionAbbr()
       << " Lata =" << input.getLata()
       << " Vert =" << input.getVert()
       << " Horz =" << input.getHorz()
       << endl;

  return dest;
}




// getClassName - return the name of this class
const char *
NpaNxxDeltaSybase::getClassName( void ) const
{
  return( "NpaNxxDeltaSybase" );
}

// good - return TRUE if no detected errors
Bool
NpaNxxDeltaSybase::good( void ) const
{
  return( DeltaSybase::good() );
}

// error - return a string describing the current state
const char *
NpaNxxDeltaSybase::error( void ) const
{
  static Str errStr;
  errStr.reset();

  errStr << getClassName();

  if( DeltaSybase::good() )
    {
       errStr << ": Ok";
    }
  else
    {
      errStr << DeltaSybase::error() ;
    }

  return( errStr.cstr() );
}


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
