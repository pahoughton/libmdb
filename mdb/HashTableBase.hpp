#ifndef _mdb_HashTableBase_hpp_
#define _mdb_HashTableBase_hpp_
// 1997-05-19 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MultiMemOffset.hpp>
#include <mdb/MapFile.hpp>

#include <iostream>

namespace mdb {

class HashTableBase
{

public:

  typedef long	    HashValue;
  typedef off_t	    Loc;
  typedef size_t    size_type;

  HashTableBase( MultiMemOffset *   memMgr,
		 const char *	    indexFileName,
		 std::ios::openmode mode = std::ios::in,
		 bool		    create = false,
		 unsigned short	    permMask = 02 );

  virtual ~HashTableBase( void );

  inline bool		empty( void ) const;
  inline size_type	size( void ) const;

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  static const HashValue  badHash;

  struct Header
  {
    unsigned long   version;
    unsigned long   count;
  };

  struct HashNodeBase
  {
    Loc	    next;
    Loc	    prev;
  };

  class DumpMethods
  {
  public:

    virtual std::ostream &	dumpNode( std::ostream & dest,
					  Loc  node ) const {
      return( dest );
    };
  };

  std::ostream & dumpTable( std::ostream & dest, const DumpMethods & meth ) const;

protected:

  virtual std::ostream &	dumpNode( std::ostream & dest, Loc node ) const;

  enum ErrorNum
  {
    E_OK,
    E_VERSION,
    E_UNDEFINED
  };

  inline Header &	header( void );
  inline const Header &	header( void ) const;

  inline const Loc &	hashLoc( HashValue hash ) const;
  inline Loc &		hashLoc( HashValue hash );

  inline const HashNodeBase &	hashNode( Loc node ) const;
  inline HashNodeBase &		hashNode( Loc node );

  Loc		insert( HashValue hash, Loc node );
  bool		erase( HashValue hash, Loc node );
  bool		erase( HashValue firstHash, Loc firstNode,
		       HashValue lastHash, Loc lastNode );

  inline Loc	find( HashValue hash ) const;

  inline HashValue	first( void ) const;
  inline Loc	next( HashValue & hash, Loc & node  ) const;
  inline Loc	prev( HashValue & hash, Loc & node ) const;
  inline HashValue	endHash( void ) const;

  bool		setError( ErrorNum err );

  MultiMemOffset *  mgr;
  MapFile *	    index;
  ErrorNum	    errorNum;

private:

  HashTableBase( const HashTableBase & from );
  HashTableBase & operator =( const HashTableBase & from );

  static const unsigned long hashTableVersion;

};


inline
HashTableBase::size_type
HashTableBase::size( void ) const
{
  return( good() ? header().count : 0 );
}

inline
bool
HashTableBase::empty( void ) const
{
  return( size() == 0 );
}

inline
HashTableBase::Header &
HashTableBase::header( void )
{
  return( *((Header *)index->getBase()) );
}

inline
const HashTableBase::Header &
HashTableBase::header( void ) const
{
  return( *((const Header *)index->getBase()) );
}


inline
const HashTableBase::Loc &
HashTableBase::hashLoc( HashValue hash ) const
{
  return( *( (const Loc *)( index->getBase() +
			 sizeof( Header ) +
			 ( hash * sizeof( Loc ) ) ) ) );
}


inline
HashTableBase::Loc &
HashTableBase::hashLoc( HashValue hash )
{
  return( *( (Loc *)( index->getBase() +
			 sizeof( Header ) +
			 ( hash * sizeof( Loc ) ) ) ) );
}

inline
const HashTableBase::HashNodeBase &
HashTableBase::hashNode( Loc node ) const
{
  return( *( (const HashNodeBase *)(mgr->address( node )) ) );
}

inline
HashTableBase::HashNodeBase &
HashTableBase::hashNode( Loc node )
{
  return( *( (HashNodeBase *)(mgr->address( node )) ) );
}

inline
HashTableBase::Loc
HashTableBase::find( HashValue hash ) const
{
  if( good() && hash >= 0 && hash < endHash() )
    return( hashLoc( hash ) );
  else
    return( MultiMemOffset::badLoc );
}


inline
HashTableBase::HashValue
HashTableBase::first( void ) const
{
  if( ! good() )
    return( badHash );

  HashValue hash;

  for( hash = 0;
       hash < endHash() && hashLoc(hash) == 0;
       ++ hash );

  return( hash );
}

inline
HashTableBase::Loc
HashTableBase::next( HashValue & hash, Loc & node ) const
{
  if( hashNode( node ).next )
    {
      node = hashNode( node ).next;
    }
  else
    {
      for( ++ hash;
	   hash < endHash() && hashLoc( hash ) == 0;
	   ++ hash );

      if( hash != endHash() )
	node = hashLoc( hash );
      else
	node = 0;
    }


  return( node );
}

inline
HashTableBase::Loc
HashTableBase::prev( HashValue & hash, Loc & node ) const
{
  if( node && hashNode( node ).prev )
    {
      node = hashNode( node ).prev;
    }
  else
    {
      if( hash > 0 )
	-- hash;

      for( ; hash && hashLoc( hash ) == 0; -- hash );

      if( hash <= 0 )
	hash = first();

      for( node = hashLoc( hash );
	   hashNode( node ).next;
	   node = hashNode( node ).next );
    }

  return( node );
}


inline
HashTableBase::HashValue
HashTableBase::endHash( void ) const
{
  if( ! good() )
    return( badHash );

  return( (index->getSize() - sizeof( Header )) / sizeof( Loc ) );
}



}; // namespace mdb

#endif // ! def _mdb_HashTableBase_hpp_
