#ifndef _MapMemDynamicDynamic_hh_
#define _MapMemDynamicDynamic_hh_
//
// File:        MapMemDynamicDynamic.hh
// Project:	Clue
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

#include <ClueConfig.hh>
#include <MapMem.hh>
#include <Record.hh>

#include <ClueUtils.hh>
#include <DumpInfo.hh>

#include <iostream>

#define MMD_VERSION 0x4d4d4403	// 'MMD3'

#define NUM_KEYS    16

#if defined( CLUE_DEBUG )
#define inline
#endif


class MapMemDynamicDynamic : public MapMem
{

public:

  enum ErrorNum
  {
    E_OK,
    E_MAPMEM,
    E_BADSIZE,
    E_OWNER,
    E_UNDEFINED
  };
    
  // use this constructor to create a new map file  
  MapMemDynamicDynamic( const char * 	fileName,
			size_t		minChunkSize = 16,
			size_t		allocSize = 0,
			unsigned short	permMask = 0 );

  // use this constructor to access an existing map file  
  MapMemDynamicDynamic( const char * 	fileName,
			ios::open_mode	mode = (ios::open_mode)(ios::in),
			bool		overrideOwner = false );

  virtual ~MapMemDynamicDynamic( void );

  off_t	    	    getMem( size_t size );	// returns offset not addr!
  void	    	    freeMem( off_t  offset ); 	// needs offset not addr!
  
  inline const void *	    getAddr( off_t offset ) const;
  inline void *    	    getAddr( off_t offset );
  inline off_t	    	    getOffset( const void * addr ) const;

  inline unsigned long	    getChunkCount( void ) const;
  inline unsigned long	    getChunkSize( void ) const;
  inline unsigned long	    getFreeCount( void ) const;
  inline unsigned long	    getFreeSize( void ) const;
  
  inline long	    setKey( long value, unsigned short key = 0 );
  inline long	    getKey( unsigned short key = 0) const;
  
  void 	    	    expand( size_t minAmount );

  inline long		    addRef( void );
  inline long		    getRefCount( void ) const;
  inline bool		    delRef( void );
  
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

  inline size_t &   setNodeSize( off_t node, size_t nodeSize );
  inline size_t &   getNodeSize( off_t node );
  inline size_t     getNodeSize( off_t node ) const;

private:

  struct MapDynamicDynamicInfo : MapInfo
  {
    long	    owner;	    // pid of owner (writer)
    unsigned long   minChunkSize;   // minimum chunk size
    unsigned long   allocSize;	    // bytes to allocate at a time
    unsigned long   chunkCount;	    // allocated chunks
    unsigned long   chunkSize;	    // total allocated size
    unsigned long   freeCount;	    // available chunks
    unsigned long   freeSize;	    // total available size;
    long    	    keys[NUM_KEYS]; // general purpose values
    struct FreeList freeList;	    // head to list of free chunks
  };

  static const char *	ErrorStrings[];
  
  ErrorNum			    errorNum;
  struct MapDynamicDynamicInfo *    base;

  off_t				    nextFree;

  long				    refCount;
  
  MapMemDynamicDynamic( const MapMemDynamicDynamic & from );
  MapMemDynamicDynamic & operator =( const MapMemDynamicDynamic & from );
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

