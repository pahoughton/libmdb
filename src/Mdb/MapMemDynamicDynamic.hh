#ifndef _MapMemDynamicDynamic_hh_
#define _MapMemDynamicDynamic_hh_
//
// File:        MapMemDynamicDynamic.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     03/07/97 05:41
//
// Revision History: (See end of file for Revision Log)
//
// $Id$
//

#include <MdbConfig.hh>
#include <MapMemDynamic.hh>

#include <DumpInfo.hh>

#include <iostream>

#define MDD_VERSION 0x4d444404	// 'MDD4'

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
			MapMask		permMask = 0777 );
  
  // use this constructor to create a new map file  
  MapMemDynamicDynamic( const char * 	fileName,
			size_type	minChunkSize = 16,
			size_type	allocSize = 0,
			unsigned short	permMask = 0777 );

  // use this constructor to access an existing map file  
  MapMemDynamicDynamic( const char * 	fileName,
			ios::open_mode	mode = (ios::open_mode)(ios::in),
			bool		overrideOwner = false );

  virtual ~MapMemDynamicDynamic( void );

  virtual Loc	    allocate( size_type size );	// returns offset not addr!
  virtual void 	    release( Loc  offset ); 	// needs offset not addr!
  
  inline unsigned long	    getChunkSize( void ) const;
  inline unsigned long	    getFreeSize( void ) const;
  
  void 	    	    expand( size_type minAmount );

  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  ostream &	dumpFreeList( ostream & dest ) const;
  ostream &	dumpNodes( ostream & dest ) const;
  
  static const ClassVersion version;

  inline
  DumpInfo< MapMemDynamicDynamic >  dump( const char *	prefix = "    ",
					  bool		showVer = true ) const;

  // FreeList should be protected but AIX can't deal with it.
  
  struct FreeList
  {
    unsigned long   size;
    unsigned long   next;
    unsigned long   prev;
  };

protected:

  inline FreeList *	    getFreeNode( off_t f );
  inline const FreeList *   getFreeNode( off_t f ) const ;
  inline bool		    setPrevFnodeNext( off_t f, off_t n );
  inline bool		    setNextFnodePrev( off_t f, off_t p );

  inline size_type &   setNodeSize( off_t node, size_type nodeSize );
  inline size_type &   getNodeSize( off_t node );
  inline size_type     getNodeSize( off_t node ) const;

private:

  MapMemDynamicDynamic( const MapMemDynamicDynamic & from );
  MapMemDynamicDynamic & operator =( const MapMemDynamicDynamic & from );

  void	    createMapMemDynamicDynamic( size_type   minChunkSize,
					size_type   allocSize );

  void	    openMapMemDynamicDynamic( void );

  struct MapDynamicDynamicInfo : MapDynamicInfo
  {
    unsigned long   minChunkSize;   // minimum chunk size
    unsigned long   allocSize;	    // bytes to allocate at a time
    unsigned long   chunkSize;	    // total allocated size
    unsigned long   freeSize;	    // total available size;
    struct FreeList freeList;	    // head to list of free chunks
  };

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
//
//  Constructors:
//
//  	MapMemDynamicDynamic( );
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
//  	operator <<( ostream & dest, const MapMemDynamicDynamic & src );
//
//	istream &
//	operator >> ( istream & src, MapMemDynamicDynamic & dest );
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
// Revision 1.7  1997/06/18 14:14:47  houghton
// Removed include ClueUtils.
//
// Revision 1.6  1997/06/05 11:27:20  houghton
// Cleanup.
// Change to be part of libMdb (vs Clue1).
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

