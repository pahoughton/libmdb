#ifndef _LocationRec_hh_
#define _LocationRec_hh_
//
// File:        LocationRec.hh
// Desc:        
//              
//
// Locationor:      David Lin x6048
// Created:     05/10/95 15:57
//

#include <DateTime.hh>
#include <StlUtilsMisc.hh>

#include <TeleNum.hh>

#include <RatingTypes.h>
#include <Rating.hh>

#include <Connection.hh>


#define LOC_US		0  // hard coded value from tdir_cat_rgn.
#define LOC_CAN		1
#define LOC_CAR		2
#define LOC_MEX		3
#define LOC_ALA		4
#define LOC_HAW		5
#define LOC_PUE		6
#define LOC_VIR		7
#define LOC_INTL  	8
#define LOC_INTER 	9
#define LOC_INTRA 	10
#define LOC_CAL 	11
#define LOC_INTRALATA 	12
#define LOC_INTERLATA 	13
#define LOC_UNDEFINED   14 // !!!! Don't forget to adjust this if you
                           // plan to add a new location or you will be 
                           // screwed up.

class LocationRec
{

public:

  
  struct Key  
  {
    Location_t   location; // for npa nxx map, it is NPANXX, and
    // for location maps, it is location number. There is a nice API
    // for setting npa, nxx, dialing plan and location, so that you
    // won't get lost. 
  };

  struct Data
  {
    CityName_t		name;
    Geo_t               geo;            // Numeric code for cde_rgn

    RegionId_t          regionId;
    RegionAbbr_t	regionAbbr;	// state abbr
  
    union {
      unsigned short	dialPlan;	// international country codes
      unsigned short 	band;		// Mexico Bands
      unsigned short	lata;		// Us lata
      unsigned short	generic;	// generic value
    } lbd; // lbd keeps various data, and meaning of its content is
	   // defined by information in the location field of the key.
    
    unsigned short	vert;
    unsigned short	horz;

    TZOffset_t          TZOffset;  // timesone offset
    Bool                honorsDST; // Do I honor DST?
  };
 
  struct NameGeo
  {
    const char * name;
    Geo_t        geo;
  };


  LocationRec( void ); // for transfer
  LocationRec( Location_t       number,
	       const char * 	name = 0,
	       Geo_t    	geo = 0xffff,
	       const char * 	regionAbbr = 0,
	       unsigned short   lataBandDialPlan = 0,
	       unsigned short	vert = 0,
	       unsigned short	horz = 0,
	       TZOffset_t       TZOffset = 0,
	       Bool             DSTHonored = 0 );
  
  LocationRec( Key *locKey, Data * locData );
  LocationRec( const Key & locKey, const Data & locData );
  LocationRec( const TeleNum & tele );

  

  ~LocationRec( void );

  Bool	    	    isValidKey( void ) const;
  Bool	    	    isValid( void ) const;

  unsigned short 	setGeo( unsigned short newGeo );
  RegionId_t            setRegionId( RegionId_t ); // returns previous one
  
  unsigned short	getNpa( void ) const;
  unsigned short	getNxx( void ) const;
  unsigned long	 	getNumber( void ) const;
  
  const char *		getName( void ) const;
  unsigned short	getRegionId( void ) const;
  unsigned short	getLata( void  ) const;
  unsigned short	getDialPlan( void ) const;
  unsigned short	getBand( void ) const;
  unsigned short        getLbd( void ) const;
  unsigned short	getVert( void ) const;
  unsigned short	getHorz( void ) const;
  unsigned short 	getGeo( void ) const;
  const char *		getRegionAbbr( void ) const;

  TZOffset_t            getTZOffset( void ) const;
  Bool                  isDSTHonored( void ) const;

  inline static RegionId_t makeRegionId( const char * regionName );
  // Makes regionId as regionName[0] * 256 + regionName[1]
  inline static char * getRegionIdStr( RegionId_t id, char * buf = 0 );
  
  static Geo_t      makeGeo( const char * catRgn );
  // Makes geo by simple rules such as "CAN" --> LOC_CAN
  
  static const char * GeoToChar ( int geo );
  // Make legend by simple rules such as LOC_CAN --> "CAN"

  static int	    compareKey( const Key & one, const Key & two );
  static Key &      copyKey(Key & dest, const Key & src);
  static Data &	    copyData( Data & dest, const Data & src );
  
  const Data & 	    getData( void ) const;
  Data & 	    getData( void );
  const Key &       getKey( void ) const;
  Key &             getKey( void );

  const Data * 	    getDataAddr( void ) const;
  Data * 	    getDataAddr( void );
  const Key *       getKeyAddr( void ) const;
  Key *             getKeyAddr( void );

  Bool	    	    good( void ) const;
  const char *	    error( void ) const;

  Bool operator ==( const LocationRec & equalTo ) const;
  Bool operator !=( const LocationRec & notEqualTo ) const;

  static const NameGeo geos[];

  virtual Ret_Status transfer( Connection & conn,
                               MessageSize_t * nbytes,
                               Direction dir );

  virtual MessageType_t getMsgType( void ) const { return OT_LOCATION_REC; }


protected:

private:

  LocationRec( const LocationRec & copyFrom );
  LocationRec & operator = ( const LocationRec & assignFrom );
  
  Bool	    needToDelete;   

  Key *     key;
  Data *    data;
  
};

inline ostream & operator<<( ostream & dest, const LocationRec & loc );


//
// Inline methods
//

inline
LocationRec::LocationRec (
    Location_t   	number,
    const char *	name,
    Geo_t	        geo,
    const char * 	regionAbbr,
    unsigned short	lataBandDialPlan,
    unsigned short	vert,
    unsigned short	horz,
    TZOffset_t          TZOffset,
    Bool                honorsDST )
{
  key = new Key;
  data = new Data;
  needToDelete = TRUE;

  key->location = number;
   
  SAFE_STRNCPY( data->name, name );
  SAFE_STRNCPY( data->regionAbbr, regionAbbr );

  data->geo 		= geo;

  data->regionId        = makeRegionId( regionAbbr );
  data->lbd.generic	= lataBandDialPlan;
  data->vert		= vert;
  data->horz		= horz;

  data->TZOffset = TZOffset;
  data->honorsDST = honorsDST;
}

inline
LocationRec::LocationRec( void )
{
  key = new Key;
  data = new Data;
  needToDelete = TRUE;
  // do not fill in the data!
}

inline
LocationRec::LocationRec( const Key & locKey, const Data & locData )
{
  needToDelete = TRUE;
  key = new Key;
  data = new Data;

  key->location = locKey.location;
  memcpy( data, &locData, sizeof( Data ) );
}



inline
LocationRec::LocationRec( const TeleNum & tele )
{
  key = new Key;
  data = new Data;
  needToDelete = TRUE;

  key->location = atoi( tele.getTele() );
  // Do not need to set the data
}

inline
LocationRec::LocationRec( Key * locKey, Data * locData )
{
  needToDelete = FALSE;

  key  = locKey;
  data = locData;
}

inline
LocationRec::~LocationRec( void )
{
  if( needToDelete )
    {
      if(key)  delete key;
      if(data) delete data;
    }
}


inline
int
LocationRec::compareKey( const Key & one, const Key & two )
{
  return( ( one.location < two.location ) ? -1 :
	  ( one.location == two.location ) ? 0 : 1 );
}

inline
LocationRec::Key &
LocationRec::copyKey( Key & dest, const Key & src )
{
  dest.location = src.location;

  return( dest );
}

inline
LocationRec::Data &
LocationRec::copyData( Data & dest, const Data & src)
{
  memcpy( &dest, &src, sizeof( dest ) );
  
  return(dest);
}


inline
const LocationRec::Data &
LocationRec::getData( void ) const
{
  return( *data );
}

inline
LocationRec::Data &
LocationRec::getData( void )
{
  return( *data );
}

inline
const LocationRec::Data *
LocationRec::getDataAddr( void ) const
{
  return( data );
}

inline
LocationRec::Data *
LocationRec::getDataAddr( void )
{
  return( data );
}

inline
const LocationRec::Key &
LocationRec::getKey( void ) const
{
  return( *key );
}

inline
LocationRec::Key &
LocationRec::getKey( void )
{
  return( *key );
}

inline
const LocationRec::Key *
LocationRec::getKeyAddr( void ) const
{
  return( key );
}

inline
LocationRec::Key *
LocationRec::getKeyAddr( void )
{
  return( key );
}

inline int
LocationRec::isValidKey( void ) const
{
  if( key != 0 )
    {
      if( key->location != 0 )
	{
	  return( TRUE );
	}
    }

  return( FALSE );
}

inline int
LocationRec::isValid( void ) const
{
  if( isValidKey() && data->geo != 0xffff )
    {
      return( TRUE );
    }
    
  return( FALSE );
}


inline
unsigned short
LocationRec::setGeo( unsigned short newGeo )
{
  unsigned short oldGeo = data->geo;
  data->geo = newGeo;
  
  return( oldGeo );
}

inline
const char *
LocationRec::getName( void  ) const
{
  return( data->name );
}

inline
RegionId_t
LocationRec::makeRegionId( const char * regionName )
{
  return( regionName[0] * 0x0100 + regionName[1] );
}


inline
RegionId_t
LocationRec::getRegionId( void ) const
{
  return( data->regionId );
}

inline
unsigned short
LocationRec::getLata( void ) const
{
  return( data->lbd.lata );
}

inline
unsigned short
LocationRec::getDialPlan( void  ) const
{
  return( data->lbd.dialPlan );
}

inline
unsigned long
LocationRec::getNumber( void ) const
{
   return( key->location );
}

inline
unsigned short
LocationRec::getNpa( void  ) const
{
  return( key->location / 1000 );
}

inline
unsigned short
LocationRec::getNxx( void  ) const
{
  return( key->location % 1000 );
}

inline
unsigned short
LocationRec::getBand( void ) const
{
  return( data->lbd.band );
}

inline
unsigned short
LocationRec::getLbd( void ) const
// Essentially the same as getBand, but I want to keep  the code clean.
{
  return( data->lbd.generic );
}

inline
unsigned short
LocationRec::getVert( void  ) const
{
  return( data->vert );
}

inline
unsigned short
LocationRec::getHorz( void  ) const
{
  return( data->horz );
}

inline
unsigned short
LocationRec::getGeo( void ) const
{
  return( data->geo );
}

inline
const char *
LocationRec::getRegionAbbr( void ) const
{
  return( data->regionAbbr );
}

inline 
char * 
LocationRec::getRegionIdStr( RegionId_t id, char * buf )
{
  static char mybuf[3];
  char * p = buf ? buf : mybuf;
  char * p0 = p;

  *p++ = (id >> 8);
  *p++ = (id & 0x00FF);
  *p = '\0';

  return( p0 );
}


inline
TZOffset_t
LocationRec::getTZOffset( void ) const
{
  return( data->TZOffset );
}

inline
Bool
LocationRec::isDSTHonored( void ) const
{
  return( data->honorsDST );
}

RegionId_t
LocationRec::setRegionId( RegionId_t id )
// returns previous one
{
  RegionId_t old = getRegionId();
  data->regionId = id;
  return( old );
}


inline
Bool
LocationRec::operator ==( const LocationRec & equalTo ) const
{
  if( compareKey( getKey(), equalTo.getKey() ) != 0 )
    {
      return( FALSE );
    }
  
  return(
    !strcmp( data->name, equalTo.getName() ) &&
    data->geo == equalTo.getGeo()            &&
    getRegionId() == equalTo.getRegionId()   &&
    data->lbd.generic == equalTo.getLbd()    &&
    data->vert == equalTo.getVert()          &&
    data->horz == equalTo.getHorz()          &&
    getTZOffset() == equalTo.getTZOffset()   &&
    isDSTHonored() == equalTo.isDSTHonored() );
}

inline
Bool
LocationRec::operator !=( const LocationRec & notEqualTo ) const
{
  return( !(*this == notEqualTo ) );
}


inline
Bool
LocationRec::good( void ) const
{
  return( isValid() );
}

inline
const char *
LocationRec::error( void ) const
{
  if( good() )
    {
      return( "LocationRec: Ok" );
    }
  else
    {
      if( ! isValidKey() )
	{
	  return( "LocationRec: bad loc id" );
	}
      else
	{
	  return( "LocationRec: bad plan id" );
	}
    }
}


inline
ostream &
operator<<( ostream & dest, const LocationRec & loc )
{
  if( ! loc.good() )
    {
      dest << loc.error() << ' ';
    }

  dest << "Location: "
       << "DP, lata, band: " << loc.getDialPlan() << ' '
       << "#: " << loc.getKey().location << ' '
       << "Geo: " << loc.getGeo() << ' '
       << setiosflags( ios::left ) << setw( 12 )
       << loc.getName() << setiosflags( ios::internal ) << ' '
       << "R: " << setw(3) << loc.getRegionId() << ' '
       << "RN: " << loc.getData().regionAbbr << ' '
       << endl
       << "LBD: " << setw(3) << loc.getLata() << ' '
       << "V: " << setw(4) << loc.getVert() << ' '
       << "H: " << setw(4) << loc.getHorz() << ' ' 
       << "TZOffset: " << (int) loc.getTZOffset() << ' '
       << "DST? " << (loc.isDSTHonored() ? "TRUE" : "FALSE" );
           ;

  return( dest );
}




// Revision History:
//
// 
// $Log$
// Revision 2.2  2000/05/27 14:32:04  houghton
// Port: Sun CC 5.0.
//
// Revision 2.1  1997/09/21 19:22:40  houghton
// Changed version to 2
//
// Revision 1.2  1997/09/17 16:56:22  houghton
// Changed for new library rename to StlUtils
//
// Revision 1.1  1997/07/19 12:52:04  houghton
// Initial Version (work in progess).
//
// Revision 1.10  1996/04/29  15:46:33  ichudov
// Changes related to Phase IIa of Rating Split.
//
// Revision 1.9  1996/02/07  17:27:12  ichudov
// Rating Split Phase I.
//
// Revision 1.8  1995/11/07  19:05:38  rjankir
// Added function to display legend
//
// Revision 1.7  1995/09/27  15:16:40  ichudov
// Committed changes after struggling with CVS.
//
// Revision 1.5.2.5  1995/09/25  19:43:00  ichudov
// Many changes for ITFS, 700 1+ calls, enhanced 800 (putting 24 in infodig),
// and 7-digit called numbers.
//
// Revision 1.5.2.4  1995/08/29  20:08:18  ichudov
// Zero Tolerance.
//
// Revision 1.5.2.2  1995/08/21  02:30:14  ichudov
// Reconciling my and David's zero tol versions.
//
// Revision 1.5.2.1  1995/08/19  16:28:56  bcooper
// Development versions of libRating for Zero Tolerance/Pristine
// 	-- find call start time on 800# calls from terminating side of call
// 	-- rate 14-digit auth codes
//
// Revision 1.5  1994/12/30  21:13:21  rjankir
// Implemented getNumber - needed for unratables
//

#endif // ! def _LocationRec_hh_ 
