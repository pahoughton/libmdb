#ifndef _HashTable_hh_
#define _HashTable_hh_
// 1997-05-19 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/HashTableBase.hpp>
#include <clue/DumpInfo.hpp>

#include <iterator>
#include <utility>

namespace mdb {

template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
class HashTable : public HashTableBase
{

public:
  typedef HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > self;

  typedef ptrdiff_t		    difference_type;
  typedef const Value &		    const_referance;
  typedef Value &		    referance;

  struct HashNode : public HashNodeBase
  {
    Value   value;
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
      if( table ) table->nextNode( hash, node );
      return( *this );
    };

    inline iterator &	    operator -- ( void ) {
      if( table ) table->prevNode( hash, node );
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
    friend class HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >;
    friend class const_iterator;

    inline iterator(
      HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > * aTable,
      HashTableBase::HashValue   aHash,
      HashTableBase::Loc    aNode )
      : table( aTable ), hash( aHash ), node( aNode ) {};

    HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > *    table;
    HashTableBase::HashValue	    hash;
    HashTableBase::Loc	    node;
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
      if( table ) table->nextNode( hash, node );
      return( *this );
    };

    inline const_iterator &	operator -- ( void ) {
      if( table ) table->prevNode( hash, node );
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
    };

    inline bool		operator == ( const const_iterator & rhs ) const {
      return( table == rhs.table && hash == rhs.hash && node == rhs.node );
    }

    inline bool		operator != ( const const_iterator & rhs ) const {
      return( ! (*this == rhs) );
    };

    inline bool	operator == ( const typename self::iterator & rhs ) const {
      return( table == rhs.table && hash == rhs.hash && node == rhs.node );
    }

    inline bool	operator != ( const typename self::iterator & rhs ) const {
      return( ! (*this == rhs) );
    }

    inline const_iterator & operator = ( const const_iterator & rhs ) {
      table = rhs.table;
      hash = rhs.hash;
      node = rhs.node;
      return( *this );
    };

    inline const_iterator & operator = (
      const typename self::iterator & rhs ) {

      table = rhs.table;
      hash = rhs.hash;
      node = rhs.node;
      return( *this );
    };


  protected:
    friend class HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >;

    inline const_iterator(
      const HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > * aTable,
      HashTableBase::HashValue   aHash,
      HashTableBase::Loc    aNode )
      : table( aTable ), hash( aHash ), node( aNode ) {};

    const HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > *    table;
    HashTableBase::HashValue	    hash;
    HashTableBase::Loc	    node;
  };

  typedef std::reverse_iterator< iterator >	    reverse_iterator;
  typedef std::reverse_iterator< const_iterator >   const_reverse_iterator;

  typedef std::pair< iterator, bool >    pair_iterator_bool;

  inline HashTable( MultiMemOffset *	memMgr,
		    const char *	indexFileName,
		    std::ios::openmode	mode = std::ios::in,
		    bool		create = false,
		    unsigned short	permMask = 02 );

  virtual ~HashTable( void ) {};

  inline pair_iterator_bool	insert( const Value & rec ) {
    HashValue   hash = hashFunct( keyOf( rec ) );
    Loc		node = findNode( hash, rec );

    if( node )  // prevent dup keys
      return( pair_iterator_bool( iterator( this, hash, node ), false ) );

    node = mgr->allocate( sizeof( HashNode ) );
    if( node ) {
      if( HashTableBase::insert( hash, node ) ) {
	value( node ) = rec;
	return( pair_iterator_bool( iterator( this, hash, node ), true ) );
      }
    }
    return( pair_iterator_bool( end(), false ) );
  };


  inline const_iterator	    find( const Key & key ) const {
    HashValue    hash = hashFunct( key );
    Loc		 node = findNode( hash, key );
    return( node ? const_iterator( this, hash, node ) : end() );
  };

  inline iterator	    find( const Key & key ) {
    HashValue    hash = hashFunct( key );
    Loc		 node = findNode( hash, key );
    return( node ? iterator( this, hash, node ) : end() );
  };


  inline bool		erase( const Key & key ) {
    iterator  it = find( key );
    if( it != end() )
      return( erase( it ) );
    else
      return( false );
  };

  inline bool		erase( const iterator & it ) {
    return( HashTableBase::erase( it.hash, it.node ) );
  };

  inline bool	    erase( const iterator & first, const iterator & last ) {
    return( HashTableBase::erase( first.hash, first.node,
				  last.hash, last.node ) );
  };

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

  inline const Value &	    value( Loc node ) const;
  inline Value &	    value( Loc node );

  static size_type	    getNodeSize( void );

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  inline
  DumpInfo< HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > >
  dump( const char *	prefix = "    " ) const {
    return( DumpInfo<
	    HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > >(
	      *this, prefix ) );
  };

  class TableDumpMethods : public HashTableBase::DumpMethods
  {
  public:

    TableDumpMethods(
      const HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > & me ) :
      self( me ) {};

    virtual std::ostream & dumpKey( std::ostream &  dest,
				    const Key &	    key ) const {
      return( dest );
    };

    virtual std::ostream & dumpValue( std::ostream &	dest,
				      const Value &	value ) const {
      return( dest );
    };

    virtual std::ostream & dumpNode( std::ostream & dest, Loc node ) const {
      dumpKey( dest, self.keyOf( self.value( node ) ) );
      dest << ' ';
      dumpValue( dest, self.value( node ) );
      return( dest );
    };

  private:
    const HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > & self;
  };


  // ostream &	dumpTable( ostream & dest,
  //			   const TableDumpMethods & meth ) const;


protected:

  friend class iterator;
  friend class const_iterator;
  friend class TableDumpMethods;

  inline Loc	nextNode( HashValue & hash, Loc & node  ) const {
    return( next( hash, node ) );
  };

  inline Loc	prevNode( HashValue & hash, Loc & node ) const {
    return( prev( hash, node ) );
  };

  inline Loc	findNode( HashValue & hash, const Key & key ) const {
    Loc	    node = HashTableBase::find( hash );
    for( ; node; node = hashNode( node ).next ) {
      if( ! lessKey( key, keyOf( value( node ) ) ) &&
	  ! lessKey( keyOf( value( node ) ), key ) )
	break;
    }
    return( node );
  };

  HashFunct	hashFunct;
  KeyOfValue	keyOf;
  LessKey	lessKey;

private:

  // these are prevented by HashTableBase
  //
  // HashTable( const HashTable & from );
  // HashTable & operator =( const HashTable & from );


};



template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::HashTable(
  MultiMemOffset *	memMgr,
  const char *		indexFileName,
  std::ios::openmode    mode,
  bool			create,
  unsigned short	permMask
  )
  : HashTableBase( memMgr, indexFileName, mode, create, permMask )
{
}

template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
HashTableBase::size_type
HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::getNodeSize( void )
{
  return( sizeof( HashNode ) );
}

template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
const Value &
HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::value(
  HashTableBase::Loc node
  ) const
{
  return( ((const HashNode *)(mgr->address( node )))->value );
}

template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
Value &
HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::value(
  HashTableBase::Loc node
  )
{
  return( ((HashNode *)(mgr->address( node )))->value );
}

template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
bool
HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::good( void ) const
{
  return( HashTableBase::good() );
}

template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
const char *
HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::error( void ) const
{
  return( HashTableBase::error() );
}


template< class Key,
          class Value,
          class KeyOfValue,
          class HashFunct,
          class LessKey >
inline
std::ostream &
HashTable< Key, Value, KeyOfValue, HashFunct, LessKey >::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{
  if( ! HashTable<Key,Value,KeyOfValue,HashFunct,LessKey>::good() )
    dest << prefix << "Error: "
	 << HashTable<Key,Value,KeyOfValue,HashFunct,LessKey>::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  clue::Str pre;
  pre = prefix;
  pre << "HashTableBase::";

  HashTableBase::dumpInfo( dest, pre );

  return( dest );
}




#if 0
template< class T >
inline
HashTableBase::Loc
HashTable<T>::insert( HashTableBase::Key key, const T & rec )
{
  Loc recLoc = mgr->allocate( sizeof( T ) );

  if( recLoc == ChunkMgr::badLoc )
    return( recLoc );

  *((T *)(mgr->address( recLoc ))) = rec;

  return( HashTableBase::insert( key, recLoc ) );
}

template< class T >
inline
bool
HashTable<T>::erase( HashTableBase::Key key )
{
  return( HashTableBase::erase( key ) );
}

template< class T >
inline
HashTableBase::Loc
HashTable<T>::find( HashTableBase::Key key ) const
{
  return( HashTableBase::find( key ) );
}

template< class T >
inline
const T &
HashTable<T>::get( HashTableBase::Key key ) const
{
  Loc recLoc = find( key );

  if( recLoc != ChunkMgr::badLoc )
    return( getRec( recLoc ) );
  else
    return( noRec );
}

template< class T >
inline
T &
HashTable<T>::get( HashTableBase::Key key )
{
  Loc recLoc = find( key );

  if( recLoc != ChunkMgr::badLoc )
    return( getRec( recLoc ) );
  else
    return( noRec );
}

template< class T >
inline
DumpInfo< HashTable<T> >
HashTable<T>::dump( const char * prefix, bool showVer ) const
{
  return( DumpInfo< HashTable<T> >( *this, prefix, showVer ) );
}

template< class T >
inline
const T &
HashTable<T>::getRec( HashTableBase::Loc loc ) const
{
  return( *((const T *)mgr->address( loc ) ) );
}

template< class T >
inline
T &
HashTable<T>::getRec( HashTableBase::Loc loc )
{
  return( *((T *)mgr->address( loc ) ) );
}

#endif

}; // namespace mdb

#endif // ! def _HashTable_hh_
