#ifndef _CARNpaNxxDeltaSybase_hh_
#define _CARNpaNxxDeltaSybase_hh_
//
// File:        CARNpaNxxDeltaSybase.hh
// Desc:        
//
//
//  Quick Start: - short example of class usage
//
//  Data Types: - data types defined by this header
//
//  	CARNpaNxxDeltaSybase	class
//
//  Constructors:
//
//  	CARNpaNxxDeltaSybase( );
//
//  Destructors:
//
//  Public Interface:
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. CARNpaNxxDeltaSybase )
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
//  	operator <<( ostream & dest, const CARNpaNxxDeltaSybase & obj );
//
// Locationor:      Igor Chudov - (ichudov@shoe.wiltel.com)
// Created:     06/30/95 11:24
//
// Revision History:
//
// $Log$
// Revision 1.1  1997/07/19 12:52:41  houghton
// Initial Version (work in progess).
//
// Revision 1.1  1995/08/29  22:18:00  ichudov
// Caribbean loading.
//
// Revision 1.1  1995/08/25  22:13:11  ichudov
// New module added.
//
//

#include <Clue.hh>
#include <Str.hh>
#include <DeltaSybase.hh>
#include <TeleNum.hh>
#include <DateTime.hh>

#include <LocationRec.hh> // For some type definitions only

class CARNpaNxxDeltaSybase : public DeltaSybase
{

public:

  CARNpaNxxDeltaSybase(const char * sybServer,
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

  
  friend ostream & operator << ( ostream & dest, const CARNpaNxxDeltaSybase & input);

  virtual const char *	getClassName( void ) const;
  virtual Bool	    	good( void ) const;
  virtual const char * 	error( void ) const;

protected:

private:

  CARNpaNxxDeltaSybase( const CARNpaNxxDeltaSybase & copyFrom );
  CARNpaNxxDeltaSybase & operator=( const CARNpaNxxDeltaSybase & assignFrom );
  
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


CARNpaNxxDeltaSybase::CARNpaNxxDeltaSybase( 
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
CARNpaNxxDeltaSybase::getRecType( void ) const
{
  return( recType );
}

inline
const char *
CARNpaNxxDeltaSybase::getName( void  ) const
{
  return( name );
}

inline
unsigned short
CARNpaNxxDeltaSybase::getRegion( void ) const
{
  return( regionId );
}

inline
unsigned short
CARNpaNxxDeltaSybase::getLata( void ) const
{
  return( lata );
}

inline
unsigned int
CARNpaNxxDeltaSybase::getNpaNxx( void  ) const
{
  return( npaNxx );
}

inline
unsigned short
CARNpaNxxDeltaSybase::getVert( void  ) const
{
  return( vert );
}

inline
unsigned short
CARNpaNxxDeltaSybase::getHorz( void  ) const
{
  return( horz );
}

inline
unsigned short
CARNpaNxxDeltaSybase::getGeo( void ) const
{
  return( geo );
}

inline
const char *
CARNpaNxxDeltaSybase::getRegionAbbr( void ) const
{
  return( regionAbbr );
}

inline
TZOffset_t
CARNpaNxxDeltaSybase::getTZOffset( void ) const
{
  return( TZOffset );
}

inline
Bool
CARNpaNxxDeltaSybase::isDSTHonored( void ) const
{
  return( honorsDST );
}



inline
const DateTime &
CARNpaNxxDeltaSybase::getEffDate( void ) const
{
  return( effDate );
}


#endif // ! def _CARNpaNxxDeltaSybase_hh_
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

