
#ifndef _RBTree_hh_
#define _RBTree_hh_
//
// File:        RBTree.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/09/97 04:05
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
#include <RBTreeBase.hh>
#include <iostream>
#include <iterator>
#include <pair>

template< class Key, class Value, class KeyOfValue, class LessKey >
class RBTree : public RBTreeBase
{

public:
  typedef ptrdiff_t	    difference_type;
  typedef const Value &	    const_referance;
  typedef Value &	    referance;
  
  struct RBNode : public RBNodeBase
  {
    Value   value;
  };

  class const_iterator;
  class iterator
    : public bidirectional_iterator< Value, difference_type >
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

    inline Value &	operator * ( void ) {
      return( table->value( node ) );
    };
    
    inline bool		operator == ( const iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node );
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
    : public bidirectional_iterator< Value, difference_type >
  {
  public:

    inline const_iterator( void )
      : table( 0 ), node( 0 ) {};

    inline const_iterator( const const_iterator & from )
      : table( from.table ), node( from.node ) {} ;

    inline const_iterator( const iterator & from )
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

    inline const Value &    operator * ( void ) const {
      return( table->value( node ) );
    };
    
    inline bool		    operator == ( const const_iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node );
    };
    
    inline bool		    operator == ( const iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node );
    };
    
    inline const_iterator & operator = ( const const_iterator & rhs ) {
      table = rhs.table;
      node = rhs.node;
      return( *this );
    };

    inline const_iterator & operator = ( const iterator & rhs ) {
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

  typedef reverse_bidirectional_iterator< const_iterator,
    Value, const Value &, difference_type >	const_reverse_iterator;
  typedef reverse_bidirectional_iterator< iterator,
    Value, Value &, difference_type >		reverse_iterator;
    
  typedef pair< iterator, bool >    pair_iterator_bool;
    
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
  
  ostream &	dumpTree( ostream & dest ) const;

  class DumpTreeMethods : public DumpMethods
  {
  public:
    ostream & dumpNode( ostream & dest, const Loc node ) const {
      return( dumpKey( dest, self->keyOf( self->value( node ) ) ) );
    };

    virtual ostream & dumpKey( ostream & dest,
			       const Key & STLUTILS_UNUSED( key ) ) const {
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

#include <RBTree.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	RBTree	class
//
//  Constructors:
//
//  	RBTree( );
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
//  	    Return the name of this class (i.e. RBTree )
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
//  	operator <<( ostream & dest, const RBTree & src );
//
//	istream &
//	operator >> ( istream & src, RBTree & dest );
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
// Revision 2.8  1997/09/17 16:56:09  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.7  1997/08/17 01:39:45  houghton
// Bug-Fix: const_iterator::operator * () changed to const method.
// Added lower_bound
// Added upper_bound
//
// Revision 2.6  1997/07/25 15:59:27  houghton
// Cleanup.
//
// Revision 2.5  1997/07/25 13:48:28  houghton
// Cleanup.
//
// Revision 2.4  1997/07/19 10:29:41  houghton
// Bug-Fix: added include <pair> and <iterator>
//
// Revision 2.3  1997/07/14 10:39:19  houghton
// Port(AIX): iterator could not access the protected next & prev
//     methods. So I wrote nextNode & prevNode wrappers to provide access.
//
// Revision 2.2  1997/07/13 11:31:31  houghton
// Cleanup.
// Major rework.
//
// Revision 2.1  1997/06/05 11:29:13  houghton
// Initial Version.
//
//
#endif // ! def _RBTree_hh_ 

