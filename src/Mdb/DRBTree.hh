#ifndef _DRBTree_hh_
#define _DRBTree_hh_
//
// File:        DRBTree.hh
// Project:	Mdb
// Desc:        
//
//
// NOTE: FIXME
//
//	I am storing the key multible times. This should be avoided!
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 04:19
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
class DRBTree : public RBTreeBase
{

public:

  typedef ptrdiff_t	    difference_type;
  typedef const Value &	    const_referance;
  typedef Value &	    referance;
  
  typedef long		    EffDate;

  struct DRBNode : public RBNodeBase
  {
    Loc	    hist;
  };

  struct DRBHist
  {
    EffDate when;
    Loc	    next;
    long    del;
    Value   value;
  };

  class const_iterator;
  class iterator
    : public bidirectional_iterator< Value, difference_type >
  {
  public:

    inline iterator( void )
      : table( 0 ), node( 0 ), hist( 0 ) {};

    inline iterator( const iterator & from )
      : table( from.table ), node( from.node ), hist( from.hist ) {} ;

    inline iterator &	    operator ++ ( void ) {
      if( table ) table->nextHist( node, hist );
      return( *this );
    };

    inline iterator &	    operator -- ( void ) {
      if( table ) table->prevHist( node, hist );
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
      return( table->history( hist ).value );
    };
    
    inline bool		operator == ( const iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node && hist == rhs.hist );
    };
    
    inline iterator &	operator = ( const iterator & rhs ) {
      table = rhs.table;
      node = rhs.node;
      hist = rhs.hist;
      return( *this );
    };

  protected:

    friend class DRBTree< Key, Value, KeyOfValue, LessKey >;
    friend class const_iterator;

    inline iterator(
      DRBTree< Key, Value, KeyOfValue, LessKey > *  aTable,
      RBTreeBase::Loc				    aNode,
      RBTreeBase::Loc				    aHist )
      : table( aTable ), node( aNode ), hist( aHist ) {} ;

    DRBTree< Key, Value, KeyOfValue, LessKey > *    table;
    RBTreeBase::Loc				    node;
    RBTreeBase::Loc				    hist;
  };

  class const_iterator
    : public bidirectional_iterator< Value, difference_type >
  {
  public:

    inline const_iterator( void )
      : table( 0 ), node( 0 ), hist( 0 ) {};

    inline const_iterator( const const_iterator & from )
      : table( from.table ), node( from.node ), hist( from.hist ) {} ;

    inline const_iterator( const iterator & from )
      : table( from.table ), node( from.node ), hist( from.hist ) {} ;

    inline const_iterator &    operator ++ ( void ) {
      if( table ) table->nextHist( node, hist );
      return( *this );
    };

    inline const_iterator & nextkey( void ) {
      if( table ) table->nextkey( node, hist );
      return( *this );
    };
		    
    inline const_iterator &    operator -- ( void ) {
      if( table ) table->prevHist( node, hist );
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
      return( table->history( hist ).value );
    };
    
    inline bool		    operator == ( const const_iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node && hist == rhs.hist );
    };
    
    inline bool		    operator == ( const iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node && hist == rhs.hist );
    };
    
    inline const_iterator & operator = ( const const_iterator & rhs ) {
      table = rhs.table;
      node = rhs.node;
      hist = rhs.hist;
      return( *this );
    };

    inline const_iterator & operator = ( const iterator & rhs ) {
      table = rhs.table;
      node = rhs.node;
      hist = rhs.hist;
      return( *this );
    };

  protected:

    friend class DRBTree< Key, Value, KeyOfValue, LessKey >;

    inline const_iterator(
      const DRBTree< Key, Value, KeyOfValue, LessKey > *    aTable,
      RBTreeBase::Loc					    aNode,
      RBTreeBase::Loc					    aHist )
      : table( aTable ), node( aNode ), hist( aHist ) {} ;

    const DRBTree< Key, Value, KeyOfValue, LessKey > *	table;
    RBTreeBase::Loc					node;
    RBTreeBase::Loc					hist;
  };

  typedef reverse_bidirectional_iterator< const_iterator,
    Value, const Value &, difference_type >	const_reverse_iterator;
  typedef reverse_bidirectional_iterator< iterator,
    Value, Value &, difference_type >		reverse_iterator;
    
  typedef pair< iterator, bool >    pair_iterator_bool;
  
  inline DRBTree( MultiMemOffset *  memMgr,
		  unsigned short    treeKey = 0,
		  bool		    create = false );

  inline DRBTree( MultiMemOffset *  memMgr,
		  MultiMemOffset *  histMemMgr,
		  unsigned short    treeKey = 0,
		  bool		    create = false );

  virtual ~DRBTree( void );

  inline pair_iterator_bool	insert( const Value & rec, EffDate eff ) {
    Loc node = mgr->allocate( sizeof( DRBNode ) );
    Loc hist = histMgr->allocate( sizeof( DRBHist ) );
    
    if( node && hist )
      {
	history( hist ).value	= rec;
	history( hist ).when	= eff;
	history( hist ).next	= 0;
	history( hist ).del	= 0;
	
	drbNode( node ).hist	= hist;

	Loc insLoc = RBTreeBase::insert( node );
	if( insLoc == node )
	  {
	    return( pair_iterator_bool( iterator( this, node, hist ), true ) );
	  }
	else
	  {
	    mgr->release( node );
	    
	    ++ header().count;
  
	    Loc * h = &(drbNode( insLoc ).hist);
	    for( ; *h && history( *h ).when > eff ;
		 h = &(history( *h ).next) );

	    history( hist ).next = *h;
	    *h = hist;
	    return( pair_iterator_bool( iterator( this, insLoc, hist ),
					true ) );
	  }
      }
    else
      {
	if( node ) mgr->release( node );
	if( hist ) histMgr->release( hist );
	return( pair_iterator_bool( end(), false ) );
      }
  };

  inline const_iterator	    find( const Key & key, EffDate eff ) const {
    Loc node = findNode( key );
    if( node != headerLoc &&
	! lessKeyObj( key, keyOf( nodeValue( node ) ) ) )
      {
	Loc hist = drbNode( node ).hist;
	for( ; hist && history( hist ).when > eff;
	     hist = history( hist ).next );
	
	if( hist )
	  return( const_iterator( this, node, hist ) );
	else
	  return( end() );
      }
    else
      {
	return( end() );
      }
  };

  inline iterator	    find( const Key & key, EffDate eff ) {
    Loc node = findNode( key );
    if( node != headerLoc &&
	! lessKeyObj( key, keyOf( nodeValue( node ) ) ) )
      {
	Loc hist = drbNode( node ).hist;
	for( ; hist && history( hist ).when > eff;
	     hist = history( hist ).next );
	
	if( hist )
	  return( history( hist ).del ? end() : iterator( this, node, hist ) );
	else
	  return( end() );
      }
    else
      {
	return( end() );
      }
  };

  inline const_iterator	    lower_bound( const Key & key ) const {
    Loc node = findNode( key );
    if( node != headerLoc )
      {
	Loc hist = drbNode( node ).hist;

	// return the newest (latest eff date) non deleted history 
	for( ; hist && history( hist ).del;
	     hist = history( hist ).next );
	
	if( hist )
	  return( history( hist ).del ? end() :
		  const_iterator( this, node, hist ) );
	else
	  return( end() );
      }
    else
      {
	return( end() );
      }
  };
  
  inline bool		    erase( const Key & key, EffDate eff ) {
    Loc node = findNode( key );
    if( node != headerLoc &&
	! lessKeyObj( key, keyOf( nodeValue( node ) ) ) )
      return( eraseNode( node, eff ) );
    else
      return( false );
  };
  
  inline bool		    erase( const iterator & pos, EffDate eff ) {
    if( pos.node != headerLoc )
      return( eraseNode( pos.node, eff ) );
    else
      return( false );
  };

  inline bool		    erase( const iterator & first,
				   const iterator & last,
				   EffDate	    eff ) {
    for( Loc node = first.node; node != last.node; next( node ) ) {
      if( ! eraseNode( node, eff ))
	return( false );
    }
    return( true );
  };
    
    
    
  inline bool		    trim( const Key & key, EffDate eff ) {
    Loc node = findNode( key );
    if( node != headerLoc &&
	! lessKeyObj( key, keyOf( nodeValue( node ) ) ) )
      {
	return( trimNode( node, eff ) );
      }
    return( true );
  };
	
  inline bool		trim( EffDate eff ) {
    Loc node = first();
    for(Loc nNode; node != headerLoc; node = nNode )
      {
	nNode = node;
	RBTreeBase::next( nNode );
	if( ! trimNode( node, eff ) )
	  return( false );
      }
    return( true );
  };
    
  inline const_iterator	    begin( void ) const {
    return( const_iterator( this, first(), firstHist( first() ) ) );
  };
  
  inline const_iterator	    end( void ) const {
    return( const_iterator( this, headerLoc, 0 ) );
  };
  
  inline iterator	    begin( void ) {
    return( iterator( this, first(), firstHist( first() ) ) );
  };
  
  inline iterator	    end( void ) {
    return( iterator( this, headerLoc, 0 ) );
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

  inline EffDate	effective( const const_iterator & it ) const {
    if( it.hist )
      return( history( it.hist ).when );
    else
      return( 0 );
  };

  inline bool		effective( const const_iterator & it, EffDate eff ) const {
    Loc node = it.node;
    Loc hist = it.hist;
    
    if( node && hist )
      {

	Loc h = drbNode( node ).hist;

	// look down the chain for the first effective
	// record for this node
	for( ; h && history( h ).when > eff;
	     h = history( h ).next );

	// no record found that had a when less that eff
	if( ! h )
	  return( false );
	
	// ok, now we should have a hist who's when is before eff
	
	if( h == hist )
	  {
	    // we stoped at the same record was the one we where passed.
	    // so, it is the one in effect if it's when is before (or =)
	    // the the effdate and it is not a 'del' rec.
	    
	    return( history( h ).when <= eff && ! history( h ).del );
	  }
	else
	  {
	    // bad news, not the same record, so whe have a rec with
	    // a newer eff date which is still before 'eff' so the 'it'
	    // is not the record in effect for 'eff'
	    return( false );
	  }
      }
    else
      {
	// we go a bad iterator.
	return( false );
      }
  }
	    
    
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
#if defined( FIXME )
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;
#endif

  ostream & dumpNode( ostream & dest, const_iterator it ) const {
    dest << "node:      " << it.node << '\n'
	 << "hist:      " << it.hist << '\n'
	 << "next hist: " << (it.hist ? history( it.hist ).next : 0 ) << '\n'
	 << "when       " << (it.hist ? history( it.hist ).when : 0 ) << '\n'
	 << "del:       " << (it.hist ? history( it.hist ).del : 0 ) << '\n'
      ;
    return( dest );
  };

  ostream & dumpHist( ostream & dest, const_iterator it ) const {
    dest << "node:     " << it.node << '\n';

    for( Loc h = drbNode( it.node ).hist; h ; h = history( h ).next )
      {
	dest << "    hist:   " << h << '\n'
	     << "    when:   " << history( h ).when << '\n'
	     << "    del:    " << history( h ).del << '\n'
	     << '\n'
	  ;
      }
    return( dest );
  };

		      
protected:

  friend iterator;
  friend const_iterator;

  inline const Value &	nodeValue( Loc node ) const {
    return( history( drbNode( node ).hist ).value );
  }

  inline Value &	nodeValue( Loc node ) {
    return( history( drbNode( node ).hist ).value );
  }

  inline const DRBHist &    history( Loc hist ) const {
    return( *((const DRBHist *)histMgr->address( hist ) ) );
  };
  
  inline DRBHist &	    history( Loc hist ) {
    return( *((DRBHist *)histMgr->address( hist ) ) );
  };

  inline const DRBNode &    drbNode( Loc node ) const {
    return( *((const DRBNode *)mgr->address( node ) ) );
  }

  inline DRBNode &	    drbNode( Loc node ) {
    return( *((DRBNode *)mgr->address( node ) ) );
  };
  
  inline Loc	nextkey(  Loc & node, Loc & hist ) const {
    for( next( node ); node != headerLoc; next( node ) )
      {
	for( hist = drbNode( node ).hist;
	     history( hist ).del ;
	     hist = history( hist ).next );

	  if( hist )
	    return( hist );
      }

    hist = 0;
    return( hist );
  };
    
  inline Loc	nextHist( Loc & node, Loc & hist ) const {
    if( history( hist ).next )
      {
	for( hist = history( hist ).next;
	     history( hist ).del;
	     hist = history( hist ).next );
	
	if( hist )
	  return( hist );
      }

    return( nextkey( node, hist ) );
  };

  inline Loc	prevHist( Loc & node, Loc & hist ) const {
    
    if( hist && hist != drbNode( node ).hist )
      {
	Loc pHist = drbNode( node ).hist;
	Loc gHist = pHist;
	
	for( ; history( pHist ).next != hist; pHist = history( pHist ).next )
	  {
	    if( history( pHist ).del == 0 )
	      gHist = pHist;
	  }

	if( history( pHist ).del == 0 )
	  {
	    hist = pHist;
	    return( hist );
	  }
	
	if( history( gHist ).del == 0 )
	  {
	    hist = gHist;
	    return( hist );
	  }
      }

    // stay on first node & good hist.
    if( node == first() )
      return( hist );

    for( prev( node ); ; prev( node ) )
      {
	Loc nHist = drbNode( node ).hist;
	Loc gHist = nHist;
	
	for( ; history( nHist ).next; nHist = history( nHist ).next )
	  {
	    if( history( nHist ).del == 0 )
	      gHist = nHist;
	  }

	if( history( nHist ).del == 0 )
	  {
	    hist = nHist;
	    return( hist );
	  }

	if( history( gHist ).del == 0 )
	  {
	    hist = gHist;
	    return( hist );
	  }

	if( node == first() )
	  break;
      }

    return( hist );
  };

  inline Loc	    firstHist( Loc node ) const {
    Loc hist;
    for( hist = drbNode( node ).hist;
	 hist && history( hist ).del;
	 hist = history( hist ).next );
    return( hist );
  };
  
  inline Loc		findNode( const Key & key ) const {
    Loc	    parent = headerLoc;
    Loc	    node = root();

    while( node )
      {
	if( lessKeyObj( keyOf( nodeValue( node ) ), key ) )
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

  inline bool		eraseNode( Loc node, EffDate eff ) {
    // we allocate hist first so 'h' will not become invalid.
    Loc hist = histMgr->allocate( sizeof( DRBHist ) );
    Loc * h = &(drbNode( node ).hist);
    
    for( ; *h && history( *h ).when > eff ;
	 h = &(history( *h ).next) );
    
    if( *h && history( *h ).when == eff )
      {
	history( *h ).del = 1;
	if( hist )
	  histMgr->release( hist );
      }
    else
      {
	if( hist )
	  {
	    ++ header().count;
	    
	    keyOf( history( hist ).value )  =
	      keyOf( history( drbNode( node ).hist ).value );
	    
	    history( hist ).when	    = eff;
	    history( hist ).next	    = 0;
	    history( hist ).del		    = 1;
	    
	    history( hist ).next = *h;
	    *h = hist;
	  }
	else
	  {
	    return( false );
	  }
      }
    return( true );
  };

  inline bool		trimNode( Loc node, EffDate eff ) {
    
    Loc prevHist    = 0;
    Loc hist	    = drbNode( node ).hist;
    for( ; hist && history( hist ).when >= eff;
	 hist = history( hist ).next )
      {
	if( history( hist ).del == 0 )
	  prevHist = hist;
      }
    
    if( hist )
      {
	Loc trimHist = history( hist ).next;
	Loc nextHist;
	
	if( ! prevHist )
	  {
	    trimHist = hist;
	    RBTreeBase::erase( node );
	  }
	else
	  {
	    trimHist = history( prevHist ).next;
	    history( prevHist ).next = 0;
	  }
	
	for( ; trimHist; trimHist = nextHist )
	  {
	    nextHist = history( trimHist ).next;
	    histMgr->release( trimHist );
	    -- header().count;
	  }
      }
    
    return( true );
  };
  
  inline bool	lessKey( Loc one, Loc two ) const {
    return( lessKeyObj( keyOf( nodeValue( one ) ),
			keyOf( nodeValue( two ) ) ) );
  };

  LessKey	lessKeyObj;
  KeyOfValue	keyOf;
  
  MultiMemOffset *  histMgr;
  
private:

  // DRBTree( const DRBTree & from );
  // DRBTree & operator =( const DRBTree & from );

};

#include <DRBTree.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	DRBTree	class
//
//  Constructors:
//
//  	DRBTree( );
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
//  	    Return the name of this class (i.e. DRBTree )
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
//  	operator <<( ostream & dest, const DRBTree & src );
//
//	istream &
//	operator >> ( istream & src, DRBTree & dest );
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
// Revision 2.11  1998/03/03 20:56:39  houghton
// Bug-Fix: effective() would lie.
//
// Revision 2.10  1997/11/03 13:38:57  houghton
// Bug-Fix: changed to alloc the hist node before getting the address of
//     node's hist.
//
// Revision 2.9  1997/11/03 12:03:21  houghton
// Added a method to check if a record is effective for a specfic date.
//
// Revision 2.8  1997/08/20 14:07:51  houghton
// Chagned lower_bound() args, it did not use the effective date so,
//     it no longer accepts the argument.
//
// Revision 2.7  1997/08/20 10:35:19  houghton
// Bug-Fix: was allocating sizeof( DRBNode ) for hist
//     (changed to sizeof(DRBHist ).
// Changed lower_bound to return the newest non deleted hist.
//
// Revision 2.6  1997/08/17 01:38:06  houghton
// Added lower_bound.
// Added nextkey.
//
// Revision 2.5  1997/07/25 15:57:43  houghton
// Added const_reverse_iterator
// Added reverse_iterator
// Added eraseNode()
// Added erase( const iterator & pos, EffDate eff )
// Added erase( const iterator & first, const iterator & last,  EffDate eff )
// Added rbegin(), rend(), rbegin() const, rend() const.
//
// Revision 2.4  1997/07/25 13:44:06  houghton
// Bug-Fix: number of records was not being updated.
//
// Revision 2.3  1997/07/22 19:42:01  houghton
// Cleanup.
// Bug-Fix: Many.
// Added trim functionallity.
//
// Revision 2.2  1997/07/19 10:17:22  houghton
// Bug-Fix: added include <pair> and <iterator>.
//
// Revision 2.1  1997/07/16 16:37:02  houghton
// Initial Version (work in progress).
//
//
#endif // ! def _DRBTree_hh_ 

