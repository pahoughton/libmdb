#ifndef _MapMemDynamicFixed_hh_
#define _MapMemDynamicFixed_hh_
// 1994-11-28 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MapMemDynamic.hpp>
#include <clue/Clue.hpp>
#include <clue/DumpInfo.hpp>

#include <iostream>

#define MDF_VERSION 0x4d444605	// 'MDF5'

namespace mdb {

class MapMemDynamicFixed : public MapMemDynamic
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
  MapMemDynamicFixed( const char *	    fileName,
		      std::ios::openmode    mode,
		      bool		    create,
		      size_type		    recSize,
		      size_type		    allocNumRecs = 0,
		      MapMask		    permMask = 02 );

  // use this constructor to create a new map file
  MapMemDynamicFixed( const char * 	fileName,
		      size_type		recSize,
		      size_type		allocNumRecs = 0,
		      MapMask		permMask = 02 );

  // use this constructor to access an existing map file
  MapMemDynamicFixed( const char *	    fileName,
		      std::ios::openmode    mode = std::ios::in,
		      bool		    overrideOwner = false );

  virtual ~MapMemDynamicFixed( void );

  Loc	    	    allocate( size_type size = 0 );
  void	    	    release( Loc loc );

  bool		    valid( Loc loc ) const;

  size_type	    getRecSize( void ) const;
  size_type	    getAllocNumRecs( void ) const;

  virtual bool	    expand( size_type minAmount );

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  inline DumpInfo< MapMemDynamicFixed >
  dump( const char *	prefix = "    " ) const {
    return( DumpInfo< MapMemDynamicFixed >( *this, prefix ) );
  }


  // debuging / testing methods
  std::ostream &    dumpFreeList( std::ostream & dest ) const;
  std::ostream &    dumpNodes( std::ostream & dest ) const;
  bool		    allTested( void );

  // FreeNode should be protected but AIX can't deal with it.
  struct FreeNode
  {
    Loc	    nextFree;
    Loc	    prevFree;
  };

protected:

  inline Loc		    firstNode( void ) const;
  inline Loc		    lastNode( void ) const;

  inline const FreeNode &   freeList( void ) const;
  inline FreeNode &	    freeList( void );

  inline const FreeNode &   freeNode( Loc f ) const;
  inline FreeNode &	    freeNode( Loc f );


private:

  MapMemDynamicFixed( const MapMemDynamicFixed & copyFrom );
  MapMemDynamicFixed & operator=( const MapMemDynamicFixed & assignFrom );

  void	createMapMemDynamicFixed( size_type recSize,
				  size_type allocNumRecs );

  void	openMapMemDynamicFixed( void );

  // do NOT use this it is only public because Sun can't handle it
  // being private.
public:
  struct MapDynamicFixedInfo : MapDynamicInfo
  {
    size_type	recSize;	    // record size
    size_type   allocNumRecs;   // records to allocate at a time
    FreeNode	freeList;	    // head to list of free records
  };

private:

  inline MapDynamicFixedInfo *		mapInfo( void );
  inline const MapDynamicFixedInfo *	mapInfo( void ) const;

  static const char * ErrorStrings[];

  ErrorNum	errorNum;

};


inline
MapMemDynamicFixed::size_type
MapMemDynamicFixed::getRecSize( void ) const
{
  return( mapInfo() ? mapInfo()->recSize : 0 );
}

inline
MapMemDynamicFixed::size_type
MapMemDynamicFixed::getAllocNumRecs( void ) const
{
  return( mapInfo() ? mapInfo()->allocNumRecs : 0 );
}

inline
MapMemDynamicFixed::Loc
MapMemDynamicFixed::firstNode( void ) const
{
  return( clue::DwordAlign( sizeof( MapDynamicFixedInfo ) ) );
}

inline
MapMemDynamicFixed::Loc
MapMemDynamicFixed::lastNode( void ) const
{
  return( ( ( ((getMapSize() - firstNode()) / mapInfo()->recSize) - 1) *
	  mapInfo()->recSize ) + firstNode() );
}

inline
const MapMemDynamicFixed::FreeNode &
MapMemDynamicFixed::freeList( void ) const
{
  return( mapInfo()->freeList );
}

inline
MapMemDynamicFixed::FreeNode &
MapMemDynamicFixed::freeList( void )
{
  return( mapInfo()->freeList );
}

inline
const MapMemDynamicFixed::FreeNode &
MapMemDynamicFixed::freeNode( Loc f ) const
{
  return( *( (const FreeNode *)( (const char *)getBase() + f ) ));
}

inline
MapMemDynamicFixed::FreeNode &
MapMemDynamicFixed::freeNode( Loc f )
{
  return( *( (FreeNode *)( (char *)getBase() + f ) ));
}


inline
MapMemDynamicFixed::MapDynamicFixedInfo *
MapMemDynamicFixed::mapInfo( void )
{
  return( (MapDynamicFixedInfo *)getBase() );
}


inline
const MapMemDynamicFixed::MapDynamicFixedInfo *
MapMemDynamicFixed::mapInfo( void ) const
{
  return( (const MapDynamicFixedInfo *)getBase() );
}

}; // namespace mdb

#endif // ! def _MapMemDynamicFixed_hh_
