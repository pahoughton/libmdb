#ifndef _DashTable_hh_
#define _DashTable_hh_
//
// File:        DashTable.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/02/97 08:47
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
#include <DashTableBase.hh>
#include <iostream>

#include <DumpInfo.hh>

#include <iterator>

template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
class DashTable : public DashTableBase
{

public:

  // * * * Types * * *
  typedef DashTable< Key, Value, KeyOfValue, HashFunct, LessKey > self;
  
  typedef ptrdiff_t		    difference_type;
  typedef const Value &		    const_referance;
  typedef Value &		    referance;
  
  struct DashNode : public DashNodeBase
  {
    Value	value;
  };
  
  class const_iterator;
  
  class iterator
    : public std::iterator< bidirectional_iterator_tag,
                            Value,
                            difference_type,
                            Value *,
                            Value & >
  {
  public:

    inline iterator( void )
      : table( 0 ), hash( 0 ), node( 0 ) {} ;
    
    inline iterator( const iterator & from )
      : table( from.table ), hash( from.hash ), node( from.node ) {} ;

    inline iterator &	    operator ++ ( void ) {
      if( table ) table->next( hash, node );
      return( *this );
    };
    
    inline iterator &	    operator -- ( void ) {
      if( table ) table->prev( hash, node );
      return( *this );
    };

    inline iterator	    operator ++ ( int ) {
      iterator it( *this );
      ++ *this;
      return( it );
    };
    
    inline iterator	    operator -- ( int ) {
      iterator it( *this );
      -- *this;
      return( it );
    };
    
    inline referance	    operator * ( void ) {
      return( table->value( node ) );
    };

    inline bool		operator == ( const iterator & rhs ) const {
      return( table == rhs.table && hash == rhs.hash && node == rhs.node );
    }
    
#if defined( STLUTILS_RELOPS_BROKEN )
    inline bool		operator != ( const iterator & rhs ) const {
      return( ! (*this == rhs) );
    };
#endif
    inline iterator &	operator = ( const iterator & rhs ) {
      table = rhs.table;
      hash = rhs.hash;
      node = rhs.node;
      return( *this );
    };

    
  protected:
    friend class DashTable< Key, Value, KeyOfValue, HashFunct, LessKey >;
    friend class const_iterator;
    
    inline iterator(
      DashTable< Key, Value, KeyOfValue, HashFunct, LessKey > * aTable,
      HashTableBase::HashValue  aHash,
      HashTableBase::Loc	aNode )
      : table( aTable ), hash( aHash ), node( aNode ) {};
    
    DashTable< Key, Value, KeyOfValue, HashFunct, LessKey > *    table;
    HashTableBase::HashValue	hash;
    HashTableBase::Loc		node;
  };

  class const_iterator
    : public std::iterator< bidirectional_iterator_tag,
                            Value,
                            difference_type,
                            const Value *,
                            const Value & >
  {
  public:

    inline const_iterator( void )
      : table( 0 ), hash( 0 ), node( 0 ) {} ;
    
    inline const_iterator( const const_iterator & from )
      : table( from.table ), hash( from.hash ), node( from.node ) {} ;

    inline const_iterator( const self::iterator & from )
      : table( from.table ), hash( from.hash ), node( from.node ) {} ;

    inline const_iterator &	operator ++ ( void ) {
      if( table ) table->next( hash, node );
      return( *this );
    };
    
    inline const_iterator &	operator -- ( void ) {
      if( table ) table->prev( hash, node );
      return( *this );
    };

    inline const_iterator	operator ++ ( int ) {
      const_iterator it( *this );
      ++ *this;
      return( it );
    };
    
    inline const_iterator	operator -- ( int ) {
      const_iterator it( *this );
      -- *this;
      return( it );
    };
    
    inline const_referance	operator * ( void ) const {
      return( table->value( node ) );
      // return( table ? table->value( node ) : Value() );
    };

    inline bool		operator == ( const const_iterator & rhs ) const {
      return( table == rhs.table && hash == rhs.hash && node == rhs.node );
    };
    
#if defined( STLUTILS_RELOPS_BROKEN )
    inline bool		operator != ( const const_iterator & rhs ) const {
      return( ! (*this == rhs) );
    };
#endif
    
    inline bool		operator == ( const self::iterator & rhs ) const {
      return( table == rhs.table && hash == rhs.hash && node == rhs.node );
    };
    
    inline bool		operator != ( const self::iterator & rhs ) const {
      return( ! (*this == rhs) );
    };
    
    inline const_iterator & operator = ( const const_iterator & rhs ) {
      table = rhs.table;
      hash = rhs.hash;
      node = rhs.node;      
      return( *this );
    };

    inline const_iterator & operator = ( const self::iterator & rhs ) {
      table = rhs.table;
      hash = rhs.hash;
      node = rhs.node;      
      return( *this );
    };

    
  protected:
    friend class DashTable< Key, Value, KeyOfValue, HashFunct, LessKey >;
    
    inline const_iterator(
      const DashTable< Key, Value, KeyOfValue, HashFunct, LessKey > * aTable,
      HashTableBase::HashValue  aHash,
      HashTableBase::Loc	aNode )
      : table( aTable ), hash( aHash ), node( aNode ) {};
    
    const DashTable< Key, Value, KeyOfValue, HashFunct, LessKey > *    table;
    HashTableBase::HashValue    hash;
    HashTableBase::Loc		node;
  };

#if defined( STDCXX_PARTIAL_SPECIALIZATION )
  typedef ::reverse_iterator< iterator >	reverse_iterator;
  typedef ::reverse_iterator< const_iterator >	const_reverse_iterator;
#else
  typedef std::reverse_iterator< const_iterator,
    const_iterator::iterator_category,
    const_iterator::value_type,
    const_iterator::reference,
    const_iterator::pointer,
    const_iterator::difference_type > const_reverse_iterator;
  typedef std::reverse_iterator<
    iterator,
    iterator::iterator_category,
    iterator::value_type,
    iterator::reference,
    iterator::pointer,
    iterator::difference_type > reverse_iterator;
#endif
  
    
  typedef pair< iterator, bool >    pair_iterator_bool;


  // * * * Methods * * * 
  
  DashTable( MultiMemOffset *	memMgr,
	     const char *	indexFileName,
	     ios::open_mode	mode = ios::in,
	     bool		create = false,
	     unsigned short	permMask = 02 );
  
  virtual ~DashTable( void ) {};

  inline iterator		find( const Key & key ) {
    HashValue   hash = hashFunct( key );
    Loc		node = HashTableBase::find( hash );
    for( ; node; node = hashNode( node ).next ) {
      if( ! lessKey( key, keyOf( value( node ) ) ) &&
	  ! lessKey( keyOf( value( node ) ), key ) )
	return( iterator( this, hash, node ) );
    }
    return( end() );
  };

  inline const_iterator		find( const Key & key ) const {
    HashValue   hash = hashFunct( key );
    Loc		node = HashTableBase::find( hash );
    for( ; node; node = hashNode( node ).next ) {
      if( ! lessKey( key, keyOf( value( node ) ) ) &&
	  ! lessKey( keyOf( value( node ) ), key ) )
	return( const_iterator( this, hash, node ) );
    }
    return( end() );
  };

  inline iterator	    find( const Key & key, EffDate effDate ) {
    iterator it = find( key );
    if( it != end() ) {
      for( Loc node = it.node; node; node = dashNode( node ).nextEff ) {
	if( dashNode( node ).when <= effDate ) {
	  it.node = node;
	  return( it );
	}
      }
    }
    return( end() );
  };
    
  inline const_iterator	    find( const Key & key, EffDate effDate ) const {
    const_iterator it = find( key );
    if( it != end() ) {
      for( Loc node = it.node; node; node = dashNode( node ).nextEff ) {
	if( dashNode( node ).when <= effDate ) {
	  it.node = node;
	  return( it );
	}
      }
    }
    return( end() );
  };
    

  inline pair_iterator_bool	insert( const Key &	key,
					EffDate	effDate,
					const Value &	rec ) {
    Loc	    node = mgr->allocate( sizeof( DashNode ) );
    if( node ) {
      value( node ) = rec;
      iterator it = find( key );
      if( it != end() ) {
	if( DashTableBase::insert( it.hash, it.node, effDate, node ) )
	  return( pair_iterator_bool( iterator( this, it.hash, node ), true ));
      }	else {
	if( DashTableBase::insert( hashFunct( key ), effDate, node ) )
	  return( pair_iterator_bool( iterator( this, hashFunct( key ), node ),
				      true ) );
      }
    }
    mgr->release( node );
    return( pair_iterator_bool( iterator( this, endHash(), 0 ), false ) );    
  };


  inline bool			erase( const Key & key, EffDate effDate ) {
    iterator it = find( key, effDate );
    if( it != end() )
      return( erase( it ) );
    else
      return( false );
  };

  inline bool		    erase( const iterator & it ) {
    return( DashTableBase::erase( it.hash, it.node ) );
  };

#if defined( FIXME )
  inline bool		    erase( const iterator & first,
				   const iterator & last ) {
    return( false );
  }
#endif
  
  inline const_iterator	    begin( void ) const {
    HashValue    hash = first();
    return( const_iterator( this, hash, hashLoc( hash ) ) );
  };
  
  inline const_iterator	    end( void ) const {
    return( const_iterator( this, endHash(), 0 ) );
  };

  inline iterator	    begin( void ) {
    HashValue    hash = first();
    return( iterator( this, hash, hashLoc( hash ) ) );
  };
  
  inline iterator	    end( void ) {
    return( iterator( this, endHash(), 0 ) );
  };

  inline const_reverse_iterator	rbegin( void ) const {
    return( const_reverse_iterator( end() ) );
  };

  inline const_reverse_iterator rend( void ) const {
    return( const_reverse_iterator( begin() ) );
  };

  inline reverse_iterator	rbegin( void ) {
    return( reverse_iterator( end() ) );
  };

  inline reverse_iterator	rend( void ) {
    return( reverse_iterator( begin() ) );
  };

  static size_type		getNodeSize( void );
  
#if defined( FIXME )
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;

#endif
  
protected:

  friend iterator;
  friend const_iterator;

  inline Loc	next( HashValue & hash, Loc & node ) const {
    return( DashTableBase::next( hash, node ) );
  };

  inline Loc	prev( HashValue & hash, Loc & node ) const {
    return( DashTableBase::prev( hash, node ) );
  };

  
  inline referance	    value( Loc node ) {
    return( ((DashNode *)mgr->address( node ))->value );
  };
  
  inline const_referance    value( Loc node ) const {
    return( ((const DashNode *)(mgr->address( node )))-> value );
  };

  
  KeyOfValue	    keyOf;
  LessKey	    lessKey;
  HashFunct	    hashFunct;
  
private:

  DashTable( const DashTable< Key, Value, KeyOfValue,
	     HashFunct, LessKey > & from );
  
  DashTable< Key, Value, KeyOfValue, HashFunct, LessKey > &
  operator =( const DashTable< Key, Value, KeyOfValue,HashFunct, LessKey > &
	      from );

};

#include <DashTable.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	DashTable	class
//
//  Constructors:
//
//  	DashTable( );
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
//  	    Return the name of this class (i.e. DashTable )
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
//  	operator <<( ostream & dest, const DashTable & src );
//
//	istream &
//	operator >> ( istream & src, DashTable & dest );
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
// Revision 2.6  2000/05/27 14:02:49  houghton
// Port: Sun CC 5.0.
//
// Revision 2.5  1998/10/23 13:18:26  houghton
// Added const_iterator::operator == ( const iterator & rhs ) const;
// Added const_iterator::operator != ( const iterator & rhs ) const;
//
// Revision 2.4  1997/07/22 19:43:10  houghton
// Cleanup.
// Changed: HashTableBase::Hash was renamed to HashValue.
//
// Revision 2.3  1997/07/13 11:04:14  houghton
// Added rbegin() & rend().
// Changed constructor args.
//
// Revision 2.2  1997/06/05 13:41:26  houghton
// Changed for AIX: made iterator & const_iterator friends, added 'next'
//     and 'prev' mehtods, and fixed copy constructor declaration.
//
// Revision 2.1  1997/06/05 11:29:08  houghton
// Initial Version.
//
//
#endif // ! def _DashTable_hh_ 

