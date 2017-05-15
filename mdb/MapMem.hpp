#ifndef _mdb_MapMem_hpp_
#define _mdb_MapMem_hpp_
// 1994-11-18 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MapFile.hpp>
#include <clue/DumpInfo.hpp>

#include <iostream>

namespace mdb {

class MapMem : public MapFile
{

public:

  typedef MapAddr	    MapBaseAddr;
  typedef uint32_t	    MapVersion;
  typedef int32_t	    MapOwner;

  enum MapType
  {
    MM_FIXED,	    // fixed 'map to' address
    MM_DYNAMIC,	    // dynamic 'map to' address
    MM_UNDEFINED
  };

  // use this to create an new map or open and existing one.
  MapMem( const char *		fileName,
	  MapType		type,
	  MapVersion		version,
	  std::ios::openmode    mode,
	  bool			create,
	  MapAddr		baseAddr,
	  size_type		size,
	  MapMask		permMask = 02 );


  // use this to create a new map
  MapMem( const char * 	    fileName,
	  MapAddr    	    baseAddr,
	  MapType   	    type,
	  MapVersion	    version,
	  size_type	    size,
	  MapMask	    permMask = 02 );

  // use this to open an existing map
  MapMem( const char *		fileName,
	  MapType		type,
	  MapVersion		version,
	  std::ios::openmode    mode = std::ios::in,
	  bool			overrideOwner = false );

  virtual ~MapMem( void );

  MapVersion	    getMapVersion( void ) const;
  void *    	    getMapToAddr( void ) const;
  MapType   	    getType( void ) const;
  const char *	    getTypeName( void ) const;
  size_type	    getMapSize( void ) const;
  MapOwner	    getOwner( void ) const;

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  inline DumpInfo< MapMem >
  dump( const char *	prefix = "    " ) const;

  struct MapInfo
  {
    int32_t	    type;
    MapVersion	    version;
    MapBaseAddr	    base;
    size_type	    size;
    MapOwner	    owner;	    // pid of owner (writer)
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
  void	openMapMem( const char *	fileName,
		    MapType		type,
		    MapVersion		version,
		    std::ios::openmode  mode,
		    bool		overrideOwner );

  inline const MapInfo * 	mapInfo( void ) const;
  inline MapInfo *		mapInfo( void );

  static const char * ErrorStrings[];
  static const char * TypeStrings[];

  ErrorNum	errorNum;
  int	    	osErrno;

};


inline
void *
MapMem::getMapToAddr( void ) const
{
  return( (mapInfo()) ? (void*)mapInfo()->base : 0 );
}

inline
MapMem::size_type
MapMem::getMapSize( void ) const
{
  return( (mapInfo()) ? mapInfo()->size : 0 );
}


inline
MapMem::MapInfo *
MapMem::mapInfo( void )
{
  return( (MapInfo *)MapFile::getBase() );
}

inline
const MapMem::MapInfo *
MapMem::mapInfo( void ) const
{
  return( (const MapInfo *)MapFile::getBase() );
}

}; // namespace mdb

#endif // ! def _MapMem_hh_
