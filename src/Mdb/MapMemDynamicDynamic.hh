#ifndef _MapMemDynamicDynamic_hh_
#define _MapMemDynamicDynamic_hh_
//
// File:        MapMemDynamicDynamic.hh
// Project:	Mdb
// Desc:        
//
//  This class provides chunk management for a Dynamic mapped memory file.
//  it allows you to allocate and release chunks from 1 to 'maxChunk' (~2 gig)
//  in size. The operating system's mapped memory functions are used to
//  map the chunks to a disk file.
//
//  See libMdb/docs/design/MapMemDynamcDynamc.txt for more info.
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     03/07/97 05:41
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

#define MDD_VERSION 0x4d444405	// 'MDD5'

#if defined( MDB_DEBUG )
#define inline
#endif


class MapMemDynamicDynamic : public MapMemDynamic
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
  MapMemDynamicDynamic( const char *	fileName,
			ios::open_mode	mode,
			bool		create,
			size_type	minChunkSize,
			size_type	allocSize,
			MapMask		permMask = 02 );
  
  // use this constructor to create a new map file  
  MapMemDynamicDynamic( const char * 	fileName,
			size_type	minChunkSize,
			size_type	allocSize = 0,
			unsigned short	permMask = 02 );

  // use this constructor to access an existing map file  
  MapMemDynamicDynamic( const char * 	fileName,
			ios::open_mode	mode = (ios::open_mode)(ios::in),
			bool		overrideOwner = false );

  virtual ~MapMemDynamicDynamic( void );

  virtual Loc	    allocate( size_type size );	// returns offset not addr!
  virtual void 	    release( Loc  loc ); 	// needs offset not addr!
  
  unsigned long	    getChunkSize( void ) const;
  unsigned long	    getFreeSize( void ) const;
  
  virtual bool	    expand( size_type minAmount );

  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline
  DumpInfo< MapMemDynamicDynamic >  dump( const char *	prefix = "    ",
					  bool		showVer = true ) const;

  static const ClassVersion version;

  // debuging / testing methods
  ostream &	dumpFreeList( ostream & dest ) const;
  ostream &	dumpNodes( ostream & dest ) const;

  // do NOT use these
  // Node & FreeNode should be protected but AIX & Sun can't deal with it.
  
  struct Node
  {
    long	next;
    long	prev;
  };

  struct FreeNode : public Node
  {
    long	nextFree;
    long	prevFree;
  };
  

protected:

  inline FreeNode &	    freeNode( Loc f );
  inline const FreeNode &   freeNode( Loc f ) const;
  inline size_type	    freeNodeSize( Loc f ) const;
  
  inline FreeNode &	    freeList( void );
  inline const FreeNode &   freeList( void ) const;

  inline void		    setNextPrev( Loc n, Loc p );
  inline void		    setFreePrevNext( Loc f, Loc n );
  inline void		    setFreeNextPrev( Loc f, Loc p );

  inline Loc		    nodeLoc( Loc f ) const;
  inline Loc		    freeLoc( Loc n ) const;

  inline Loc		    allocFreeNode( Loc f, size_type chunkSize );
  
private:

  MapMemDynamicDynamic( const MapMemDynamicDynamic & from );
  MapMemDynamicDynamic & operator =( const MapMemDynamicDynamic & from );

  void	    createMapMemDynamicDynamic( size_type   minChunkSize,
					size_type   allocSize );

  void	    openMapMemDynamicDynamic( void );

  // do NOT use this it is only public because Sun can't handle it
  // being private.
public:  
  struct MapDynamicDynamicInfo : public MapDynamicInfo
  {
    unsigned long   minChunkSize;   // minimum chunk size
    unsigned long   allocSize;	    // bytes to allocate at a time
    unsigned long   chunkSize;	    // total allocated size
    unsigned long   freeSize;	    // total available size;
    struct FreeNode freeList;	    // head to list of free chunks
  };
private:
  
  inline MapDynamicDynamicInfo *	mapInfo( void );
  inline const MapDynamicDynamicInfo *  mapInfo( void ) const;
  
  static const char *	ErrorStrings[];
  
  ErrorNum	    errorNum;

};

#if !defined( inline )
#include <MapMemDynamicDynamic.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MapMemDynamicDynamic	class
//	    sub class of 'MapMemDynamic( 3 )'
//
//  Constructors:
//
//	    Note: use 'good()' to verify there where no errors
//		  after constructing an instance.
//
//  	MapMemDynamicDynamic( const char *	fileName,
//			      ios::open_mode	mode,
//			      bool		create,
//			      size_type		minChunkSize,
//			      size_type		allocSize,
//			      MapMask		permMask = 02 );
//	    Construct a object that either opens an existing map or
//	    creates a new one.
//		'fileName' is the full name of the file.
//		'mode' is the mode to open the file, ios::in == read only
//		    (ios::in | ios::out) == read/write (note if create == true
//		    the mode argument is ignored and the file is opened
//		    with (ios::in | ios::out).
//		'create' true == create a new map; false == access an
//		    existing map.
//		'minChunkSize' is the minimum size to use for allocating a
//		    chunk.
//		'allocSize' is the minumum number of bytes to allocate when
//		    the file needs to be expanded. It is also the number of
//		    bytes to leave free when the map is shrunk. Note: this
//		    value will be aligned to the next full page.
//		'permMask' is the 'umask(2)' to use when creating the file.
//		    a value of '02' will create a file with the mode
//		    set to '-rw-rw-r--'.
//
//  	MapMemDynamicDynamic( const char *  fileName,
//			      size_type	    minChunkSize,
//			      size_type	    allocSize,
//			      MapMask	    permMask = 02 );
//	    Construct a object that creates a new map.
//	    creates a new one.
//		'fileName' is the full name of the file.
//		'minChunkSize' is the minimum size to use for allocating a
//		    chunk.
//		'allocSize' is the minumum number of bytes to allocate when
//		    the file needs to be expanded. It is also the number of
//		    bytes to leave free when the map is shrunk. Note: this
//		    value will be aligned to the next full page.
//		'permMask' is the 'umask(2)' to use when creating the file.
//		    a value of '02' will create a file with the mode
//		    set to '-rw-rw-r--'.
//
//  	MapMemDynamicDynamic( const char *	fileName,
//			      ios::open_mode	mode,
//			      bool		overrideOwner = false );
//	    Construct a object that opens an existing map.
//		'fileName' is the full name of the file.
//		'mode' is the mode to open the file, ios::in == read only
//		    (ios::in | ios::out) == read/write. 
//		'overrideOwner' if this is true. The map's current owner
//		    value will be overriden. See MapMem(3) for more info.
//
//  Public Interface:
//
//	virtual
//	Loc
//	allocate( size_type size )
//	    allocate a chunk of at lease 'size' bytes and return it's 'loc'.
//	    If an error occured, 0 will be returned.
//
//	virtual
//	void
//	release( Loc loc )
//	    release a previously allocated chunk.
//
//	unsigned long
//	getChunkSize( void ) const
//	    return the total number of bytes allocated to all chunks.
//
//	unsigned long
//	getFreeSize( void ) const
//	    return the total free space available in the map. (i.e. the
//	    number of bytes not allocated)
//
//	bool
//	expand( size_type minAmount )
//	    expand the map by at lease 'minAmount' bytes. The additional
//	    space is not allocated, it is added to the free list.
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
//  	    Return the name of this class (i.e. MapMemDynamicDynamic )
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
//	DumpInfo< MapMemDynamicDynamic >
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
//	MapMemDynamicFixed( 3 )
//	MapMemDynamic( 3 )
//	MapMem( 3 )
//	MapFile( 3 )
//	ChunkMgrLoc( 3 )
//	ChunkMgr( 3 )
//
// Files:
//
// Documented Ver:  1.9
//
// Tested Ver:	1.9
//
// Revision Log:
//
// $Log$
// Revision 2.1  1997/09/21 19:21:34  houghton
// Changed version to 2
//
// Revision 1.12  1997/09/17 16:56:05  houghton
// Changed for new library rename to StlUtils
//
// Revision 1.11  1997/07/28 16:50:04  houghton
// Changed expand() to virtual.
//
// Revision 1.10  1997/07/19 10:28:53  houghton
// Port(Sun5): the compiler could not handle protected and/or private
//     classes or structs.
//
// Revision 1.9  1997/07/13 11:21:09  houghton
// Cleanup
// Added documentation.
//
// Revision 1.8  1997/06/27 12:15:31  houghton
// Major rework to speed up 'release'.
//
// Revision 1.7  1997/06/18 14:14:47  houghton
// Removed include StlUtilsUtils.
//
// Revision 1.6  1997/06/05 11:27:20  houghton
// Cleanup.
// Change to be part of libMdb (vs StlUtils1).
// Changed to be a subclass of MapMemDynamic (vs MapMem).
// Added constructor that can create or open existing.
// Changed to use new MapFile types.
// Removed addRef, getRefCount & delRef methods (now in base class).
// Changed getMem to allocate.
// Changed freeMem to release.
// Added createMapMemDynamicDynamic and openMapMemDynamicDynamic methods.
//
// Revision 1.5  1997/04/04 20:50:14  houghton
// Cleanup.
// Added map owner to prevent to progs from opening the map in write
//     mode at the same time.
//
// Revision 1.4  1997/03/18 16:56:37  houghton
// Added setNodeSize().
//
// Revision 1.3  1997/03/13 02:25:21  houghton
// Added refCount, addRef, getRefCount and delRef.
//
// Revision 1.2  1997/03/08 10:29:52  houghton
// Initial partially tested version.
//
// Revision 1.1  1997/03/07 11:51:31  houghton
// Initial Version.
//
//
#endif // ! def _MapMemDynamicDynamic_hh_ 

