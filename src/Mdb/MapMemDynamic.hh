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

  typedef off_t	    Loc;
  typedef void *    Addr;

  // use this to create a new map or open an existing one
  MapMemDynamic( const char *	    fileName,
		 MapVersion	    version,
		 ios::open_mode	    mode,
		 bool		    create,
		 size_type	    size,
		 MapMask	    permMask = 0777 );
		 
  // use this to create a new map
  MapMemDynamic( const char * 	    fileName,
		 MapVersion	    version,
		 size_type	    size,
		 MapMask	    permMask = 0777 );

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

  inline long		getOwner( void ) const;
  inline unsigned long	getChunkCount( void ) const;
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
  
protected:

  enum ErrorNum
  {
    E_OK,
    E_OWNER,
    E_UNDEFINED
  };
  
  struct MapDynamicInfo : MapInfo
  {
    long	    owner;	    // pid of owner (writer)
    unsigned long   chunkCount;	    // allocated chunks
    unsigned long   freeCount;	    // available chunks
    long    	    keys[MMD_NUM_KEYS]; // general purpose values
  };

  ErrorNum	errorNum;

private:

  MapMemDynamic( const MapMemDynamic & from );
  MapMemDynamic & operator =( const MapMemDynamic & from );

  void createMapMemDynamic( void );
  void openMapMemDynamic( ios::open_mode mode, bool overrideOwner );
  
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
//  	MapMemDynamic( );
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
//  	operator <<( ostream & dest, const MapMemDynamic & src );
//
//	istream &
//	operator >> ( istream & src, MapMemDynamic & dest );
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
// Revision 2.1  1997/06/05 11:29:12  houghton
// Initial Version.
//
//
#endif // ! def _MapMemDynamic_hh_ 

