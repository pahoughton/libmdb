#ifndef _ChunkMgr_hh_
#define _ChunkMgr_hh_
//
// File:        ChunkMgr.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/07/97 19:27
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
#include <MapMemDynamicDynamic.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class ChunkMgr
{

public:

  typedef off_t	    Loc;
  typedef void *    Addr;
  typedef size_t    size_type;
  
  ChunkMgr( MapMemDynamicDynamic & mapMemMgr );

  virtual ~ChunkMgr( void );

  inline Loc	    location( const Addr addr ) const;
  
  inline const Addr address( Loc loc ) const;
  inline Addr	    address( Loc loc );

  inline Loc	    allocate( size_type size );
  inline bool	    release( Loc loc );
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;

  static const Loc	    badLoc;
  
protected:

private:

  ChunkMgr( const ChunkMgr & from );
  ChunkMgr & operator =( const ChunkMgr & from );

  MapMemDynamicDynamic &	mapMem;
  
};

#if !defined( inline )
#include <ChunkMgr.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const ChunkMgr & src );

istream &
operator >> ( istream & src, const ChunkMgr & dest );


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	ChunkMgr	class
//
//  Constructors:
//
//  	ChunkMgr( );
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
//  	    Return the name of this class (i.e. ChunkMgr )
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
//  	operator <<( ostream & dest, const ChunkMgr & src );
//
//	istream &
//	operator >> ( istream & src, ChunkMgr & dest );
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
// Revision 2.1  1997/06/05 11:29:07  houghton
// Initial Version.
//
//
#endif // ! def _ChunkMgr_hh_ 

