#ifndef _MapMem_hh_
#define _MapMem_hh_
//
// File:        MapMem.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Notes:
//
//  This class was originally part of libClue1
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     11/18/94 10:06
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
#include <MapFile.hh>
#include <DumpInfo.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif

class MapMem : public MapFile
{

public:

  typedef unsigned long	    MapVersion;
  
  enum MapType
  {
    MM_FIXED,	    // fixed 'map to' address
    MM_DYNAMIC,	    // dynamic 'map to' address
    MM_UNDEFINED
  };

  // use this to create an new map or open and existing one.
  MapMem( const char *	    fileName,
	  MapType	    type,
	  MapVersion	    version,
	  ios::open_mode    mode,
	  bool		    create,
	  MapAddr	    baseAddr,
	  size_type	    size,
	  MapMask	    permMask = 02 );
	  
	  
  // use this to create a new map
  MapMem( const char * 	    fileName,
	  MapAddr    	    baseAddr,
	  MapType   	    type,
	  MapVersion	    version,
	  size_type	    size,
	  MapMask	    permMask = 02 );

  // use this to open an existing map
  MapMem( const char * 	    fileName,
	  MapType   	    type,
	  MapVersion	    version,
	  ios::open_mode    mode = ios::in,
	  bool		    overrideOwner = false );

  virtual ~MapMem( void );
  
  MapVersion	    getMapVersion( void ) const;
  void *    	    getMapToAddr( void ) const;
  MapType   	    getType( void ) const;
  const char *	    getTypeName( void ) const;
  size_type	    getMapSize( void ) const;
  long		    getOwner( void ) const;
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;

  inline
  DumpInfo< MapMem >	    dump( const char *	prefix = "    ",
				  bool		showVer = true ) const;
  
  struct MapInfo
  {
    MapType 	    type;
    MapVersion	    version;
    unsigned long   base;
    unsigned long   size;
    long	    owner;	    // pid of owner (writer)
  };

protected:

private:

  enum ErrorNum
  {
    E_OK,
    E_MAPFILE,
    E_FILESIZE,
    E_VERSION,
    E_BADTYPE,
    E_OWNER,
    E_UNDEFINED
  };

  MapMem( const MapMem & copyFrom );
  MapMem & operator=( const MapMem & assignFrom );

  void	createMapMem( MapType type, MapVersion version, MapAddr baseAddr );
  void	openMapMem( const char * fileName,
		    MapType	    type,
		    MapVersion	    version,
		    ios::open_mode  mode,
		    bool	    overrideOwner );
  
  inline const MapInfo * 	mapInfo( void ) const;
  inline MapInfo *		mapInfo( void );
  
  static const char * ErrorStrings[];
  static const char * TypeStrings[];
  
  ErrorNum	errorNum;
  int	    	osErrno;
  
};


#if !defined( inline )
#include <MapMem.ii>
#else
#undef inline
#endif



//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MapMem	class
//
//  Constructors:
//
//  	MapMem( );
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
//  	    Return the name of this class (i.e. MapMem )
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
//  	operator <<( ostream & dest, const MapMem & src );
//
//	istream &
//	operator >> ( istream & src, MapMem & dest );
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
// Revision 2.7  1997/07/13 11:18:01  houghton
// Cleanup
// Added owner.
//
// Revision 2.6  1997/06/05 13:42:56  houghton
// Changed for AIX: had to make MapInfo a public member.
//
// Revision 2.5  1997/06/05 11:22:51  houghton
// Cleanup.
// Change to be part of libMdb (vs Clue1).
// Added type MapVersion.
// Added constructor that can create or open existing.
// Changed to use new MapFile types.
// Added createMapMem and openMapMemMehtods.
// Moved inlines to MapMem.ii
//
// Revision 2.4  1997/04/04 20:49:33  houghton
// Cleanup.
//
// Revision 2.3  1997/03/07 11:49:01  houghton
// Add dumpInfo.
//
// Revision 2.2  1997/03/03 14:32:34  houghton
// Added virtual destructor.
//
// Revision 2.1  1995/11/10 12:42:29  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:34  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:49  houghton
// New Style Avl an memory management. Many New Classes
//
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
#endif // ! def _MapMem_hh_ 

