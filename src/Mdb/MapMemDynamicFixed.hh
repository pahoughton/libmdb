#ifndef _MapMemDynamicFixed_hh_
#define _MapMemDynamicFixed_hh_
//
// File:        MapMemDynamicFixed.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     11/18/94 10:09
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
#include <MapMemDynamic.hh>

#include <DumpInfo.hh>

#include <iostream>

#define MDF_VERSION 0x4d444604	// 'MDF4'

#if defined( MDB_DEBUG )
#define inline
#endif

class MapMemDynamicFixed : public MapMemDynamic
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
  MapMemDynamicFixed( const char *	fileName,
		      ios::open_mode	mode,
		      bool		create,
		      size_type		recSize,
		      size_type		allocNumRecs = 0,
		      MapMask		permMask = 02 );
  
  // use this constructor to create a new map file  
  MapMemDynamicFixed( const char * 	fileName,
		      size_type		recSize,
		      size_type		allocNumRecs = 0,
		      MapMask		permMask = 02 );

  // use this constructor to access an existing map file  
  MapMemDynamicFixed( const char * 	fileName,
		      ios::open_mode	mode = (ios::open_mode)(ios::in),
		      bool		overrideOwner = false );

  virtual ~MapMemDynamicFixed( void );
  
  Loc	    	    allocate( size_type size = 0 ); 
  void	    	    release( Loc loc ); 	
  
  bool		    valid( Loc loc ) const;
  
  size_type	    getRecSize( void ) const;
  size_type	    getAllocNumRecs( void ) const;
  
  bool 	    	    expand( void );

  virtual bool	    	good( void ) const;
  virtual const char *	error( void ) const;    
  virtual const char * 	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &	dumpInfo( ostream &	dest,
				  const char *  prefix = "    ",
				  bool		showVer = false ) const;

  inline
  DumpInfo< MapMemDynamicFixed >  dump( const char *	prefix = "    ",
					bool		showVer = true ) const;

  static const ClassVersion version;

  // debuging / testing methods
  ostream &	dumpFreeList( ostream & dest ) const;
  ostream &	dumpNodes( ostream & dest ) const;
  bool		allTested( void );
  
  // FreeNode should be protected but AIX can't deal with it.
  struct FreeNode
  {
    Loc	    nextFree;
    Loc	    prevFree;
  };

protected:

  inline Loc		    firstNode( void ) const;
  inline Loc		    lastNode( void ) const;
  
  inline const FreeNode &   freeList( void ) const;
  inline FreeNode &	    freeList( void );

  inline const FreeNode &   freeNode( Loc f ) const;
  inline FreeNode &	    freeNode( Loc f );
  
  
private:

  MapMemDynamicFixed( const MapMemDynamicFixed & copyFrom );
  MapMemDynamicFixed & operator=( const MapMemDynamicFixed & assignFrom );

  void	createMapMemDynamicFixed( size_type recSize,
				  size_type allocNumRecs );

  void	openMapMemDynamicFixed( void );
  
  struct MapDynamicFixedInfo : MapDynamicInfo
  {
    size_type	recSize;	    // record size
    size_type   allocNumRecs;   // records to allocate at a time
    FreeNode	freeList;	    // head to list of free records
  };

  inline MapDynamicFixedInfo *		mapInfo( void );
  inline const MapDynamicFixedInfo *	mapInfo( void ) const;
  
  static const char * ErrorStrings[];
  
  ErrorNum	errorNum;

};


#if !defined( inline )
#include <MapMemDynamicFixed.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MapMemDynamicFixed	class
//
//  Constructors:
//
//  	MapMemDynamicFixed( );
//
//  Destructors:
//
//  Public Interface:
//
//	virtual ostream &
//	write( ostream & dest ) const;
//	    write the data for this class in binary form to the ostream.
//
//	virtual istream &
//	read( istream & src );
//	    read the data in binary form from the istream. It is
//	    assumed it stream is correctly posistioned and the data
//	    was written to the istream with 'write( ostream & )'
//
//	virtual ostream &
//	toStream( ostream & dest ) const;
//	    output class as a string to dest (used by operator <<)
//
//	virtual istream &
//	fromStream( istream & src );
//	    Set this class be reading a string representation from
//	    src. Returns src.
//
//  	virtual Bool
//  	good( void ) const;
//  	    Return true if there are no detected errors associated
//  	    with this class, otherwise false.
//
//  	virtual const char *
//  	error( void ) const;
//  	    Return a string description of the state of the class.
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. MapMemDynamicFixed )
//
//  	virtual const char *
//  	getVersion( bool withPrjVer = true ) const;
//  	    Return the version string of this class.
//
//	virtual ostream &
//	dumpInfo( ostream & dest, const char * prefix, bool showVer );
//	    output detail info to dest. Includes instance variable
//	    values, state info & version info.
//
//	static const ClassVersion version
//	    Class and project version information. (see ClassVersion.hh)
//
//  Protected Interface:
//
//  Private Methods:
//
//  Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const MapMemDynamicFixed & src );
//
//	istream &
//	operator >> ( istream & src, MapMemDynamicFixed & dest );
//
// Example:
//
// See Also:
//
// Files:
//
// Documented Ver:
//
// Tested Ver:
//
// Revision Log:
//
// $Log$
// Revision 2.10  1997/07/13 11:25:40  houghton
// Cleanup.
// Added firstNode(), lastNode(), freeList() & freeNode().
//
// Revision 2.9  1997/06/19 12:02:31  houghton
// Class was renamed from MapMemFixedDynamic to MapMemDynamicFixed.
//
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
// Copyright:
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
#endif // ! def _MapMemDynamicFixed_hh_ 
