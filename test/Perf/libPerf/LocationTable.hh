#ifndef _LocationTable_hh_
#define _LocationTable_hh_
//
// File:        LocationTrable.hh
// Desc:        
//              
//
// Locationor:      Paul Houghton x2309 - (houghton@shoe.wiltel.com)
// Created:     01/26/95 10:38
//
// Revision History:
//
// 
// $Log$
// Revision 2.1  1997/09/21 19:22:40  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/19 12:52:05  houghton
// Initial Version (work in progess).
//
// Revision 1.5  1995/09/27  15:16:42  ichudov
// Committed changes after struggling with CVS.
//
// Revision 1.3.2.1  1995/08/21  02:30:16  ichudov
// Reconciling my and David's zero tol versions.
//
// Revision 1.1  1995/07/20  14:21:29  ichudov
// DAVLs; Fixed precision for tolls.
//
//

#include <DavlTreeOffset.hh>
#include <MultiMemOffsetMapFixed.hh>
#include "LocationRec.hh"

class LocationTable: public DavlTreeOffset<LocationRec::Key, LocationRec::Data>
{

public:

  LocationTable( const char *    keyFileName,
	        const char *    dataFileName,
	        size_t  	    numRecs );

  LocationTable( const char *    keyFileName,
	        const char *    dataFileName,
	        ios::open_mode  mode = (ios::open_mode)(ios::in) );

  ~LocationTable ( void );

  static LocationTable &    getGlobalNpaNxxTable( void );
  static LocationTable &    getGlobalDialPlanTable( void );

  void                  setThisGlobalNpaNxxTable( void );
  void                  setThisGlobalDialPlanTable( void );

  virtual const char *	getClassName( void ) const { return "LocationTable"; };
  
protected:

private:

  LocationTable ( const LocationTable & copyFrom );
  LocationTable & operator=( const LocationTable & assignFrom );

  static LocationTable * pGlobalNpaNxxTable;
  static LocationTable * pGlobalDialPlanTable;

  MultiMemOffsetMapFixed *  keyMap;
  MultiMemOffsetMapFixed *  dataMap;
  
};


//
// Inline methods
//


inline
LocationTable &
LocationTable::getGlobalNpaNxxTable( void )
{
  if( pGlobalNpaNxxTable == 0 )
    {
      cerr << "No global NpaNxx table defined!!!" << endl;
    }

  return * pGlobalNpaNxxTable;
}

inline
LocationTable &
LocationTable::getGlobalDialPlanTable( void )
{
  if( pGlobalDialPlanTable == 0 )
    {
      cerr << "No global DialPlan table defined!!!" << endl;
    }

  return * pGlobalDialPlanTable;
}

inline
void
LocationTable::setThisGlobalNpaNxxTable( void )
{
  pGlobalNpaNxxTable = this;
}

inline
void
LocationTable::setThisGlobalDialPlanTable( void )
{
  pGlobalDialPlanTable = this;
}



inline
LocationTable::LocationTable(
  const char *	keyFileName,
  const char *	dataFileName,
  size_t    	numRecs
  )
  : DavlTreeOffset<LocationRec::Key, LocationRec::Data>(
    LocationRec::compareKey,
    LocationRec::copyKey,
    LocationRec::copyData,
    keyMap = new MultiMemOffsetMapFixed(
      keyFileName,
      DavlTreeOffset<LocationRec::Key, LocationRec::Data>::getKeySize(),
      numRecs ),
    dataMap = new MultiMemOffsetMapFixed(
      dataFileName,
      DavlTreeOffset<LocationRec::Key, LocationRec::Data>::getDataSize(),
      numRecs )
    )
{
  if( keyMap ) keyMap->getMap().setKey(getTreeOffset(),0 );
}
    
    
inline
LocationTable::LocationTable(
  const char *	keyFileName,
  const char *	dataFileName,
  ios::open_mode    mode
  )
  : DavlTreeOffset<LocationRec::Key, LocationRec::Data>(
    (keyMap = new MultiMemOffsetMapFixed( keyFileName, mode )) ?
      (keyMap->getMap().getKey(0)) : 0,
    LocationRec::compareKey,
    LocationRec::copyKey,
    LocationRec::copyData,
    keyMap,
    dataMap = new MultiMemOffsetMapFixed( dataFileName, mode )
    )
{
}
    

inline
LocationTable::~LocationTable( void )
{

  if( pGlobalNpaNxxTable == this )
    {
      pGlobalNpaNxxTable = 0;
    }

  if( pGlobalDialPlanTable == this )
    {
      pGlobalDialPlanTable = 0;
    }

  delete keyMap;
  delete dataMap;
}



#endif // ! def _LocationTable_hh_ 
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
