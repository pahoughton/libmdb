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
#include <iostream>

#include <ClueUtils.hh>
#include <MapMem.hh>
#include <Record.hh>

#include <iostream.h>

#define MMF_VERSION 0x4d4d4402	// 'MMD1'

#define NUM_KEYS    16

#if defined( CLUE_DEBUG )
#define inline
#endif


class MapMemDynamicDynamic : public MapMem
{

public:

  enum MapFixedDynamicError
  {
    E_OK,
    E_MAPMEM,
    E_BADSIZE,
    E_UNDEFINED
  };
    
  // use this constructor to create a new map file  
  MapMemDynamicDynamic( const char * 	fileName,
			size_t		recSize,
			unsigned long	numRecs = 0,
			unsigned short	permMask = 0 );

  // use this constructor to access an existing map file  
  MapMemDynamicDynamic( const char * 	fileName,
			ios::open_mode	mode = (ios::open_mode)(ios::in) );

  virtual ~MapMemDynamicDynamic( void );

  off_t	    	    getMem( size_t size );	// returns offset not addr!
  void	    	    freeMem( off_t  offset ); 	// needs offset not addr!
  
  void *    	    getAddr( off_t offset ) const;
  off_t	    	    getOffset( void * addr ) const;

  // size_t	    getRecSize( void ) const;
  size_t	    getChunkSize( void ) const;
  unsigned long	    getRecCount( void ) const;
  unsigned long	    getFreeRecCount( void ) const;
  unsigned long	    getFreeRecSize( void ) const;
    
  long	    	    setKey( long value, unsigned short key = 0 );
  long	    	    getKey( unsigned short key = 0) const;
  
  void 	    	    expand( void );

  RecNumber   	    first( void );
  bool	    	    next( RecNumber & rec );
  
  off_t	    	    recNum2Offset( RecNumber recNum ) const;
  RecNumber	    offset2RecNum( off_t offset ) const;
  
  virtual ostream & 	getStats( ostream & dest ) const;
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;

protected:

private:

  MapMemDynamicDynamic( const MapMemDynamicDynamic & from );
  MapMemDynamicDynamic & operator =( const MapMemDynamicDynamic & from );

};

#if !defined( inline )
#include <MapMemDynamicDynamic.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const MapMemDynamicDynamic & src );

istream &
operator >> ( istream & src, const MapMemDynamicDynamic & dest );


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
// Revision 1.1  1997/03/07 11:51:31  houghton
// Initial Version.
//
//
#endif // ! def _MapMemDynamicDynamic_hh_ 

