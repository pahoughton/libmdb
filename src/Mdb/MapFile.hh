#ifndef _MapFile_hh_
#define _MapFile_hh_
//
// File:        MapFile.hh
// Project:	Mdb
// Desc:        
//
//  MapFile uses the operating system's mapped memory functions
//  (mmap(2) & munmap(2)) to map a file to memory. A file can be to a
//  specific memory address or the operating system can select the address
//  to map the file to (See libMdb/docs/design/MapFile.txt for more info).
//
// Quick Start: - short example of class usage
//
// Notes:
//
//  This class was originally part of libClue1
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     11/11/94 06:46
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

#include <FileStat.hh>
#include <DumpInfo.hh>

#include <sys/types.h>

#if defined( MDB_DEBUG )
#define inline
#endif

class MapFile
{

public:

  typedef MDB_TYPE_SIZE	    size_type;
  
  typedef MDB_TYPE_ADDR	    MapAddr;
  typedef unsigned short    MapMask;	// map permision mask (i.e. umask)
  
  // use this constructor to create a new file or open an existing one
  MapFile( const char *	    fileName,
	   MapAddr	    baseAddr,
	   ios::open_mode   mode,
	   bool		    create,
	   size_type	    size,
	   MapMask	    permMask = 02 );

  // use this constructor to create a new file
  MapFile( const char *     fileName,
	   size_type   	    size,
 	   MapAddr   	    baseAddr = 0,
	   MapMask	    permMask = 02 );

  // use this constructor to open an existing file
  MapFile( const char *     fileName,
	   MapAddr   	    baseAddr = 0,
	   ios::open_mode   mode = ios::in );

  MapFile( void );
  
  virtual ~MapFile( void );

  size_type    	    map( const char * 	fileName,
			 MapAddr      	baseAddr = 0,
			 ios::open_mode	mode = ios::in );

  void		    unmap( void );
  
  size_type		setSize( size_type amount, MapAddr baseAddr );
  inline size_type	grow( size_type amount, MapAddr baseAddr );
  inline size_type      shrink( size_type amount, MapAddr baseAddr );

  const char *		getFileName( void ) const;
  const char *		getAccess( void ) const;
  inline ios::open_mode	getMode( void ) const;
  inline size_type	getSize( void ) const;
  inline MapAddr	getBase( void );
  inline const MapAddr  getBase( void ) const;
  inline MapAddr	getEnd( void );
  inline const MapAddr	getEnd( void ) const;
  static size_type	getPageSize( void  );

  // reference counting
  inline long	    addRef( void );
  inline long	    getRefCount( void ) const;
  inline bool	    delRef( void );

  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual int		getErrno( void ) const;
  virtual const char * 	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;  
  virtual ostream &	dumpInfo( ostream &	dest = cerr,
				  const char *	prefix = "    ",
				  bool		showVer = false ) const;
  
  static const ClassVersion version;

  inline
  DumpInfo< MapFile >	dump( const char *  prefix = "    ",
			      bool	    showVer = true ) const;
  
protected:

private:

  MapFile( const MapFile & copyFrom );
  MapFile & operator=( const MapFile & assignFrom );

  void	createMap( const char * fileName,
		   MapAddr	baseAddr,
		   size_type	size,
		   MapMask	mask );
  
  FileStat  	    fileStat;

  int	    	    mapFd;
  ios::open_mode    mapMode;
  size_type    	    mapSize;
  MapAddr    	    mapBase;

  long		    refCount;
  
  size_type    	    pageSize;

  int	    	    osErrno;

};



#if !defined( inline )
#include <MapFile.ii>
#else
#undef inline

#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MapFile	class
//
//  Constructors:
//
//  	MapFile( const char *	    fileName,
//		 MapAddr	    baseAddr,
//		 ios::open_mode	    mode,
//		 bool		    create,
//		 size_type	    size,
//		 MapMask	    permMask = 02 )
//	    Create a new map or open an existing one.
//		'fileName' is the full name of the file to use.
//		'baseAddr' is the base address to map the file to. 0
//		    should be use to allow the OS to select the best
//		    address.
//		'mode' is the mode to open the file, ios::in == read only
//		    (ios::in | ios::out) == read/write (note if create == true
//		    the mode argument is ignored and the file is opened
//		    with (ios::in | ios::out).
//		'create' true == create a new map; false == access an
//		    existing map.
//		'size' is the minumum initial size of the map.
//		'permMask' is the 'umask(2)' to use when creating the file.
//		    a value of '02' will create a file with the mode
//		    set to '-rw-rw-r--'.
//
//  	MapFile( const char *	    fileName,
//		 size_type	    size,
//		 MapAddr	    baseAddr = 0,
//		 MapMask	    permMask = 02 )
//	    Create a new map.
//		'fileName' is the full name of the file to use.
//		'baseAddr' is the base address to map the file to. 0
//		    should be use to allow the OS to select the best
//		    address.
//		'size' is the minumum initial size of the map.
//		'permMask' is the 'umask(2)' to use when creating the file.
//		    a value of '02' will create a file with the mode
//		    set to '-rw-rw-r--'.
//
//
//  	MapFile( const char *	    fileName,
//		 MapAddr	    baseAddr = 0,
//		 ios::open_mode	    mode = ios::in )
//	    Open an existing map.
//		'fileName' is the full name of the file to use.
//		'baseAddr' is the base address to map the file to. 0
//		    should be use to allow the OS to select the best
//		    address.
//		'mode' is the mode to open the file, ios::in == read only
//		    (ios::in | ios::out) == read/write (note if create == true
//		    the mode argument is ignored and the file is opened
//		    with (ios::in | ios::out).
//
//	MapFile( void )
//	    Instanciate a MapFile. Use 'map()' to open/create a map.
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
//  	    Return the name of this class (i.e. MapFile )
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
//  	operator <<( ostream & dest, const MapFile & src );
//
//	istream &
//	operator >> ( istream & src, MapFile & dest );
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
// Revision 2.9  1997/07/13 11:15:10  houghton
// Cleanup
// Added documentation.
//
// Revision 2.8  1997/06/09 11:57:12  houghton
// Cleanup.
//
// Revision 2.7  1997/06/05 11:17:35  houghton
// Cleanup.
// Change to be part of libMdb (vs Clue1).
// Added types size_type, MapAddr, MapMask and change to use them.
// Added constructor that can create or open existing.
// Moved inlines to new MapFile.ii file.
//
// Revision 2.6  1997/04/21 12:12:16  houghton
// Added getErrno.
//
// Revision 2.5  1997/04/04 20:49:02  houghton
// Cleanup.
//
// Revision 2.4  1997/03/07 11:48:41  houghton
// Add dumpInfo.
//
// Revision 2.3  1997/03/03 14:32:22  houghton
// Moved construtors to .C from .hh (no longer inline).
// Added virtual destructor.
//
// Revision 2.2  1996/02/29 19:09:49  houghton
// *** empty log message ***
//
// Revision 2.1  1995/11/10 12:42:28  houghton
// Change to Version 2
//
// Revision 1.4  1995/11/05  16:32:33  houghton
// Revised
//
// Revision 1.2  1995/03/02  16:35:35  houghton
// Linux ports & new Classes
//
// Revision 1.1  1995/02/13  16:08:48  houghton
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
#endif // ! def _MapFile_hh_ 
