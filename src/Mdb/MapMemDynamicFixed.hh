#ifndef _MapMemFixedDynamic_hh_
#define _MapMemFixedDynamic_hh_
//
// File:        MapMemFixedDynamic.hh
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/18/94 10:09
//
// Revision History: (See Revision Log at end of file)
//
// 

#include <MdbConfig.hh>
#include <MapMemDynamic.hh>

#include <DumpInfo.hh>

#include <iostream>

#define MDF_VERSION 0x4d444604	// 'MDF4'

#if defined( MDB_DEBUG )
#define inline
#endif

class MapMemFixedDynamic : public MapMemDynamic
{

public:

  enum ErrorNum
  {
    E_OK,
    E_MAPMEM,
    E_BADSIZE,
    E_UNDEFINED
  };

  
  // use this constructor to either create a new map or access an existing
  MapMemFixedDynamic( const char *	fileName,
		      ios::open_mode	mode,
		      bool		create,
		      size_type		recSize = 0,
		      size_type		numRecs = 0,
		      MapMask		permMask = 0777 );
  
  // use this constructor to create a new map file  
  MapMemFixedDynamic( const char * 	fileName,
		      size_type		recSize,
		      size_type		numRecs = 0,
		      MapMask		permMask = 0777 );

  // use this constructor to access an existing map file  
  MapMemFixedDynamic( const char * 	fileName,
		      ios::open_mode	mode = (ios::open_mode)(ios::in),
		      bool		overrideOwner = false );

  virtual ~MapMemFixedDynamic( void );
  
  Loc	    	    allocate( size_type size = 0 ); 
  void	    	    release( Loc loc ); 	
  
  bool		    valid( Loc loc ) const;
  
  size_type	    getRecSize( void ) const;
  size_type	    getChunkSize( void ) const;
  
  void 	    	    expand( void );

  virtual bool	    	good( void ) const;
  virtual const char *	error( void ) const;    
  virtual const char * 	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &	dumpInfo( ostream &	dest,
				  const char *  prefix = "    ",
				  bool		showVer = false ) const;

  static const ClassVersion version;

  inline
  DumpInfo< MapMemFixedDynamic >  dump( const char *	prefix = "    ",
					bool		showVer = true ) const;
  struct FreeList
  {
    unsigned long next;
    unsigned long prev;
  };

protected:

private:

  MapMemFixedDynamic( const MapMemFixedDynamic & copyFrom );
  MapMemFixedDynamic & operator=( const MapMemFixedDynamic & assignFrom );

  void	createMapMemFixedDynamic( size_type recSize,
				  size_type numRecs );

  void	openMapMemFixedDynamic( void );
  
  struct MapFixedDynamicInfo : MapDynamicInfo
  {
    unsigned long   recSize;	// record size
    unsigned long   chunkSize;	// records to allocate at a time
    struct FreeList freeList;	// head to list of free records
  };

  inline MapFixedDynamicInfo *		mapInfo( void );
  inline const MapFixedDynamicInfo *	mapInfo( void ) const;
  
  static const char * ErrorStrings[];
  
  ErrorNum	errorNum;

};


#if !defined( inline )
#include <MapMemFixedDynamic.ii>
#else
#undef inline


#endif

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
// $Log$
// Revision 2.8  1997/06/18 14:15:26  houghton
// Rework to use MapMemDynamic as base Class.
// Rework to be part of libMdb.
//
// Revision 2.7  1997/04/25 22:25:18  houghton
// Added valid( off_t ) - returns true if the off_t is a valid usable
//     offset for this map.
//
// Revision 2.6  1997/04/04 20:50:25  houghton
// Cleanup.
// Added map owner to prevent to progs from opening the map in write
//     mode at the same time.
//
// Revision 2.5  1997/03/08 10:28:40  houghton
// Cleanup.
// Added dump.
// Added ClassVersion.
//
// Revision 2.4  1997/03/07 11:49:33  houghton
// Add dumpInfo.
//
// Revision 2.3  1997/03/03 14:32:47  houghton
// Added virtual destructor.
//
// Revision 2.2  1996/11/06 18:07:27  houghton
// Renamed Clue.hh to ClueUtils.hh.
//
// Revision 2.1  1995/11/10 12:42:31  houghton
// Change to Version 2
//
// Revision 1.5  1995/11/05  16:32:35  houghton
// Revised
//
// Revision 1.2  1995/03/02  16:35:36  houghton
// Linux ports & new Classes
//
// Revision 1.1  1995/02/13  16:08:50  houghton
// New Style Avl an memory management. Many New Classes
//
//
#endif // ! def _MapMemFixedDynamic_hh_ 
