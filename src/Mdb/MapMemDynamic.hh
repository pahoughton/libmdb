#ifndef _MapMemDynamic_hh_
#define _MapMemDynamic_hh_
//
// File:        MapMemDynamic.hh
// Project:	Mdb
// Desc:        
//
//  Base class for MapMemDynamicDynamic and MapMemDynamicFixed
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/20/97 05:31
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
#include <MapMem.hh>

#include <DumpInfo.hh>

#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif

#define MMD_NUM_KEYS	16

class MapMemDynamic : public MapMem
{

public:

  typedef MDB_TYPE_ADDR	    Addr;
  typedef MDB_TYPE_LOC	    Loc;
  
  // use this to create a new map or open an existing one
  MapMemDynamic( const char *	    fileName,
		 MapVersion	    version,
		 ios::open_mode	    mode,
		 bool		    create,
		 size_type	    size,
		 MapMask	    permMask = 02 );
		 
  // use this to create a new map
  MapMemDynamic( const char * 	    fileName,
		 MapVersion	    version,
		 size_type	    size,
		 MapMask	    permMask = 02 );

  // use this to open an existing map
  MapMemDynamic( const char * 	    fileName,
		 MapVersion	    version,
		 ios::open_mode	    mode = ios::in,
		 bool		    overrideOwner = false );

  virtual ~MapMemDynamic( void );

  virtual Loc		allocate( size_type size ) = 0;
  virtual void		release( Loc loc ) = 0;

  inline Addr		address( Loc loc );
  inline const Addr	address( Loc loc ) const;
  inline Loc		location( const Addr addr ) const;

  inline long		setKey( unsigned short key, long value );
  inline long		getKey( unsigned short key ) const;

  inline unsigned long	getAllocCount( void ) const;
  inline unsigned long	getFreeCount( void ) const;
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline
  DumpInfo< MapMemDynamic >	dump( const char *  prefix = "    ",
				      bool	    showVer = true ) const;
  
  static const ClassVersion version;

  static const unsigned short numKeys;
  
  struct MapDynamicInfo : public MapInfo
  {
    unsigned long   allocCount;	    // allocated chunks
    unsigned long   freeCount;	    // available chunks
    long    	    keys[MMD_NUM_KEYS]; // general purpose values
  };

protected:

private:

  MapMemDynamic( const MapMemDynamic & from );
  MapMemDynamic & operator =( const MapMemDynamic & from );

  void createMapMemDynamic( void );
  void openMapMemDynamic( void );
  
  inline MapDynamicInfo *	    mapInfo( void );
  inline const MapDynamicInfo *	    mapInfo( void ) const;

  
};

#if !defined( inline )
#include <MapMemDynamic.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MapMemDynamic	class
//
//  Constructors:
//
//  	MapMemDynamic( const char *	fileName,
//		       MapVersion	version,      
//		       ios::open_mode	mode,
//		       bool		create,
//		       size_type	size
//		       MapMask		permMask = 02 );
//	    Construct a object that either opens an existing map or
//	    creates a new one.
//		'fileName' is the full name of the file.
//		'version' the map structure version.
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
//  	MapMemDynamic( const char *  fileName,
//		       MapVersion	version,      
//		       ios::open_mode	mode,
//		       bool		create,
//		       size_type	size
//		       MapMask		permMask = 02 );
//	    Construct a object that creates a new map.
//	    creates a new one.
//		'fileName' is the full name of the file.
//		'version' the map structure version.
//		'size' is the minumum initial size of the map.
//		'permMask' is the 'umask(2)' to use when creating the file.
//		    a value of '02' will create a file with the mode
//		    set to '-rw-rw-r--'.
//
//  	MapMemDynamic( const char *	fileName,
//		       MapVersion	version,      
//		       ios::open_mode	mode,
//		       bool		overrideOwner = false );
//	    Construct a object that opens an existing map.
//		'fileName' is the full name of the file.
//		'version' the map structure version.
//		'mode' is the mode to open the file, ios::in == read only
//		    (ios::in | ios::out) == read/write. 
//		'overrideOwner' if this is true. The map's current owner
//		    value will be overriden. See MapMem(3) for more info.
//
//  Public Interface:
//
//	virtual
//	Loc
//	allocate( size_type size ) = 0
//	    allocate a chunk of at lease 'size' bytes and return it's 'loc'.
//	    If an error occured, 0 will be returned.
//
//	virtual
//	void
//	release( Loc loc ) = 0
//	    release a previously allocated chunk.
//
//	inline
//	Addr
//	address( Loc loc )
//	    return a useable address for the 'loc'. This address is 
//	    valid only until the next call to allocate or release.
//
//	inline
//	const Addr
//	address( Loc loc ) const
//	    return a useable address for the 'loc'. This address is 
//	    valid only until the next call to allocate or release.
//
//	inline
//	Loc
//	location( const Addr addr ) const
//	    return the 'loc' for the 'addr'. The 'addr' must have been
//	    obtained by a call to address() for the same map.
//
//	inline
//	long
//	setKey( unsigned short key, long value )
//	    The map has room for 'maxKey' (16) key values in the header.
//	    This method is use to store a specific value in one of them.
//	    returns the previous value for the specific key.
//
//	inline
//	long
//	getKey( unsigned short key ) const
//	    return the value stored at the specific key.
//
//	inline
//	unsigned long
//	getChunkCount( void ) const
//	    return the total number of chunks allocated.
//
//	inline
//	unsigned long
//	getFreeCount( void ) const
//	    return the total number of free chunks.
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
//  	    Return the name of this class (i.e. MapMemDynamic )
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
//	inline
//	DumpInfo< MapMemDynamic >
//	dump( const char * previx, bool showVer = true )
//	    return an object that can be passed to operator << ( ostream & )
//	    that will generate the same output as 'dumpInfo'.
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
// Example:
//
// See Also:
//
//	MapMemDynamicDynamic( 3 )
//	MapMemDynamicFixed( 3 )
//	MapMem( 3 )
//	MapFile( 3 )
//	ChunkMgrLoc( 3 )
//	ChunkMgr( 3 )
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
// Revision 2.3  1997/07/13 11:19:18  houghton
// Cleanup
// Removed owner (now in MapMem).
// Added documentation.
//
// Revision 2.2  1997/06/05 13:43:28  houghton
// Changed for AIX: had to make MapDynamicInfo a public member.
//
// Revision 2.1  1997/06/05 11:29:12  houghton
// Initial Version.
//
//
#endif // ! def _MapMemDynamic_hh_ 

