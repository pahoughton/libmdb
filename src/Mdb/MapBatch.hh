#ifndef _MapBatch_hh_
#define _MapBatch_hh_
//
// File:        MapBatch.hh
// Project:	Mdb
// Desc:        
//
//  This class is intended for creating and/or accessing a file
//  containing a collection of fixed records.
//
// Quick Start: a short example of class usage
//
//  struct Rec {
//    long  a;
//    long  b;
//    char  c[50];
//  };
//
//  MapBatch<Rec>   batch( "abc.bin" );
//
//  for( MapBatch<Rec>::iterator them = batch.begin();
//	 them != batch.end();
//	 ++ them )
//    {
//	 ...
//    }
//
// Notes:
//
//  You should only use a structure or  other base type (i.e. long,
//  char ...) as `T'. Classes may contain function pointers which
//  would be no be valid in a file.
//
// Author:      Charles B. Reeves - (charles.reeves@wcom.com)
//		Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/28/97 19:25
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
#include <MapFile.hh>
#include <DumpInfo.hh>
#include <iostream>
#include <Str.hh>

template <class T>
class MapBatch
{

public:

  typedef T *		iterator;
  typedef const T *	const_iterator;
  typedef T		value_type;
  typedef MDB_TYPE_SIZE	size_type;
  typedef ptrdiff_t	difference_type;
  typedef T &		reference;
  typedef const T &	const_reference;
  
  typedef reverse_bidirectional_iterator< const_iterator,
    value_type, const_reference, difference_type >	const_reverse_iterator;
  
  typedef reverse_bidirectional_iterator< iterator,
    value_type, reference, difference_type >		reverse_iterator;

  
  inline MapBatch( const char *	    fileName,
		   ios::open_mode   mode = ios::in,
		   bool		    create = false,
		   MapFile::MapMask permMask = 02 );
  
  virtual ~MapBatch( void );

  inline const_iterator		begin( void ) const {
    return( (const_iterator)map.getBase() );
  };
  
  inline const_iterator		end( void ) const {
    return( (const_iterator)endPos );
  };
  
  inline const_reverse_iterator	rbegin( void ) const {
    return( const_reverse_iterator( end() ) );
  };
  
  inline const_reverse_iterator	rend( void ) const {
    return( const_reverse_iterator( begin() ) );
  };

  inline const_reference    front( void ) const {
    return( *begin() );
  };

  inline const_reference    back( void ) const {
    return( *(end() - 1) );
  };
  
  
  inline iterator	    begin( void ) {
    return( (iterator)map.getBase() );
  };
  
  inline iterator	    end( void ) {
    return( (iterator)endPos );
  };
  
  inline reverse_iterator   rbegin( void ) {
    return( reverse_iterator( end() ) );
  };
  
  inline reverse_iterator   rend( void ) {
    return( reverse_iterator( begin() ) );
  };

  inline reference	    front( void ) {
    return( *begin() );
  };

  inline reference	    back( void ) {
    return( *(end() - 1) );
  };
  
  inline size_type	size( void ) const {
    return( size_type( end() - begin() ) );
  };

  inline size_type	capacity( void ) const {
    return( size_type(((const_iterator)map.getEnd()) - begin()) );
  }

  bool			reserve( size_type n ) {
    if( capacity() < n ) {
      if( ! map.grow( sizeof( value_type ) * (n - capacity()), 0 ) )
	return( false );
    }
    return( true );
  };
  
  inline iterator	append( void ) {
    if( endPos + sizeof( value_type ) >= map.getEnd() ) {
      if( ! map.grow( sizeof( value_type ), 0 ) )
	return( end() );
    }
    iterator tmp( end() );
    endPos += sizeof( value_type );
    return( tmp );
  };


  inline const_reference    operator [] ( size_type rec ) const {
    return( *(begin() + rec) );
  };
  
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline
  DumpInfo< MapBatch<T> >   dump( const char *	prefix = "    ",
				  bool		showVer = true ) const;
    
protected:
  
  MapFile	    map;
  MapFile::MapAddr  endPos;

private:
  
};

#include <MapBatch.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MapBatch	class
//
//  Constructors:
//
//  	MapBatch( );
//
//  Destructors:
//
//  Public Interface:
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
//  	    Return the name of this class (i.e. MapBatch )
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
//  	operator <<( ostream & dest, const MapBatch & src );
//
//	istream &
//	operator >> ( istream & src, MapBatch & dest );
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
// Revision 2.4  1997/08/31 10:17:30  houghton
// Added operator [] to retreive a specific record.
//
// Revision 2.3  1997/08/17 01:38:39  houghton
// Bug-Fix: MapMask is part of MapFile
// Bug-Fix: endPos is not a function :)
//
// Revision 2.2  1997/08/10 20:31:58  houghton
// Cleanup.
// Added some methods.
// Removed some of the definitions (now in MapBatch.ii).
//
// Revision 2.1  1997/08/10 19:46:47  houghton
// Initial Version (by Charlie Reeves).
//
//
#endif // ! def _MapBatch_hh_ 

