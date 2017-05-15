#ifndef _DRBTree_hh_
#define _DRBTree_hh_
/* 1997-07-16 (cc) Paul Houghton <paul4hough@gmail.com>

   FIXME

   I am storing the key multible times. This should be avoided!
*/

#include <mdb/RBTreeBase.hpp>

#include <iostream>
#include <iomanip>
#include <iterator>
#include <utility>

namespace mdb {

template< class Key, class Value, class KeyOfValue, class LessKey >
class DRBTree : public RBTreeBase
{

public:

  typedef DRBTree< Key, Value, KeyOfValue, LessKey >	self;

  typedef ptrdiff_t	    difference_type;
  typedef const Value &	    const_referance;
  typedef Value &	    referance;

  typedef time_t	    EffDate;

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
    : public std::iterator< std::bidirectional_iterator_tag,
			    Value,
			    difference_type,
			    Value *,
			    Value & >
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

    inline bool		operator != ( const iterator & rhs ) const {
      return( ! (*this == rhs) );
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
    : public std::iterator< std::bidirectional_iterator_tag,
			    Value,
			    difference_type,
			    const Value *,
			    const Value & >
  {
  public:

    inline const_iterator( void )
      : table( 0 ), node( 0 ), hist( 0 ) {};

    inline const_iterator( const const_iterator & from )
      : table( from.table ), node( from.node ), hist( from.hist ) {} ;

    inline const_iterator( const typename self::iterator & from )
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

    inline bool		operator != ( const const_iterator & rhs ) const {
      return( ! (*this == rhs) );
    };

    inline bool	    operator == ( const typename self::iterator & rhs ) const {
      return( table == rhs.table && node == rhs.node && hist == rhs.hist );
    };

    inline bool	    operator != ( const typename self::iterator & rhs ) const {
      return( ! (*this == rhs) );
    }

    inline const_iterator & operator = ( const const_iterator & rhs ) {
      table = rhs.table;
      node = rhs.node;
      hist = rhs.hist;
      return( *this );
    };

    inline const_iterator & operator = (
      const typename self::iterator & rhs ) {

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

  typedef std::reverse_iterator< iterator >	    reverse_iterator;
  typedef std::reverse_iterator< const_iterator >   const_reverse_iterator;

  typedef std::pair< iterator, bool >    pair_iterator_bool;

  inline DRBTree( MultiMemOffset *  memMgr,
		  unsigned short    treeKey = 0,
		  bool		    create = false )
    : RBTreeBase( memMgr, treeKey, create ),
      histMgr( memMgr ) {};

  inline DRBTree( MultiMemOffset *  memMgr,
		  MultiMemOffset *  histMemMgr,
		  unsigned short    treeKey = 0,
		  bool		    create = false )
    : RBTreeBase( memMgr, treeKey, create ),
      histMgr( histMemMgr ) {};

  virtual ~DRBTree( void ) {};

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
	  return( history( hist ).del ? end() : const_iterator( this, node, hist ) );
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

  inline iterator	    lower_bound( const Key & key ) {
    Loc node = findNode( key );
    if( node != headerLoc )
      {
	Loc hist = drbNode( node ).hist;

	// return the newest (latest eff date) non deleted history
	for( ; hist && history( hist ).del;
	     hist = history( hist ).next );

	if( hist )
	  return( history( hist ).del ? end() :
		  iterator( this, node, hist ) );
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

  inline size_type	histSize( void ) const {
    size_type count( 0 );

    for( const_iterator them = begin();
	 them != end();
	 them.nextkey() )
      {
	for( Loc h = drbNode( them.node ).hist; h ; h = history( h ).next )
	  {
	    ++ count;
	  }
      }
    return( count );
  };


  virtual bool		    good( void ) const {
    return( RBTreeBase::good() && histMgr && histMgr->good() );
  }
  virtual const char *	    error( void ) const {

    static std::string errStr;

    errStr = "DRBTree<Key, Value, KeyOfValue, LessKey>";

    if( good() ) {
      errStr += ": ok";
    } else {
      size_t eSize = errStr.size();

      if( ! RBTreeBase::good() ) {
	errStr += ": ";
	errStr += RBTreeBase::error();
      }

      if( ! histMgr )
	errStr += ": no hist mem mgr.";

      if( histMgr && ! histMgr->good() ) {
	errStr += ": histMgr: ";
	errStr += histMgr->error();
      }

      if( eSize == errStr.size() )
        errStr += ": unknown error";
    }

    return( errStr.c_str() );
  }


  virtual std::ostream &  dumpInfo( std::ostream &  dest = std::cerr,
				    const char *    prefix = "    " ) const {
    if( ! good() )
      dest << prefix << "Error: " << error() << '\n';
    else
      dest << prefix << "Good" << '\n';

    RBTreeBase::dumpInfo( dest, prefix );
    if( histMgr ) {
      std::string pre("histmgr:");
      (*histMgr).dumpInfo( dest, pre.c_str() );
    }
    return( dest );
  };


  std::ostream & dumpNode( std::ostream & dest, const_iterator it ) const {
    dest << "node:      " << it.node << '\n'
	 << "hist:      " << it.hist << '\n'
	 << "next hist: " << (it.hist ? history( it.hist ).next : 0 ) << '\n'
	 << "when       " << (it.hist ? history( it.hist ).when : 0 ) << '\n'
	 << "del:       " << (it.hist ? history( it.hist ).del : 0 ) << '\n'
      ;
    return( dest );
  };

  std::ostream & dumpHist( std::ostream &		dest,
			   const_iterator	it,
			   const char *	prefix = "   ",
			   int		nameWidth = 0,
			   bool		locs = false ) const {
    if( locs )
      dest << prefix
	   << std::setw( nameWidth ) << "node"
	   << ": '" << it.node << "'\n";

    for( Loc h = drbNode( it.node ).hist; h ; h = history( h ).next )
      {
	if( locs )
	  dest << prefix << std::setw( nameWidth ) << "hist"
	       << ": '" << h << "'\n";

	dest << prefix
	     << std::setw( nameWidth ) << "when"
	     << ": '" << DateTime( history( h ).when ) << "' ("
	     << history( h ).when << ")\n"
	     << prefix
	     << std::setw( nameWidth ) << "del"
	     << ": '" << (history( h ).del ? 'Y' : 'N') << "'\n"
	  ;

	if( ! history( h ).del )
	  dest << history( h ).value << '\n';
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
	     hist && history( hist ).del ;
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
	     hist && history( hist ).del;
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




}; // namespace mdb

#endif // ! def _DRBTree_hh_
