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
  
  typedef T		Rec;
  
#if defined( STLTUTILS_STD_ITERATORS )

  typedef std::reverse_iterator< iterator,
    random_access_iterator_tag,
    value_type >					    reverse_iterator;
  typedef std::reverse_iterator< const_iterator,
    random_access_iterator_tag,
    const value_type >				    const_reverse_iterator;
  
#else
  typedef reverse_bidirectional_iterator< const_iterator,
    value_type, const_reference, difference_type >	const_reverse_iterator;
  
  typedef reverse_bidirectional_iterator< iterator,
    value_type, reference, difference_type >		reverse_iterator;
#endif
  
  inline MapBatch( const char *	    fileName,
		   ios::open_mode   mode = ios::in );
		   
  inline MapBatch( const char *	    fileName,
		   ios::open_mode   mode,
		   MapFile::MapMask permMask );
  
  inline MapBatch( const char *	    fileName,
		   ios::open_mode   mode,
		   bool		    create,
		   MapFile::MapMask permMask );
  
  virtual ~MapBatch( void );

  inline const_iterator		begin( void ) const {
    return( (const_iterator)map.getBase() );
  };
  
  inline const_iterator		end( void ) const {
    return( (const_iterator)(map.getBase() + endPos ));
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
    return( (iterator)(map.getBase() + endPos ));
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
  
  inline bool		append( const_reference rec ) {
    if( endPos + sizeof( value_type ) >= map.getSize() ) {
      if( ! map.grow( sizeof( value_type ), 0 ) )
	return( false );
    }
    iterator tmp( end() );
    *tmp = rec;
    endPos += sizeof( value_type );
    return( true );
  };
    
  inline iterator	append( void ) {
    if( endPos + sizeof( value_type ) >= map.getSize() ) {
      if( ! map.grow( sizeof( value_type ), 0 ) )
	return( end() );
    }
    iterator tmp( end() );
    endPos += sizeof( value_type );
    return( tmp );
  };

  inline bool		write( iterator it ) {
    // dummy function
    return( true );
  };

  inline bool		pop( void ) {
    if( endPos >= sizeof( value_type ) )
      {
	endPos -= sizeof( value_type );
	return( true );
      }
    else
      {
	return( false );
      }
  };
    
  inline const_reference    operator [] ( size_type rec ) const {
    return( *(begin() + rec) );
  };
  
  inline reference	    operator [] ( size_type rec ) {
    return( *(begin() + rec) );
  };
  

  inline bool		sync( size_type	beg = 0,
			      size_type	len = MapFile::npos,
			      bool	async = false ) {
    return( map.sync( beg, len, async ) );
  };
  
  inline const char *	getFileName( void ) const;
  
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
  
  MapFile		map;
  MapFile::size_type    endPos;

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
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.11  2000/07/31 13:05:06  houghton
// Port(Sun CC 5.0) changes to support Sun Workshop 5.0.
//
// Revision 2.10  1999/06/24 10:26:29  houghton
// Added getFileName().
//
// Revision 2.9  1999/05/01 12:57:38  houghton
// Reworked constructors.
//
// Revision 2.8  1999/03/02 12:57:19  houghton
// Changed constructors.
// Added append().
// Added write().
// Added pop().
//
// Revision 2.7  1997/12/19 12:41:05  houghton
// Added operator [].
//
// Revision 2.6  1997/10/01 14:03:56  houghton
// Added typedef for Rec
// Added sync().
//
// Revision 2.5  1997/09/02 13:25:53  houghton
// Bug-Fix: endpos had the address which could change due to an
//     append(). Changed so endpos is an offset and added to getBase() which
//     allways has the correct base address.
//
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

