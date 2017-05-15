#ifndef _MapMemDynamic_hh_
#define _MapMemDynamic_hh_
/* 1997-05-20 (cc) Paul Houghton <paul4hough@gmail.com>

   Base class for MapMemDynamicDynamic and MapMemDynamicFixed
*/
#include <mdb/MapMem.hpp>
#include <clue/DumpInfo.hpp>

#include <iostream>

#define MMD_NUM_KEYS	32

namespace mdb {

class MapMemDynamic : public MapMem
{

public:

  typedef char *	    Addr;
  typedef off_t		    Loc;
  typedef int64_t	    KeyValue;

  // use this to create a new map or open an existing one
  MapMemDynamic( const char *	    fileName,
		 MapVersion	    version,
		 std::ios::openmode mode,
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
		 std::ios::openmode mode = std::ios::in,
		 bool		    overrideOwner = false );

  virtual ~MapMemDynamic( void );

  virtual Loc		allocate( size_type size ) = 0;
  virtual void		release( Loc loc ) = 0;

  virtual bool		expand( size_type minAmount ) = 0;

  inline Addr		address( Loc loc );
  inline const Addr	address( Loc loc ) const;
  inline Loc		location( const Addr addr ) const;

  inline bool		reserveKey( unsigned short key );
  inline bool		setNewKey( unsigned short key, KeyValue value );

  inline bool		setKey( unsigned short key, KeyValue value );
  inline KeyValue	getKey( unsigned short key ) const;

  inline unsigned long	getAllocCount( void ) const;
  inline unsigned long	getFreeCount( void ) const;

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  inline DumpInfo< MapMemDynamic >
  dump( const char *  prefix = "    " ) const {
    return( DumpInfo< MapMemDynamic >( *this, prefix ) );
  };

  static const unsigned short numKeys;

  struct MapDynamicInfo : public MapInfo
  {
    uint32_t    allocCount;	    // allocated chunks
    uint32_t	freeCount;	    // available chunks
    uint32_t	keysUsed;	    // bitmask of keys in use
    KeyValue	keys[MMD_NUM_KEYS]; // general purpose values
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


inline
MapMemDynamic::Addr
MapMemDynamic::address( Loc loc )
{
  return( getBase() ? (Addr)( getBase() + loc ) : 0 );
}

inline
const MapMemDynamic::Addr
MapMemDynamic::address( Loc loc ) const
{
  return( getBase() ? (const Addr)( getBase() + loc ) : 0 );
}

inline
MapMemDynamic::Loc
MapMemDynamic::location( const Addr addr ) const
{
  return( getBase() ? (Loc)addr - (Loc)getBase() : 0 );
}

inline
bool
MapMemDynamic::reserveKey( unsigned short key )
{
  if( mapInfo() && ( getMode() & std::ios::out ) && key < numKeys )
    {
      clue::Bitmask   used( mapInfo()->keysUsed );

      if( ! used[ key ] )
	{
	  used.set( key );
	  mapInfo()->keysUsed = used.to_ulong();
	  return( true );
	}
      else
	{
	  return( false );
	}
    }
  else
    {
      return( false );
    }
}

inline
bool
MapMemDynamic::setNewKey( unsigned short key, KeyValue value )
{
  if( reserveKey( key ) )
    {
      mapInfo()->keys[ key ] = value;
      return( true );
    }
  else
    {
      return( false );
    }
}

inline
bool
MapMemDynamic::setKey( unsigned short key, KeyValue value )
{
  if( mapInfo() && ( getMode() & std::ios::out ) && key < numKeys )
    {
      clue::Bitmask	used( mapInfo()->keysUsed );
      if( used[ key ] == true )
	{
	  mapInfo()->keys[ key ] = value;
	  return( true );
	}
      else
	{
	  return( false );
	}
    }
  else
    {
      return( false );
    }
}

inline
MapMemDynamic::KeyValue
MapMemDynamic::getKey( unsigned short key ) const
{
  return( mapInfo() ? mapInfo()->keys[ key ] : -1 );
}

inline
unsigned long
MapMemDynamic::getAllocCount( void ) const
{
  return( mapInfo() ? mapInfo()->allocCount : 0 );
}

inline
unsigned long
MapMemDynamic::getFreeCount( void ) const
{
  return( mapInfo() ? mapInfo()->freeCount : 0 );
}

inline
MapMemDynamic::MapDynamicInfo *
MapMemDynamic::mapInfo( void )
{
  return( (MapDynamicInfo *)getBase() );
}

inline
const MapMemDynamic::MapDynamicInfo *
MapMemDynamic::mapInfo( void ) const
{
  return( (const MapDynamicInfo *)getBase() );
}

}; // namespace mdb

/*

  	MapMemDynamic( const char *	fileName,
		       MapVersion	version,
		       std::ios::openmode	mode,
		       bool		create,
		       size_type	size
		       MapMask		permMask = 02 );
	    Construct a object that either opens an existing map or
	    creates a new one.
		'fileName' is the full name of the file.
		'version' the map structure version.
		'mode' is the mode to open the file, std::ios::in == read only
		    (std::ios::in | std::ios::out) == read/write (note if create == true
		    the mode argument is ignored and the file is opened
		    with (std::ios::in | std::ios::out).
		'create' true == create a new map; false == access an
		    existing map.
		'size' is the minumum initial size of the map.
		'permMask' is the 'umask(2)' to use when creating the file.
		    a value of '02' will create a file with the mode
		    set to '-rw-rw-r--'.

  	MapMemDynamic( const char *  fileName,
		       MapVersion	version,
		       std::ios::openmode	mode,
		       bool		create,
		       size_type	size
		       MapMask		permMask = 02 );
	    Construct a object that creates a new map.
	    creates a new one.
		'fileName' is the full name of the file.
		'version' the map structure version.
		'size' is the minumum initial size of the map.
		'permMask' is the 'umask(2)' to use when creating the file.
		    a value of '02' will create a file with the mode
		    set to '-rw-rw-r--'.

  	MapMemDynamic( const char *	fileName,
		       MapVersion	version,
		       std::ios::openmode	mode,
		       bool		overrideOwner = false );
	    Construct a object that opens an existing map.
		'fileName' is the full name of the file.
		'version' the map structure version.
		'mode' is the mode to open the file, std::ios::in == read only
		    (std::ios::in | std::ios::out) == read/write.
		'overrideOwner' if this is true. The map's current owner
		    value will be overriden. See MapMem(3) for more info.

  Public Interface:

	virtual
	Loc
	allocate( size_type size ) = 0
	    allocate a chunk of at lease 'size' bytes and return it's 'loc'.
	    If an error occured, 0 will be returned.

	virtual
	void
	release( Loc loc ) = 0
	    release a previously allocated chunk.

	inline
	Addr
	address( Loc loc )
	    return a useable address for the 'loc'. This address is
	    valid only until the next call to allocate or release.

	inline
	const Addr
	address( Loc loc ) const
	    return a useable address for the 'loc'. This address is
	    valid only until the next call to allocate or release.

	inline
	Loc
	location( const Addr addr ) const
	    return the 'loc' for the 'addr'. The 'addr' must have been
	    obtained by a call to address() for the same map.

	inline
	long
	setKey( unsigned short key, long value )
	    The map has room for 'maxKey' (16) key values in the header.
	    This method is use to store a specific value in one of them.
	    returns the previous value for the specific key.

	inline
	long
	getKey( unsigned short key ) const
	    return the value stored at the specific key.

	inline
	unsigned long
	getChunkCount( void ) const
	    return the total number of chunks allocated.

	inline
	unsigned long
	getFreeCount( void ) const
	    return the total number of free chunks.

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
	DumpInfo< MapMemDynamic >
	dump( const char * previx )
	    return an object that can be passed to operator << ( ostream & )
	    that will generate the same output as 'dumpInfo'.


*/
#endif // ! def _MapMemDynamic_hh_
