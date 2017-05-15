#ifndef _mdb_MapMemDynamicDynamic_hpp_
#define _mdb_MapMemDynamicDynamic_hpp_
/* 1997-03-07 (cc) Paul Houghton <paul4hough@gmail.com>

   Provide chunk management for a Dynamic mapped memory file.
   it allows you to allocate and release chunks from 1 to 'maxChunk' (~2 gig)
   in size. The operating system's mapped memory functions are used to
   map the chunks to a disk file.

   See libMdb/docs/design/MapMemDynamcDynamc.txt for more info.
*/

#include <mdb/MapMemDynamic.hpp>

#define MDD_VERSION 0x4d444406	// 'MDD6'

namespace mdb {

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
  MapMemDynamicDynamic( const char *	    fileName,
			std::ios::openmode  mode,
			bool		    create,
			size_type	    minChunkSize,
			size_type	    allocSize,
			MapMask		    permMask = 02 );

  // use this constructor to create a new map file
  MapMemDynamicDynamic( const char * 	fileName,
			size_type	minChunkSize,
			size_type	allocSize = 0,
			unsigned short	permMask = 02 );

  // use this constructor to access an existing map file
  MapMemDynamicDynamic( const char *	    fileName,
			std::ios::openmode  mode = std::ios::in,
			bool		    overrideOwner = false );

  virtual ~MapMemDynamicDynamic( void );

  virtual Loc	    allocate( size_type size );	// returns offset not addr!
  virtual void 	    release( Loc  loc ); 	// needs offset not addr!

  unsigned long	    getChunkSize( void ) const;
  unsigned long	    getFreeSize( void ) const;

  virtual bool	    expand( size_type minAmount );

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  inline DumpInfo< MapMemDynamicDynamic >
  dump( const char *	prefix = "    " ) const {
    return( DumpInfo<MapMemDynamicDynamic>( *this, prefix ) );
  };


  // debuging / testing methods
  std::ostream &	dumpFreeList( std::ostream & dest ) const;
  std::ostream &	dumpNodes( std::ostream & dest ) const;

  // do NOT use these
  // Node & FreeNode should be protected but AIX & Sun can't deal with it.

  struct Node
  {
    Loc	    next;
    Loc	    prev;
  };

  struct FreeNode : public Node
  {
    Loc	    nextFree;
    Loc	    prevFree;
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
    size_type   minChunkSize;   // minimum chunk size
    size_type   allocSize;	    // bytes to allocate at a time
    size_type   chunkSize;	    // total allocated size
    size_type   freeSize;	    // total available size;
    struct FreeNode freeList;	    // head to list of free chunks
  };
private:

  inline MapDynamicDynamicInfo *	mapInfo( void );
  inline const MapDynamicDynamicInfo *  mapInfo( void ) const;

  static const char *	ErrorStrings[];

  ErrorNum	    errorNum;

};


inline
MapMemDynamicDynamic::FreeNode &
MapMemDynamicDynamic::freeNode( Loc f )
{
  return( *( (FreeNode *)(getBase() + f)) );
}

inline
const MapMemDynamicDynamic::FreeNode &
MapMemDynamicDynamic::freeNode( Loc f ) const
{
  return( *( (const FreeNode *)(getBase() + f)) );
}

inline
MapMemDynamicDynamic::size_type
MapMemDynamicDynamic::freeNodeSize( Loc f ) const
{
  return( freeNode( f ).next ?
	  abs( freeNode( f ).next ) - f :
	  getMapSize() - f );
}

inline
MapMemDynamicDynamic::FreeNode &
MapMemDynamicDynamic::freeList( void )
{
  return( mapInfo()->freeList );
}

inline
const MapMemDynamicDynamic::FreeNode &
MapMemDynamicDynamic::freeList( void ) const
{
  return( mapInfo()->freeList );
}

inline
void
MapMemDynamicDynamic::setNextPrev( Loc f, Loc p )
{
  Loc next = freeNode( f ).next;

  if( next )
    freeNode( next ).prev = p;
  else
    freeList().prev = abs( p );
}

inline
void
MapMemDynamicDynamic::setFreePrevNext( Loc f, Loc n )
{
  Loc prev = freeNode( f ).prevFree;

  if( prev )
    freeNode( prev ).nextFree = n ;
  else
    freeList().nextFree = n;
}

inline
void
MapMemDynamicDynamic::setFreeNextPrev( Loc f, Loc p )
{
  Loc next = freeNode( f ).nextFree;

  if( next )
    freeNode( next ).prevFree = p;
  else
    freeList().prevFree = p;
}

inline
MapMemDynamicDynamic::Loc
MapMemDynamicDynamic::nodeLoc( Loc f ) const
{
  return( f + sizeof( Node ) );
}

inline
MapMemDynamicDynamic::Loc
MapMemDynamicDynamic::freeLoc( Loc n ) const
{
  return( n - sizeof( Node ) );
}


inline
MapMemDynamicDynamic::MapDynamicDynamicInfo *
MapMemDynamicDynamic::mapInfo( void )
{
  return( (MapDynamicDynamicInfo *)getBase() );
}

inline
const MapMemDynamicDynamic::MapDynamicDynamicInfo *
MapMemDynamicDynamic::mapInfo( void ) const
{
  return( (const MapDynamicDynamicInfo *)getBase() );
}


}; // namespace mdb


/*
  	MapMemDynamicDynamic( const char *	fileName,
			      std::ios::openmode	mode,
			      bool		create,
			      size_type		minChunkSize,
			      size_type		allocSize,
			      MapMask		permMask = 02 );
	    Construct a object that either opens an existing map or
	    creates a new one.
		'fileName' is the full name of the file.
		'mode' is the mode to open the file, std::ios::in == read only
		    (std::ios::in | std::ios::out) == read/write (note if create == true
		    the mode argument is ignored and the file is opened
		    with (std::ios::in | std::ios::out).
		'create' true == create a new map; false == access an
		    existing map.
		'minChunkSize' is the minimum size to use for allocating a
		    chunk.
		'allocSize' is the minumum number of bytes to allocate when
		    the file needs to be expanded. It is also the number of
		    bytes to leave free when the map is shrunk. Note: this
		    value will be aligned to the next full page.
		'permMask' is the 'umask(2)' to use when creating the file.
		    a value of '02' will create a file with the mode
		    set to '-rw-rw-r--'.

  	MapMemDynamicDynamic( const char *  fileName,
			      size_type	    minChunkSize,
			      size_type	    allocSize,
			      MapMask	    permMask = 02 );
	    Construct a object that creates a new map.
	    creates a new one.
		'fileName' is the full name of the file.
		'minChunkSize' is the minimum size to use for allocating a
		    chunk.
		'allocSize' is the minumum number of bytes to allocate when
		    the file needs to be expanded. It is also the number of
		    bytes to leave free when the map is shrunk. Note: this
		    value will be aligned to the next full page.
		'permMask' is the 'umask(2)' to use when creating the file.
		    a value of '02' will create a file with the mode
		    set to '-rw-rw-r--'.

  	MapMemDynamicDynamic( const char *	fileName,
			      std::ios::openmode	mode,
			      bool		overrideOwner = false );
	    Construct a object that opens an existing map.
		'fileName' is the full name of the file.
		'mode' is the mode to open the file, std::ios::in == read only
		    (std::ios::in | std::ios::out) == read/write.
		'overrideOwner' if this is true. The map's current owner
		    value will be overriden. See MapMem(3) for more info.

  Public Interface:

	virtual
	Loc
	allocate( size_type size )
	    allocate a chunk of at lease 'size' bytes and return it's 'loc'.
	    If an error occured, 0 will be returned.

	virtual
	void
	release( Loc loc )
	    release a previously allocated chunk.

	unsigned long
	getChunkSize( void ) const
	    return the total number of bytes allocated to all chunks.

	unsigned long
	getFreeSize( void ) const
	    return the total free space available in the map. (i.e. the
	    number of bytes not allocated)

	bool
	expand( size_type minAmount )
	    expand the map by at lease 'minAmount' bytes. The additional
	    space is not allocated, it is added to the free list.

  	virtual Bool
  	good( void ) const;
  	    Return true if there are no detected errors associated
  	    with this class, otherwise false.

  	virtual const char *
  	error( void ) const;
  	    Return a string description of the state of the class.

	virtual ostream &
	dumpInfo( ostream & dest, const char * prefix );
	    output detail info to dest. Includes instance variable
	    values, state info & version info.

	inline
	DumpInfo< MapMemDynamicDynamic >
	dump( const char * previx )
	    return an object that can be passed to operator << ( ostream & )
	    that will generate the same output as 'dumpInfo'.

*/
#endif // ! def _MapMemDynamicDynamic_hh_
