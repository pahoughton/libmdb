#ifndef _MapFile_hh_
#define _MapFile_hh_
//
// File:        MapFile.hh
// Project:	Mdb
// Desc:        
//
//  MapFile uses the operating system's mapped memory functions
//  (mmap(2) & munmap(2)) to map a file to memory. A file can be
//  mapped to a specific memory address or the operating system can
//  select the address to use (See libMdb/docs/design/MapFile.txt
//  for more info).
//
// Notes:
//
//  When using dynamicly addressed maps (baseAddr == 0). The values
//  returned by 'getBase()' and 'getEnd()' can change any time
//  'setSize()', 'grow()' or 'shrink()' is called. So, it is best to
//  never have a variable that contains a pointer into the map. The
//  best thing to do is always call getBase() to access the data.
//
//  This class was originally part of libStlUtils1
//
// Author:      Paul A. Houghton - (paul.houghton@mci.com)
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

  // helpers
  typedef MDB_S16_T	s16;
  typedef MDB_U16_T	u16;
  typedef MDB_S32_T	s32;
  typedef MDB_U32_T	u32;
#if defined( MDB_HAVE_LONG_LONG )
  typedef MDB_S64_T	s64;
  typedef MDB_U64_T	u64;
#endif
  
  typedef size_t	    size_type;
  
  typedef MDB_TYPE_ADDR	    MapAddr;
  typedef unsigned short    MapMask;	// map permision mask (i.e. umask)

  static const size_type    npos;
  
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

  size_type	    createMap( const char * fileName,
			       MapAddr	    baseAddr,
			       size_type    size,
			       MapMask	    permMask = 02 );
  
  size_type    	    map( const char * 	fileName,
			 MapAddr      	baseAddr = 0,
			 ios::open_mode	mode = ios::in );

  void		    unmap( void );

  bool		    sync( size_type	beg = 0,
			  size_type	len = npos,
			  bool		async = false ); 
			  
  size_type		setSize( size_type size, MapAddr baseAddr );
  inline size_type	grow( size_type amount, MapAddr baseAddr );
  inline size_type      shrink( size_type amount, MapAddr baseAddr );

  const FileStat &	getFileStat( void ) const;
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

  static size_type  getTotalMapped( void );
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual int		getErrno( void ) const;
  virtual const char * 	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;  
  virtual ostream &	dumpInfo( ostream &	dest = cerr,
				  const char *	prefix = "    ",
				  bool		showVer = false ) const;
  
  inline
  DumpInfo< MapFile >	dump( const char *  prefix = "    ",
			      bool	    showVer = true ) const;
  
  static const ClassVersion version;

protected:

  static size_type	totalMapped;
  
private:

  MapFile( const MapFile & copyFrom );
  MapFile & operator=( const MapFile & assignFrom );

  FileStat  	    fileStat;

  int	    	    mapFd;
  ios::open_mode    mapMode;
  size_type    	    mapSize;
  MapAddr    	    mapBase;

  long		    refCount;
  
  int    	    pageSize;

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
//	MapFile::size_type
//	MapFile::MapAddr
//	MapFile::MapMask
//
//  Constructors:
//
//  	MapFile( const char *	    fileName,
//		 MapAddr	    baseAddr,
//		 ios::open_mode	    mode,
//		 bool		    create,
//		 size_type	    size,
//		 MapMask	    permMask = 02 )
//	    Create a new map or open an existing one. If create is true
//	    and 'fileName' already exists, it will be removed before the
//	    map is created.
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
//	    Create a new map. Note, if 'fileName' already exist, it will be
//	    removed before the map is created.
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
//	    Instanciate a MapFile. Use 'createMap()' or 'map()' to create
//	    or open a map.
//
//  Public Interface:
//
//	size_type
//	createMap( const char *	fileName,
//		   MapAddr	baseAddr,
//		   size_type    size,
//		   MapMask	permMask = 02 );
//	    This method will 'unmap()' the any existing map and create a
//	    new map file.  Note, if 'fileName' already exist, it will be
//	    removed before the map is created.
//		'fileName' is the full name of the file to use.
//		'baseAddr' is the base address to map the file to. 0
//		    should be use to allow the OS to select the best
//		    address.
//		'size' is the minumum initial size of the map.
//		'permMask' is the 'umask(2)' to use when creating the file.
//		    a value of '02' will create a file with the mode
//		    set to '-rw-rw-r--'.
//	    The size of the new map is returned or 0 if an error occured.
//
//	size_type
//	map( const char *   fileName,
//	     MapAddr	    baseAddr,
//	     ios::open_mode mode = ios::in )
//	    Open an existing map.
//		'fileName' is the full name of the file to use.
//		'baseAddr' is the base address to map the file to. 0
//		    should be use to allow the OS to select the best
//		    address.
//		'mode' is the mode to open the file, ios::in == read only
//		    (ios::in | ios::out) == read/write (note if create == true
//		    the mode argument is ignored and the file is opened
//		    with (ios::in | ios::out).
//	    The size of the map is returned or 0 if an error occured.
//
//	void
//	unmap( void )
//	    Unmap the current map file.
//
//	size_type
//	setSize( size_type size, MapAddr baseAddr )
//	    Change the size of the map file to 'size'. 'baseAddr' is
//	    the address to remap the file to. Since the map
//	    file MUST be page aligned, the new size is returned. It will
//	    be either 'size' or a greater value (up to the next page).
//	    If an error occures, 0 is returned.
//
//	inline
//	size_type
//	grow( size_type amount, MapAddr baseAddr )
//	    Increase the map's size by at least 'amount' bytes. 'baseAddr'
//	    is the address to remap the file to. Since the map
//	    file MUST be page aligned, the new size is returned. 
//	    If an error occures, 0 is returned.
//
//	inline
//	size_type
//	shrink( size_type amount, MapAddr baseAddr )
//	    Reduce the map's size by 'amount' bytes. 'baseAddr'
//	    is the address to remap the file to. Since the map
//	    file MUST be page aligned, the new size is returned. 
//	    If an error occures, 0 is returned.
//
//	const FileStat &
//	getFileStat( void ) const
//	    Return the 'FileStat(3)' for the map file. Note, this is only
//	    updated by calls to 'map()' and 'createMap()' and the
//	    constructors, so it may accurately represent the values
//	    for the map file.
//
//	const char *
//	getFileName( void ) const
//	    Return the file name of the map file.
//
//	const char *
//	getAccess( void ) const
//	    Return the maps access capablilties. This will be one of:
//	    "RW", "R", or "W", depending on the mode used to open the
//	    map file.
//
//	inline
//	ios::open_mode
//	getMode( void ) const
//	    Return the mode used to open the map. Note, newly created
//	    maps will have a mode of (ios::in|ios::out).
//
//	inline
//	size_type
//	getSize( void ) const
//	    Return the map file's size.
//
//	inline
//	MapAddr
//	getBase( void )
//	    Return the map's base address.
//
//	inline
//	const MapAddr
//	getBase( void ) const
//	    Return the map's base address as a const pointer.
//
//	inline
//	MapAddr
//	getEnd( void )
//	    Return the map's end address. This is 'getBase() + getSize()'.
//	    It is the first INVALID address beyond the map's range.
//
//	inline
//	const MapAddr
//	getEnd( void )
//	    Return the map's end address as a const pointer. This is
//	    'getBase() + getSize()'. It is the first INVALID address
//	    beyond the map's range.
//
//	static
//	size_type
//	getPageSize( void )
//	    Return the number of bytes in a page. This come directly
//	    the operating system being used.
//
//	inline
//	long
//	addRef( void )
//	    This can be used for referance counting. The referance counter
//	    is initialzed to 0 by the constructors.
//
//	inline
//	long
//	getRefCount( void ) const
//	    Return the current referance count value.
//
//	inline
//	bool
//	delRef( void )
//	    This can be used for referance counting. It will return
//	    true when the referance count reaches 0.
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
// Example:
//
// See Also:
//
// Files:
//
// Documented Ver: 2.10
//
// Tested Ver: 2.10
//
// Revision Log:
//
// $Log$
// Revision 4.3  2003/08/09 12:43:23  houghton
// Changed ver strings.
//
// Revision 4.2  2003/07/19 09:11:13  houghton
// Port to 64 bit.
//
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.14  1999/05/09 17:32:05  houghton
// Added long long support.
//
// Revision 2.13  1999/03/02 12:58:15  houghton
// Added totalMapped static variable.
// Added getTotalMapped().
//
// Revision 2.12  1997/10/01 13:59:54  houghton
// Changed to use portable multi platform data types.
// Added sync().
//
// Revision 2.11  1997/09/17 16:56:01  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.10  1997/07/25 13:47:42  houghton
// Moved createMap to public.
// Added documentation.
//
// Revision 2.9  1997/07/13 11:15:10  houghton
// Cleanup
// Added documentation.
//
// Revision 2.8  1997/06/09 11:57:12  houghton
// Cleanup.
//
// Revision 2.7  1997/06/05 11:17:35  houghton
// Cleanup.
// Change to be part of libMdb (vs StlUtils1).
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
