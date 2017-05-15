#ifndef _mdb_RBTree_hpp_
#define _mdb_RBTree_hpp_
// 1997-05-09 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/RBTreeBase.hpp>

#include <iostream>
#include <iterator>
#include <utility>

namespace mdb {

template< class Key, class Value, class KeyOfValue, class LessKey >
class RBTree : public RBTreeBase
{

public:
  typedef RBTree< Key, Value, KeyOfValue, LessKey >	self;

  typedef ptrdiff_t	    difference_type;
  typedef const Value &	    const_reference;
  typedef Value &	    reference;

  struct RBNode : public RBNodeBase
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
      : table( 0 ), node( 0 ) {};

    inline iterator( const iterator & from )
      : table( from.table ), node( from.node ) {} ;

    inline iterator &	    operator ++ ( void ) {
      if( table ) table->nextNode( node );
      return( *this );
    };

    inline iterator &	    operator -- ( void ) {
      if( table ) table->prevNode( node );
      return( *this );
    };

    inline iterator 	    operator ++ (int) {
      iterator it( *this );
      ++ *this;
      return( it );
    };

    inline iterator	    operator -- (int) {
      iterator it( *this );
      -- *this;
      return( it );
    };

    inline reference	operator * ( void ) {
      return( table->value( node ) );
    };

    inline bool		operator == ( const iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node );
    };

    inline bool		operator != ( const iterator & rhs ) const {
      return( ! (*this == rhs) );
    };

    inline iterator &	operator = ( const iterator & rhs ) {
      table = rhs.table;
      node = rhs.node;
      return( *this );
    };

  protected:

    friend class RBTree< Key, Value, KeyOfValue, LessKey >;
    friend class const_iterator;

    inline iterator(
      RBTree< Key, Value, KeyOfValue, LessKey > *   aTable,
      RBTreeBase::Loc				    aNode )
      : table( aTable ), node( aNode ) {} ;

    RBTree< Key, Value, KeyOfValue, LessKey > *	table;
    RBTreeBase::Loc				node;
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
      : table( 0 ), node( 0 ) {};

    inline const_iterator( const const_iterator & from )
      : table( from.table ), node( from.node ) {} ;

    inline const_iterator( const typename self::iterator & from )
      : table( from.table ), node( from.node ) {} ;

    inline const_iterator &    operator ++ ( void ) {
      if( table ) table->nextNode( node );
      return( *this );
    };

    inline const_iterator &    operator -- ( void ) {
      if( table ) table->prevNode( node );
      return( *this );
    };

    inline const_iterator    operator ++ (int) {
      const_iterator it( *this );
      ++ *this;
      return( it );
    };

    inline const_iterator    operator -- (int) {
      const_iterator it( *this );
      -- *this;
      return( it );
    };

    // ?? odd compile error
    inline const reference    operator * ( void ) const {
      return( (const reference) table->value( node ) );
    };

    inline bool		    operator == ( const const_iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node );
    };

    inline bool		    operator != ( const const_iterator & rhs ) const {
      return( ! (*this == rhs) );
    };

    inline bool		    operator == ( const typename self::iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node );
    };

    inline bool		    operator != ( const typename self::iterator & rhs ) const {
      return( ! (*this == rhs) );
    };

    inline const_iterator & operator = ( const const_iterator & rhs ) {
      table = rhs.table;
      node = rhs.node;
      return( *this );
    };

    inline const_iterator & operator = ( const typename self::iterator & rhs ) {
      table = rhs.table;
      node = rhs.node;
      return( *this );
    };

  protected:

    friend class RBTree< Key, Value, KeyOfValue, LessKey >;

    inline const_iterator(
      const RBTree< Key, Value, KeyOfValue, LessKey > *   aTable,
      RBTreeBase::Loc				    aNode )
      : table( aTable ), node( aNode ) {} ;

    const RBTree< Key, Value, KeyOfValue, LessKey > *	table;
    RBTreeBase::Loc					node;
  };


  typedef std::reverse_iterator< iterator >	    reverse_iterator;
  typedef std::reverse_iterator< const_iterator >   const_reverse_iterator;

  typedef std::pair< iterator, bool >    pair_iterator_bool;

  inline RBTree( MultiMemOffset *   memMgr,
		 unsigned short	    treeKey = 0,
		 bool		    create = false );

  inline ~RBTree( void );

  inline pair_iterator_bool	insert( const Value & rec ) {
    Loc loc = mgr->allocate( sizeof( RBNode ) );
    if( loc )
      {
	value( loc ) = rec;
	Loc insLoc = RBTreeBase::insert( loc );
	if( insLoc == loc )
	  {
	    return( pair_iterator_bool( iterator( this, loc ), true ) );
	  }
	else
	  {
	    mgr->release( loc );
	    return( pair_iterator_bool( iterator( this, insLoc ), false ) );
	  }
      }
    else
      {
	return( pair_iterator_bool( end(), false ) );
      }
  };

  inline const_iterator	    find( const Key & key ) const {
    const_iterator  found( this, findNode( key ) );
    return( ( found == end() ||
	      lessKeyObj( key, keyOf( *found ) ) ) ?
	    end() : found );
  };

  inline iterator	    find( const Key & key ) {
    iterator	found( this, findNode( key ) );
    return( ( found == end() ||
	      lessKeyObj( key, keyOf( *found ) ) ) ?
	    end() : found );
  };

  inline const_iterator	    lower_bound( const Key & key ) const {
    return( const_iterator( this, findNode( key ) ) );
  };

  inline iterator	    lower_bound( const Key & key ) {
    return( iterator( this, findNode( key ) ) );
  };

  inline const_iterator	    upper_bound( const Key & key ) const {
    return( const_iterator( this, upperBound( key ) ) );
  };

  inline iterator	    upper_bound( const Key & key ) {
    return( iterator( this, upperBound( key ) ) );
  };

  inline bool		    erase( const iterator & pos ) {
    if( RBTreeBase::erase( pos.node ) )
      {
	mgr->release( pos.node );
	return( true );
      }
    return( false );
  };

  inline bool		    erase( const Key & key ) {
    iterator it = find( key );
    if( it != end() )
      return( erase( it ) );
    else
      return( false );
  };

  inline bool		    erase( iterator first, const iterator & last ) {
    while( first != last )
      if( ! erase( first ++ ) )
	return( false );
    return( true );
  };

  inline iterator	    begin( void ) {
    return( iterator( this, first() ) );
  };

  inline iterator	    end( void ) {
    return( iterator( this, headerLoc ) );
  };

  inline const_iterator	    begin( void ) const {
    return( const_iterator( this, first() ) );
  };

  inline const_iterator	    end( void ) const {
    return( const_iterator( this, headerLoc ) );
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

  std::ostream &	    dumpTree( std::ostream & dest ) const;

  class DumpTreeMethods : public DumpMethods
  {
  public:
    std::ostream & dumpNode( std::ostream & dest, const Loc node ) const {
      return( dumpKey( dest, self->keyOf( self->value( node ) ) ) );
    };

    virtual std::ostream & dumpKey( std::ostream & dest,
				    const Key & key ) const {
      return( dest );
    };

  private:
    friend class RBTree< Key, Value, KeyOfValue, LessKey >;

    DumpTreeMethods( const RBTree< Key, Value, KeyOfValue, LessKey > * me )
      : self( me ) {};

    const RBTree< Key, Value, KeyOfValue, LessKey > * self;
  };

protected:

  friend class iterator;
  friend class const_iterator;
  friend class DumpTreeMethods;

  inline Loc		findNode( const Key & key ) const {
    Loc	    parent = headerLoc;
    Loc	    node = root();

    while( node )
      {
	if( lessKeyObj( keyOf( value( node ) ), key ) )
	  {
	    node = right( node ).loc();
	  }
	else
	  {
	    parent = node;
	    node = left( node ).loc();
	  }
      }

    return( parent );
  };

  inline Loc	    upperBound( const Key & key ) const {
    Loc	    parent = headerLoc;
    Loc	    node = root();

    while( node )
      {
	if( lessKeyObj( key, keyOf( value( node ) ) ) )
	  {
	    parent = node;
	    node = left( node ).loc();
	  }
	else
	  {
	    node = right( node ).loc();
	  }
      }

    return( parent );
  };


  inline bool	lessKey( Loc one, Loc two ) const {
    return( lessKeyObj( keyOf( value( one ) ), keyOf( value( two ) ) ) );
  };

  inline Loc		    nextNode( Loc & node ) const {
    return( next( node ) );
  };

  inline Loc		    prevNode( Loc & node ) const {
    return( prev( node ) );
  };


  LessKey	lessKeyObj;
  KeyOfValue	keyOf;

private:

};


template< class Key, class Value, class KeyOfValue, class LessKey >
inline
RBTree< Key, Value, KeyOfValue, LessKey >::RBTree(
  MultiMemOffset *  memMgr,
  unsigned short    treeKey,
  bool		    create
  )
  : RBTreeBase( memMgr, treeKey, create )
{
}

template< class Key, class Value, class KeyOfValue, class LessKey >
inline
RBTree< Key, Value, KeyOfValue, LessKey >::~RBTree( void )
{
}

template< class Key, class Value, class KeyOfValue, class LessKey >
inline
RBTreeBase::size_type
RBTree< Key, Value, KeyOfValue, LessKey >::getNodeSize( void )
{
  return( std::max( sizeof( RBNode ), sizeof( RBTreeHeader ) ) );
}

template< class Key, class Value, class KeyOfValue, class LessKey >
inline
const Value &
RBTree< Key, Value, KeyOfValue, LessKey >::value( RBTreeBase::Loc node ) const
{
  return( ((const RBNode *)(mgr->address( node )))->value );
}

template< class Key, class Value, class KeyOfValue, class LessKey >
inline
Value &
RBTree< Key, Value, KeyOfValue, LessKey >::value(  RBTreeBase::Loc node )
{
  return( ((RBNode *)(mgr->address( node )))->value );
}


template< class Key, class Value, class KeyOfValue, class LessKey >
inline
std::ostream &
RBTree< Key, Value, KeyOfValue, LessKey >::dumpTree( std::ostream & dest ) const
{
  DumpTreeMethods meth( this );

  return( dumpRBTree( dest, meth ) );
}

}; // namespace mdb

#endif // ! def _RBTree_hh_
