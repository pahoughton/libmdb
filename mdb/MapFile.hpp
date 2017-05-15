#ifndef _mdb_MapFile_hpp_
#define _mdb_MapFile_hpp_
/* 1994-11-11 (cc) Paul Houghton <paul4hough@gmail.com>

  MapFile uses the operating system's mapped memory functions
  (mmap(2) & munmap(2)) to map a file to memory. A file can be
  mapped to a specific memory address or the operating system can
  select the address to use (See libMdb/docs/design/MapFile.txt
  for more info).

  Notes:

  When using dynamicly addressed maps (baseAddr == 0). The values
  returned by 'getBase()' and 'getEnd()' can change any time
  'setSize()', 'grow()' or 'shrink()' is called. So, it is best to
  never have a variable that contains a pointer into the map. The
  best thing to do is always call getBase() to access the data.
*/

#include <clue/FileStat.hpp>
#include <clue/DumpInfo.hpp>

#include <sys/types.h>

namespace mdb {

class MapFile
{

public:

  typedef size_t	    size_type;

  typedef char *	    MapAddr;
  typedef unsigned short    MapMask;	// map permision mask (i.e. umask)

  static const size_type    npos;

  // use this constructor to create a new file or open an existing one
  MapFile( const char *		fileName,
	   MapAddr		baseAddr,
	   std::ios::openmode	mode,
	   bool			create,
	   size_type		size,
	   MapMask		permMask = 02 );

  // use this constructor to create a new file
  MapFile( const char *     fileName,
	   size_type   	    size,
 	   MapAddr   	    baseAddr = 0,
	   MapMask	    permMask = 02 );

  // use this constructor to open an existing file
  MapFile( const char *		fileName,
	   MapAddr		baseAddr = 0,
	   std::ios::openmode   mode = std::ios::in );

  MapFile( void );

  virtual ~MapFile( void );

  size_type	    createMap( const char * fileName,
			       MapAddr	    baseAddr,
			       size_type    size,
			       MapMask	    permMask = 02 );

  size_type    	    map( const char *		fileName,
			 MapAddr		baseAddr = 0,
			 std::ios::openmode	mode = std::ios::in );

  void		    unmap( void );

  bool		    sync( size_type	beg = 0,
			  size_type	len = npos,
			  bool		async = false );

  size_type		setSize( size_type size, MapAddr baseAddr );
  inline size_type	grow( size_type amount, MapAddr baseAddr );
  inline size_type      shrink( size_type amount, MapAddr baseAddr );

  const clue::FileStat &	getFileStat( void ) const;
  const char *			getFileName( void ) const;
  const char *			getAccess( void ) const;
  inline std::ios::openmode	getMode( void ) const;
  inline size_type		getSize( void ) const;
  inline MapAddr		getBase( void );
  inline const MapAddr		getBase( void ) const;
  inline MapAddr		getEnd( void );
  inline const MapAddr		getEnd( void ) const;
  static size_type		getPageSize( void  );

  // reference counting
  inline long	    addRef( void );
  inline long	    getRefCount( void ) const;
  inline bool	    delRef( void );

  static size_type  getTotalMapped( void );

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual int		    getErrno( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &    dest = std::cerr,
				      const char *	prefix = "    " ) const;

  inline DumpInfo< MapFile >
  dump( const char *  prefix = "    " ) const {
    return( DumpInfo< MapFile >( *this, prefix ) );
  };

protected:

  static size_type	totalMapped;

private:

  MapFile( const MapFile & copyFrom );
  MapFile & operator=( const MapFile & assignFrom );

  clue::FileStat  	fileStat;

  int			mapFd;
  std::ios::openmode    mapMode;
  size_type		mapSize;
  MapAddr		mapBase;

  long			refCount;

  int			pageSize;

  int			osErrno;

};


inline
MapFile::size_type
MapFile::grow( size_type amount, caddr_t baseAddr )
{
  return( setSize( mapSize + amount, baseAddr ) );
}

inline
MapFile::size_type
MapFile::shrink( size_type amount, caddr_t baseAddr )
{
  return( setSize( mapSize - amount, baseAddr ) );
}

inline
std::ios::openmode
MapFile::getMode( void ) const
{
  return( mapMode );
}

inline
MapFile::size_type
MapFile::getSize( void ) const
{
  return( mapSize );
}

inline
caddr_t
MapFile::getBase( void )
{
  return( mapBase );
}

inline
const caddr_t
MapFile::getBase( void ) const
{
  return( mapBase );
}

inline
caddr_t
MapFile::getEnd( void )
{
  return( ((char *)mapBase) + mapSize );
}

inline
const caddr_t
MapFile::getEnd( void ) const
{
  return( ((char *)mapBase) + mapSize );
}

inline
long
MapFile::addRef( void )
{
  return( ++ refCount );
}

inline
long
MapFile::getRefCount( void ) const
{
  return( refCount );
}

inline
bool
MapFile::delRef( void )
{
  return( (--refCount <= 0 ) ? true : false );
}


}; // namespace mdb

/*
  Data Types: - data types defined by this header

  	MapFile	class

	MapFile::size_type
	MapFile::MapAddr
	MapFile::MapMask

  Constructors:

  	MapFile( const char *	    fileName,
		 MapAddr	    baseAddr,
		 std::ios::openmode	    mode,
		 bool		    create,
		 size_type	    size,
		 MapMask	    permMask = 02 )
	    Create a new map or open an existing one. If create is true
	    and 'fileName' already exists, it will be removed before the
	    map is created.
		'fileName' is the full name of the file to use.
		'baseAddr' is the base address to map the file to. 0
		    should be use to allow the OS to select the best
		    address.
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

  	MapFile( const char *	    fileName,
		 size_type	    size,
		 MapAddr	    baseAddr = 0,
		 MapMask	    permMask = 02 )
	    Create a new map. Note, if 'fileName' already exist, it will be
	    removed before the map is created.
		'fileName' is the full name of the file to use.
		'baseAddr' is the base address to map the file to. 0
		    should be use to allow the OS to select the best
		    address.
		'size' is the minumum initial size of the map.
		'permMask' is the 'umask(2)' to use when creating the file.
		    a value of '02' will create a file with the mode
		    set to '-rw-rw-r--'.


  	MapFile( const char *	    fileName,
		 MapAddr	    baseAddr = 0,
		 std::ios::openmode	    mode = std::ios::in )
	    Open an existing map.
		'fileName' is the full name of the file to use.
		'baseAddr' is the base address to map the file to. 0
		    should be use to allow the OS to select the best
		    address.
		'mode' is the mode to open the file, std::ios::in == read only
		    (std::ios::in | std::ios::out) == read/write (note if create == true
		    the mode argument is ignored and the file is opened
		    with (std::ios::in | std::ios::out).

	MapFile( void )
	    Instanciate a MapFile. Use 'createMap()' or 'map()' to create
	    or open a map.

  Public Interface:

	size_type
	createMap( const char *	fileName,
		   MapAddr	baseAddr,
		   size_type    size,
		   MapMask	permMask = 02 );
	    This method will 'unmap()' the any existing map and create a
	    new map file.  Note, if 'fileName' already exist, it will be
	    removed before the map is created.
		'fileName' is the full name of the file to use.
		'baseAddr' is the base address to map the file to. 0
		    should be use to allow the OS to select the best
		    address.
		'size' is the minumum initial size of the map.
		'permMask' is the 'umask(2)' to use when creating the file.
		    a value of '02' will create a file with the mode
		    set to '-rw-rw-r--'.
	    The size of the new map is returned or 0 if an error occured.

	size_type
	map( const char *   fileName,
	     MapAddr	    baseAddr,
	     std::ios::openmode mode = std::ios::in )
	    Open an existing map.
		'fileName' is the full name of the file to use.
		'baseAddr' is the base address to map the file to. 0
		    should be use to allow the OS to select the best
		    address.
		'mode' is the mode to open the file, std::ios::in == read only
		    (std::ios::in | std::ios::out) == read/write (note if create == true
		    the mode argument is ignored and the file is opened
		    with (std::ios::in | std::ios::out).
	    The size of the map is returned or 0 if an error occured.

	void
	unmap( void )
	    Unmap the current map file.

	size_type
	setSize( size_type size, MapAddr baseAddr )
	    Change the size of the map file to 'size'. 'baseAddr' is
	    the address to remap the file to. Since the map
	    file MUST be page aligned, the new size is returned. It will
	    be either 'size' or a greater value (up to the next page).
	    If an error occures, 0 is returned.

	inline
	size_type
	grow( size_type amount, MapAddr baseAddr )
	    Increase the map's size by at least 'amount' bytes. 'baseAddr'
	    is the address to remap the file to. Since the map
	    file MUST be page aligned, the new size is returned.
	    If an error occures, 0 is returned.

	inline
	size_type
	shrink( size_type amount, MapAddr baseAddr )
	    Reduce the map's size by 'amount' bytes. 'baseAddr'
	    is the address to remap the file to. Since the map
	    file MUST be page aligned, the new size is returned.
	    If an error occures, 0 is returned.

	const FileStat &
	getFileStat( void ) const
	    Return the 'FileStat(3)' for the map file. Note, this is only
	    updated by calls to 'map()' and 'createMap()' and the
	    constructors, so it may accurately represent the values
	    for the map file.

	const char *
	getFileName( void ) const
	    Return the file name of the map file.

	const char *
	getAccess( void ) const
	    Return the maps access capablilties. This will be one of:
	    "RW", "R", or "W", depending on the mode used to open the
	    map file.

	inline
	std::ios::openmode
	getMode( void ) const
	    Return the mode used to open the map. Note, newly created
	    maps will have a mode of (std::ios::in|std::ios::out).

	inline
	size_type
	getSize( void ) const
	    Return the map file's size.

	inline
	MapAddr
	getBase( void )
	    Return the map's base address.

	inline
	const MapAddr
	getBase( void ) const
	    Return the map's base address as a const pointer.

	inline
	MapAddr
	getEnd( void )
	    Return the map's end address. This is 'getBase() + getSize()'.
	    It is the first INVALID address beyond the map's range.

	inline
	const MapAddr
	getEnd( void )
	    Return the map's end address as a const pointer. This is
	    'getBase() + getSize()'. It is the first INVALID address
	    beyond the map's range.

	static
	size_type
	getPageSize( void )
	    Return the number of bytes in a page. This come directly
	    the operating system being used.

	inline
	long
	addRef( void )
	    This can be used for referance counting. The referance counter
	    is initialzed to 0 by the constructors.

	inline
	long
	getRefCount( void ) const
	    Return the current referance count value.

	inline
	bool
	delRef( void )
	    This can be used for referance counting. It will return
	    true when the referance count reaches 0.

  	virtual Bool
  	good( void ) const;
  	    Return true if there are no detected errors associated
  	    with this class, otherwise false.

  	virtual const char *
  	error( void ) const;
  	    Return a string description of the state of the class.

  	virtual const char *
  	getClassName( void ) const;
  	    Return the name of this class (i.e. MapFile )

  	virtual const char *
  	getVersion( bool withPrjVer = true ) const;
  	    Return the version string of this class.

	virtual ostream &
	dumpInfo( ostream & dest, const char * prefix, bool showVer );
	    output detail info to dest. Includes instance variable
	    values, state info & version info.


*/
#endif // ! def _mdb_MapFile_hpp_
