#ifndef _NpaNxxDeltaSybase_hh_
#define _NpaNxxDeltaSybase_hh_
//
// File:        NpaNxxDeltaSybase.hh
// Desc:        
//
//
//  Quick Start: - short example of class usage
//
//  Data Types: - data types defined by this header
//
//  	NpaNxxDeltaSybase	class
//
//  Constructors:
//
//  	NpaNxxDeltaSybase( );
//
//  Destructors:
//
//  Public Interface:
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. NpaNxxDeltaSybase )
//
//  	virtual Bool
//  	good( void ) const;
//  	    Returns true if there are no detected errors associated
//  	    with this class, otherwise FALSE.
//
//  	virtual const char *
//  	error( void ) const
//  	    Returns as string description of the state of the class.
//
//  Protected Interface:
//
//  Private Methods:
//
//  Other Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const NpaNxxDeltaSybase & obj );
//
// Locationor:      Igor Chudov - (ichudov@shoe.wiltel.com)
// Created:     06/30/95 11:24
//
// Revision History:
//
// $Log$
// Revision 2.1  1997/09/21 19:22:22  houghton
// Changed version to 2
//
// Revision 1.2  1997/09/17 16:56:19  houghton
// Changed for new library rename to StlUtils
//
// Revision 1.1  1997/07/19 12:52:43  houghton
// Initial Version (work in progess).
//
// Revision 1.1  1995/08/25  22:13:11  ichudov
// New module added.
//
//

#include <StlUtils.hh>
#include <Str.hh>
#include <DeltaSybase.hh>
#include <TeleNum.hh>
#include <DateTime.hh>

#include <LocationRec.hh> // For some type definitions only

class NpaNxxDeltaSybase : public DeltaSybase
{

public:

  NpaNxxDeltaSybase(const char * sybServer,
                  const char * sybDatabase,
                  const char * sybUser,
                  const char * sybPassword );

  const int     getRecCount(void) const { return recCount; }

  const char	getType(void) const  { return 'A' ; }

  Bool  next(void);

  RecType           getRecType( void ) const;


  unsigned int          getNpaNxx( void ) const;
  
  const char *		getName( void ) const;
  unsigned short	getRegion( void ) const;
  unsigned short	getLata( void  ) const;
  unsigned short        getLbd( void ) const;
  unsigned short	getVert( void ) const;
  unsigned short	getHorz( void ) const;
  unsigned short 	getGeo( void ) const;
  const char *		getRegionAbbr( void ) const;

  TZOffset_t            getTZOffset( void ) const;
  Bool                  isDSTHonored( void ) const;
  
  const DateTime &      getEffDate( void ) const;

  
  friend ostream & operator << ( ostream & dest, const NpaNxxDeltaSybase & input);

  virtual const char *	getClassName( void ) const;
  virtual Bool	    	good( void ) const;
  virtual const char * 	error( void ) const;

protected:

private:

  NpaNxxDeltaSybase( const NpaNxxDeltaSybase & copyFrom );
  NpaNxxDeltaSybase & operator=( const NpaNxxDeltaSybase & assignFrom );
  
  int         recCount;
  RecType     recType;

  int                   npaNxx;
  char	       	        name[40];
  unsigned short	geo;
  unsigned short	regionId;
  char                  regionAbbr[3];	// state abbr

  int lata;		// Us lata
  int vert;
  int horz;

  int        TZOffset;
  int /* Bool */ honorsDST;

  DateTime  effDate;
  
};

//
// Inline functions
//


NpaNxxDeltaSybase::NpaNxxDeltaSybase( 
                   const char * sybServer,
                   const char * sybDatabase,
                   const char * sybUser,
                   const char * sybPassword ) :
  DeltaSybase( sybServer, sybDatabase, sybUser, sybPassword )
{	
  // Nothing
}

inline
Delta::RecType
NpaNxxDeltaSybase::getRecType( void ) const
{
  return( recType );
}

inline
const char *
NpaNxxDeltaSybase::getName( void  ) const
{
  return( name );
}

inline
unsigned short
NpaNxxDeltaSybase::getRegion( void ) const
{
  return( regionId );
}

inline
unsigned short
NpaNxxDeltaSybase::getLata( void ) const
{
  return( lata );
}

inline
unsigned int
NpaNxxDeltaSybase::getNpaNxx( void  ) const
{
  return( npaNxx );
}

inline
unsigned short
NpaNxxDeltaSybase::getVert( void  ) const
{
  return( vert );
}

inline
unsigned short
NpaNxxDeltaSybase::getHorz( void  ) const
{
  return( horz );
}

inline
unsigned short
NpaNxxDeltaSybase::getGeo( void ) const
{
  return( geo );
}

inline
const char *
NpaNxxDeltaSybase::getRegionAbbr( void ) const
{
  return( regionAbbr );
}

inline
TZOffset_t
NpaNxxDeltaSybase::getTZOffset( void ) const
{
  return( TZOffset );
}

inline
Bool
NpaNxxDeltaSybase::isDSTHonored( void ) const
{
  return( honorsDST );
}



inline
const DateTime &
NpaNxxDeltaSybase::getEffDate( void ) const
{
  return( effDate );
}


#endif // ! def _NpaNxxDeltaSybase_hh_
//
//              This software is the sole property of
//
//                 The Williams Complocationes, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1995 by The Williams Complocationes, Inc.
//
//                      All Rights Reserved.
//
//

