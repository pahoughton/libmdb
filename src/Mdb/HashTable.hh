#ifndef _HashTable_hh_
#define _HashTable_hh_
//
// File:        HashTable.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@mci.com)
// Created:     05/19/97 04:43
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
#include <HashTableBase.hh>

#include <DumpInfo.hh>

#include <iterator>
#include <utility>

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
    
#if defined( STLUTILS_RELOPS_BROKEN )
    inline bool		operator != ( const const_iterator & rhs ) const {
      return( ! (*this == rhs) );
    };
#endif
    
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

#if defined( STDCXX_PARTIAL_SPECIALIZATION )
  typedef ::reverse_iterator< iterator >	reverse_iterator;
  typedef ::reverse_iterator< const_iterator >	const_reverse_iterator;
#else
  typedef std::reverse_iterator<
    const_iterator,
    typename const_iterator::iterator_category,
    typename const_iterator::value_type,
    typename const_iterator::reference,
    typename const_iterator::pointer,
    typename const_iterator::difference_type > const_reverse_iterator;
  typedef std::reverse_iterator<
    iterator,
    typename iterator::iterator_category,
    typename iterator::value_type,
    typename iterator::reference,
    typename iterator::pointer,
    typename iterator::difference_type > reverse_iterator;
#endif
    
  typedef pair< iterator, bool >    pair_iterator_bool;
  
  inline HashTable( MultiMemOffset *	memMgr,
		    const char *	indexFileName,
		    ios::open_mode	mode = ios::in,
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
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline
  DumpInfo< HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > >
  dump( const char *	prefix = "    ",
	bool		showVer = true ) const {
    return( DumpInfo<
	    HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > >(
	      *this, prefix, showVer ) );
  };

  class TableDumpMethods : public HashTableBase::DumpMethods
  {
  public:

    TableDumpMethods(
      const HashTable< Key, Value, KeyOfValue, HashFunct, LessKey > & me ) :
      self( me ) {};
    
    virtual ostream &	dumpKey( ostream &	dest,
				 const Key &	STLUTILS_UNUSED( key ) ) const {
      return( dest );
    };
  
    virtual ostream &	dumpValue( ostream &	 dest,
				   const Value & STLUTILS_UNUSED( value ) ) const {
      return( dest );
    };

    virtual ostream &	    dumpNode( ostream & dest, Loc node ) const {
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

#include <HashTable.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	HashTable	class
//
//  Constructors:
//
//  	HashTable( );
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
//  	    Return the name of this class (i.e. HashTable )
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
//  	operator <<( ostream & dest, const HashTable & src );
//
//	istream &
//	operator >> ( istream & src, HashTable & dest );
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
// Revision 4.4  2004/04/19 20:26:48  houghton
// Fixed warning messages.
//
// Revision 4.3  2003/08/09 12:43:23  houghton
// Changed ver strings.
//
// Revision 4.2  2002/02/28 15:12:55  houghton
// Port(Forte 6.2) Added typename to eliminate warnings.
//
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.9  2000/05/27 14:02:49  houghton
// Port: Sun CC 5.0.
//
// Revision 2.8  1998/10/23 13:18:43  houghton
// Changed include <pair> to include <utility>.
//
// Revision 2.7  1997/09/17 16:55:57  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.6  1997/07/25 15:58:44  houghton
// Cleanup.
// Reworked insert() and find() methods to use new findNode() method.
//
// Revision 2.5  1997/07/25 13:45:16  houghton
// Changed so that only unique keys could be inserted.
//
// Revision 2.4  1997/07/19 10:20:03  houghton
// Port(Sun5): HashTableBase::Hash was renamed to HashValue becuase
//     'Hash' was conflicting with the 'Hash' template class.
// Bug-Fix: added include <MutliMemOffset.hh>
//
// Revision 2.3  1997/07/14 10:38:40  houghton
// Port(AIX): iterator could not access the protected next & prev
//     methods. So I wrote nextNode & prevNode wrappers to provide access.
//
// Revision 2.2  1997/07/13 11:11:58  houghton
// Changed to use MultiMemOffset.
// Added erase( iterator it ).
// Added erase( iterator first, iterator last ).
// Added rbegin() & rend().
// Added getNodeSize().
// Reworked dumpTree().
//
// Revision 2.1  1997/06/05 11:29:10  houghton
// Initial Version.
//
//
#endif // ! def _HashTable_hh_ 

