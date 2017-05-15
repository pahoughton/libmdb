#ifndef _mdb_DashTable_hpp_
#define _mdb_DashTable_hpp_
// 1997-06-02 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/DashTableBase.hpp>
#include <clue/DumpInfo.hpp>

#include <iostream>
#include <iterator>

namespace mdb {

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
    : public std::iterator< std::bidirectional_iterator_tag,
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

    inline bool		operator != ( const iterator & rhs ) const {
      return( ! (*this == rhs) );
    };

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
    : public std::iterator< std::bidirectional_iterator_tag,
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

    inline const_iterator( const typename self::iterator & from )
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

    inline bool		operator != ( const const_iterator & rhs ) const {
      return( ! (*this == rhs) );
    };

    inline bool		operator == ( const typename self::iterator & rhs ) const {
      return( table == rhs.table && hash == rhs.hash && node == rhs.node );
    };

    inline bool		operator != ( const typename self::iterator & rhs ) const {
      return( ! (*this == rhs) );
    };

    inline const_iterator & operator = ( const const_iterator & rhs ) {
      table = rhs.table;
      hash = rhs.hash;
      node = rhs.node;
      return( *this );
    };

    inline const_iterator & operator = ( const typename self::iterator & rhs ) {
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

  typedef std::reverse_iterator< iterator >	reverse_iterator;
  typedef std::reverse_iterator< const_iterator >	const_reverse_iterator;


  typedef std::pair< iterator, bool >    pair_iterator_bool;


  // * * * Methods * * *

  DashTable( MultiMemOffset *	memMgr,
	     const char *	indexFileName,
	     std::ios::openmode	mode = std::ios::in,
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



template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
DashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::DashTable(
  MultiMemOffset *	memMgr,
  const char *		indexFileName,
  std::ios::openmode	mode,
  bool			create,
  unsigned short	permMask
  )
  : DashTableBase( memMgr, indexFileName, mode, permMask, create )
{
}

template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
HashTableBase::size_type
DashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::getNodeSize( void )
{
  return( sizeof( DashNode ) );
}


}; // namespace mdb

#endif // ! def _mdb_DashTable_hpp_
