#ifndef _MapMemDlist_hh_
#define _MapMemDlist_hh_
//
// File:        MapMemDlist.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     08/18/98 09:59
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
#include <DumpInfo.hh>
#include <iostream>

template <class Value>
class MapMemDlist
{

public:

  typedef ptrdiff_t			difference_type;
  typedef const Value &			const_referance;
  typedef Value &			referance;
  typedef MultiMemOffset::size_type	size_type;
  
  typedef MultiMemOffset::Loc		Loc;

  struct Rec
  {
    Loc		next;
    Loc		prev;

    Value	value;
  };

  inline MapMemDlist( MultiMemOffset *	    memMgr )
    : dataMgr( memMgr ) {};
  
  virtual ~MapMemDlist( void ) {};

  class const_iterator;
  
  class iterator
    : public bidirectional_iterator< Value, difference_type >
  {
  public:

    inline iterator( void )
      : dlist( 0 ), recLoc( 0 ) {};

    inline iterator( const iterator & from )
      : dlist( from.dlist ), recLoc( from.recLoc ) {};

    inline iterator &	    operator ++ ( void ) {
      if( dlist ) (*dlist).next( recLoc );
      return( *this );
    };

    inline iterator &	    operator -- ( void ) {
      if( dlist ) (*dlist).prev( recLoc );
      return( *this );
    };

    inline iterator	    operator -- (int) {
      iterator it( *this );
      -- *this;
      return( it );
    };

    inline Value &	    operator * ( void ) {
      return( (*dlist).value( recLoc ) );
    };
    
    inline bool		operator == ( const iterator & rhs ) const {
      return( dlist == rhs.dlist && recLoc == rhs.recLoc );
    };
    
    inline iterator &	operator = ( const iterator & rhs ) {
      dlist = rhs.dlist;
      recLoc = rhs.recLoc;
      return( *this );
    };

  protected:

    friend class MapMemDlist< Value >;
    friend class const_iterator;

    inline Loc	    loc( void ) const {
      return( recLoc );
    };

    inline Loc &    prevLoc( void ) const {
      return( (*dlist).prevLoc( recLoc ) );
    };

    inline Loc &    nextLoc( void ) const {
      return( (*dlist).nextLoc( recLoc ) );
    };

    inline Rec &    rec( void ) const {
      return( (*dlist).rec( recLoc ) );
    };
    
    inline Rec &    nextRec( void ) const {
      return( (*dlist).nextRec( recLoc ) );
    };

    inline Rec &    prevRec( void ) const {
      return( (*dlist).prevRec( recLoc ) );
    };
    
    
    inline iterator(
      MapMemDlist< Value > *	aDlist,
      Loc			aLoc )
      : dlist( aDlist ), recLoc( aLoc ) {};

    MapMemDlist< Value > *  dlist;
    Loc			    recLoc;
    
  };

  typedef pair< iterator, bool >     pair_iterator_bool;
  
  inline pair_iterator_bool	insert( Loc &	    first,
					Loc &	    last,
					iterator &  before,
					Value &	    value ) {
    
    Loc  loc = (*dataMgr).allocate( sizeof( Rec ) );
    
    if( loc )
      {

	Rec &  r( (*((Rec *)(*dataMgr).address( loc ))));

	r.value = value;

	if( before == end() )
	  {
	    r.next = 0;
	    r.prev = last;

	    if( last )
	      rec( last ).next = loc;
	    else
	      first = loc;
	    
	    last = loc;
	  }
	else
	  {
	    r.prev = before.prevLoc();
	    r.next = before.loc();
	    
	    if( before.prevLoc() )
	      {
		before.prevRec().next = loc;
	      }
	    else
	      {
		first = loc;
	      }
	    
	    if( before.loc() )
	      {
		before.prevLoc() = loc;
	      }
	    else
	      {
		last = loc;
	      }
	  }
	return( pair_iterator_bool( iterator( this, loc ), true ) );
      }
    else
      {
	return( pair_iterator_bool( end(), false ) );
      }
  };

  
  inline void		erase( Loc &		firstLoc,
			       Loc &		lastLoc,
			       iterator &	it ) {
    if( it != end() )
      {
	if( it.nextLoc() )
	  {
	    it.nextRec().prev = it.prevLoc();
	  }
	else
	  {
	    lastLoc = it.prevLoc();
	  }

	if( it.prevLoc() )
	  {
	    it.prevRec().next = it.nextLoc();
	  }
	else
	  {
	    firstLoc = it.nextLoc();
	  }

	(*dataMgr).release( it.loc() );
      };
  };
  

  
  inline iterator	begin( Loc first ) {
    return( iterator( this, first ) );
  };
  
  inline iterator	end( void ) {
    return( iterator( this, 0 ) );
  };
  
  inline bool	    	good( void ) const;
  inline const char * 	error( void ) const;
  
#if defined( FIXME )  
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline DumpInfo< MapMemDlist >
  dump( const char * preifx = "    ", bool showVer = true ) const;

  static const ClassVersion version;
#endif
  
protected:

  friend iterator;
  friend const_iterator;

  inline const Rec &	rec( Loc loc ) const {
    return( (* ((const Rec *)((*dataMgr).address( loc )))) );
  };
  
  inline Loc	    next( Loc & loc ) const {
    return( (loc = rec( loc ).next ) );
  };

  inline Loc	    prev( Loc & loc ) const {
    return( (loc = rec( loc ).prev ) );
  };

  inline Loc	    prevLoc( Loc loc ) const {
    return( rec( loc ).prev );
  };

  inline Loc	    nextLoc( Loc loc ) const {
    return( rec( loc ).next );
  };

  inline Rec &	    rec( Loc loc ) {
    return( (* ((Rec *)((*dataMgr).address( loc )))) );
  };
  
  inline Rec &	    prevRec( Loc loc ) {
    return( rec( rec( loc ).prev ) );
  };
  
  inline Rec &	    nextRec( Loc loc ) {
    return( rec( rec( loc ).next ) );
  };

  inline Loc &	    prevLoc( Loc loc ) {
    return( rec( loc ).prev );
  };
  
  inline Loc &	    nextLoc( Loc loc ) {
    return( rec( loc ).next );
  };

  
  inline Value &	value( Loc loc ) {
    return( rec( loc ).value );
  };
  
  inline const Value &	value( Loc loc ) const {
    return( rec( loc ).value );
  };
  

  MultiMemOffset *   dataMgr;
  
private:

  MapMemDlist( const MapMemDlist & from );
  MapMemDlist & operator =( const MapMemDlist & from );

};

#include <MapMemDlist.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MapMemDlist	class
//
//  Constructors:
//
//  	MapMemDlist( );
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
//  	    Return the name of this class (i.e. MapMemDlist )
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
//  	operator <<( ostream & dest, const MapMemDlist & src );
//
//	istream &
//	operator >> ( istream & src, MapMemDlist & dest );
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
// Revision 2.2  1998/10/27 15:33:19  houghton
// Bug-Fix: if inserting to 'end' there was no 'return' statement.
//
// Revision 2.1  1998/10/23 13:20:17  houghton
// Initial Version.
//
//
#endif // ! def _MapMemDlist_hh_ 

