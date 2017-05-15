#ifndef _mdb_RBTreeBase_hpp_
#define _mdb_RBTreeBase_hpp_
// 1997-05-07 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MultiMemOffset.hpp>
#include <clue/Str.hpp>

#include <iostream>

namespace mdb {

class RBTreeBase
{

public:

  typedef MultiMemOffset::Loc		Loc;
  typedef MultiMemOffset::size_type	size_type;

  enum Color
  {
    Red,
    Black
  };

  struct RBNodeBase
  {
    Color   color;
    Loc	    parent;
    Loc	    left;
    Loc	    right;
  };

  class ConstNodeBase
  {
  public:

    inline ConstNodeBase( const MultiMemOffset & memMgr, Loc loc );

    inline Loc		loc( void ) const;
    inline Loc		parent( void ) const;
    inline Loc		left( void ) const;
    inline Loc		right( void ) const;
    inline Color	color( void ) const;

    inline void	    next( void );
    inline void	    prev( void );

    inline ConstNodeBase &  operator ++ ( void );
    inline ConstNodeBase &  operator -- ( void );

    inline ConstNodeBase &  operator = ( Loc rhs );

  protected:

    inline const RBNodeBase &	rbNode( void ) const;

    Loc			nodeLoc;

  private:

    const MultiMemOffset &	mgr;

  };

  class NodeBase : public ConstNodeBase
  {
  public:

    inline NodeBase( MultiMemOffset & memMgr, Loc nodeLoc );

    inline Loc &	parent( void );
    inline Loc &	left( void );
    inline Loc &	right( void );
    inline Color &	color( void );

    inline void		set( Loc p, Loc l, Loc r, Color c );

    inline NodeBase &	operator =  ( Loc rhs );

    inline NodeBase &  operator ++ ( void );
    inline NodeBase &  operator -- ( void );

  protected:

    inline RBNodeBase &	rbNode( void );

    MultiMemOffset &		    mgr;

  private:

  };

  struct RBTreeHeader : RBNodeBase
  {
    unsigned long   version;
    unsigned long   count;
  };

  RBTreeBase( MultiMemOffset *	memMgr,
	      unsigned short	treeKey,
	      bool		create );

  virtual ~RBTreeBase( void );

  inline size_type	size( void ) const;
  inline bool		empty( void ) const;

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;



  class DumpMethods
  {
  public:

    virtual std::ostream &	dumpNode( std::ostream & dest,
					  const Loc node ) const {
      return( dest );
    };
  };

  std::ostream &	dumpRBTree( std::ostream & dest,
				    const DumpMethods & meth ) const;

  bool		testNode( std::ostream & dest, const ConstNodeBase & node ) const;
  bool		testTree( std::ostream & dest ) const;
  bool		allTested( void ) const;
  void		resetTested( void ) const;

protected:

  virtual bool	lessKey( Loc one, Loc two ) const = 0;

  Loc	    insertNode( const NodeBase & c, NodeBase & p, NodeBase & n );
  Loc	    insert( Loc nodeLoc );

  bool	    erase( Loc nodeLoc );

  inline RBTreeHeader &		header( void );
  inline const RBTreeHeader &   header( void ) const;

  inline Loc &		    root( void );
  inline Loc		    root( void ) const;

  inline Loc &		    first( void );
  inline Loc		    first( void ) const;

  inline Loc &		    last( void );
  inline Loc		    last( void ) const;

  inline Loc		    next( Loc & node ) const;
  inline Loc		    prev( Loc & node ) const;

  inline NodeBase	    parent( const ConstNodeBase & node );
  inline NodeBase	    parent( Loc loc );
  inline ConstNodeBase	    parent( const ConstNodeBase & node ) const;
  inline ConstNodeBase	    parent( Loc loc ) const;

  inline NodeBase	    left( const ConstNodeBase & node );
  inline NodeBase	    left( Loc loc );
  inline ConstNodeBase	    left( const ConstNodeBase & node ) const;
  inline ConstNodeBase	    left( Loc loc ) const;

  inline NodeBase	    right( const ConstNodeBase & node );
  inline NodeBase	    right( Loc loc );
  inline ConstNodeBase	    right( const ConstNodeBase & node ) const;
  inline ConstNodeBase	    right( Loc loc ) const;

  inline bool		    setNode( Loc loc, Loc p, Loc l, Loc r, Color c );

  inline NodeBase	    minimum( const NodeBase & node );
  inline NodeBase	    maximum( const NodeBase & node );

  void		rotate_left( Loc loc );
  void		rotate_right( Loc loc );

  enum ErrorNum
  {
    E_OK,
    E_VERSION,
    E_BADTREEKEY,
    E_UNDEFINED
  };

  bool	setError( ErrorNum err );

  MultiMemOffset *  mgr;
  Loc		    headerLoc;
  ErrorNum	    errorNum;

  static clue::Str errStr;

private:

  RBTreeBase( const RBTreeBase & from );
  RBTreeBase & operator =( const RBTreeBase & from );

  static const unsigned long rbTreeVersion;

};


// * * * ConstNodeBase * * *

inline
RBTreeBase::ConstNodeBase::ConstNodeBase(
  const MultiMemOffset &  memMgr,
  Loc		    loc
  )
  : nodeLoc( loc ),
    mgr( memMgr )
{
}

inline
RBTreeBase::Loc
RBTreeBase::ConstNodeBase::loc( void ) const
{
  return( nodeLoc );
}

inline
RBTreeBase::Loc
RBTreeBase::ConstNodeBase::parent( void ) const
{
  return( rbNode().parent );
}

inline
RBTreeBase::Loc
RBTreeBase::ConstNodeBase::left( void ) const
{
  return( rbNode().left );
}

inline
RBTreeBase::Loc
RBTreeBase::ConstNodeBase::right( void ) const
{
  return( rbNode().right );
}

inline
RBTreeBase::Color
RBTreeBase::ConstNodeBase::color( void ) const
{
  return( rbNode().color );
}

inline
const RBTreeBase::RBNodeBase &
RBTreeBase::ConstNodeBase::rbNode( void ) const
{
  return( *((const RBNodeBase *)(mgr.address( nodeLoc ))) );
}

inline
RBTreeBase::ConstNodeBase &
RBTreeBase::ConstNodeBase::operator ++ ( void )
{
  next();
  return( *this );
}

inline
RBTreeBase::ConstNodeBase &
RBTreeBase::ConstNodeBase::operator -- ( void )
{
  prev();
  return( *this );
}

inline
RBTreeBase::ConstNodeBase &
RBTreeBase::ConstNodeBase::operator = ( Loc rhs )
{
  nodeLoc = rhs;
  return( *this );
}

inline
void
RBTreeBase::ConstNodeBase::next( void )
{
  if( right() )
    {
      nodeLoc = right();
      while( left() )
	{
	  nodeLoc = left();
	}
    }
  else
    {
      ConstNodeBase parentNode( mgr, parent() );

      while( nodeLoc == parentNode.right() )
	{
	  nodeLoc = parentNode.loc();
	  parentNode.nodeLoc = parentNode.parent();
	}

      if( right() != parentNode.loc() )
	{
	  nodeLoc = parentNode.loc();
	}
    }
}

inline
void
RBTreeBase::ConstNodeBase::prev( void )
{
  ConstNodeBase parentNode( mgr, parent() );

  if( color() == Red && parentNode.parent() == nodeLoc )
    {
      nodeLoc = right();
    }
  else
    {
      if( left() )
	{
	  for( nodeLoc = left(); right() ; nodeLoc = right() );
	}
      else
	{
	  while( nodeLoc == parentNode.left() )
	    {
	      nodeLoc = parentNode.loc();
	      parentNode.nodeLoc = parentNode.parent();
	    }
	  nodeLoc = parentNode.loc();
	}
    }
}



// * * * NodeBase * * *

inline
RBTreeBase::NodeBase::NodeBase( MultiMemOffset & memMgr, Loc nLoc )
  : RBTreeBase::ConstNodeBase( memMgr, nLoc ),
    mgr( memMgr )
{
}

inline
RBTreeBase::Loc &
RBTreeBase::NodeBase::parent( void )
{
  return( rbNode().parent );
}

inline
RBTreeBase::Loc &
RBTreeBase::NodeBase::left( void )
{
  return( rbNode().left );
}

inline
RBTreeBase::Loc &
RBTreeBase::NodeBase::right( void )
{
  return( rbNode().right );
}

inline
RBTreeBase::Color &
RBTreeBase::NodeBase::color( void )
{
  return( rbNode().color );
}

inline
void
RBTreeBase::NodeBase::set( Loc p, Loc l, Loc r, Color c )
{
  parent() = p;
  left() = l;
  right() = r;
  color() = c;
}

inline
RBTreeBase::NodeBase &
RBTreeBase::NodeBase::operator = ( Loc rhs )
{
  nodeLoc = rhs;
  return( *this );
}

inline
RBTreeBase::NodeBase &
RBTreeBase::NodeBase::operator ++ ( void )
{
  next();
  return( *this );
}

inline
RBTreeBase::NodeBase &
RBTreeBase::NodeBase::operator -- ( void )
{
  prev();
  return( *this );
}

inline
RBTreeBase::RBNodeBase &
RBTreeBase::NodeBase::rbNode( void )
{
  return( *((RBNodeBase *)(mgr.address( nodeLoc ))) );
}

// * * * RBTreeBase * * *

inline
RBTreeBase::size_type
RBTreeBase::size( void ) const
{
  return( header().count );
}

inline
bool
RBTreeBase::empty( void ) const
{
  return( size() == 0 );
}

inline
RBTreeBase::RBTreeHeader &
RBTreeBase::header( void )
{
  return( *((RBTreeHeader *)(mgr->address( headerLoc )) ) );
}

inline
const RBTreeBase::RBTreeHeader &
RBTreeBase::header( void ) const
{
  return( *((const RBTreeHeader *)(mgr->address( headerLoc )) ) );
}

inline
RBTreeBase::Loc &
RBTreeBase::root( void )
{
  return( header().parent );
}

inline
RBTreeBase::Loc
RBTreeBase::root( void ) const
{
  return( header().parent );
}


inline
RBTreeBase::Loc &
RBTreeBase::first( void )
{
  return( header().left );
}

inline
RBTreeBase::Loc
RBTreeBase::first( void ) const
{
  return( header().left );
}

inline
RBTreeBase::Loc &
RBTreeBase::last( void )
{
  return( header().right );
}

inline
RBTreeBase::Loc
RBTreeBase::last( void ) const
{
  return( header().right );
}

inline
RBTreeBase::Loc
RBTreeBase::next( Loc & node ) const
{
  ConstNodeBase n( *mgr, node );
  n.next();
  node = n.loc();
  return( node );
}

inline
RBTreeBase::Loc
RBTreeBase::prev( Loc & node ) const
{
  ConstNodeBase p( *mgr, node );
  p.prev();
  node = p.loc();
  return( node );
}

inline
RBTreeBase::NodeBase
RBTreeBase::parent( const ConstNodeBase & node )
{
  return( NodeBase( *mgr, node.parent() ) );
}

inline
RBTreeBase::NodeBase
RBTreeBase::parent( Loc loc )
{
  return( parent( ConstNodeBase( *mgr, loc ) ) );
}

inline
RBTreeBase::ConstNodeBase
RBTreeBase::parent( const ConstNodeBase & node ) const
{
  return( ConstNodeBase( *mgr, node.parent() ) );
}

inline
RBTreeBase::ConstNodeBase
RBTreeBase::parent( Loc loc ) const
{
  return( parent( ConstNodeBase( *mgr, loc ) ) );
}

inline
RBTreeBase::NodeBase
RBTreeBase::left( const ConstNodeBase & node )
{
  return( NodeBase( *mgr, node.left() ) );
}

inline
RBTreeBase::NodeBase
RBTreeBase::left( Loc loc )
{
  return( left( ConstNodeBase( *mgr, loc ) ) );
}

inline
RBTreeBase::ConstNodeBase
RBTreeBase::left( const ConstNodeBase & node ) const
{
  return( ConstNodeBase( *mgr, node.left() ) );
}

inline
RBTreeBase::ConstNodeBase
RBTreeBase::left( Loc loc ) const
{
  return( left( ConstNodeBase( *mgr, loc ) ) );
}

inline
RBTreeBase::NodeBase
RBTreeBase::right( const ConstNodeBase & node )
{
  return( NodeBase( *mgr, node.right() ) );
}

inline
RBTreeBase::NodeBase
RBTreeBase::right( Loc loc )
{
  return( right( ConstNodeBase( *mgr, loc ) ) );
}

inline
RBTreeBase::ConstNodeBase
RBTreeBase::right( const ConstNodeBase & node ) const
{
  return( ConstNodeBase( *mgr, node.right() ) );
}

inline
RBTreeBase::ConstNodeBase
RBTreeBase::right( Loc loc ) const
{
  return( right( ConstNodeBase( *mgr, loc ) ) );
}

inline
RBTreeBase::NodeBase
RBTreeBase::minimum( const NodeBase & node )
{
  NodeBase m = node;
  while( m.left() )
    m = m.left();
  return( m );
}

inline
RBTreeBase::NodeBase
RBTreeBase::maximum( const NodeBase & node )
{
  NodeBase m = node;
  while( m.right() )
    m = m.right();
  return( m );
}


}; // namespace mdb
#endif // ! def _RBTreeBase_hh_
