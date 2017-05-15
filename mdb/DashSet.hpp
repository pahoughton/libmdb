#ifndef _mdb_DashSet_hpp_
#define _mdb_DashSet_hpp_
/* 1995-01-28 (cc) Paul Houghton <paul4hough@gmail.com>

   A 'DashSet' is a hash table for a data type that includes an
   effecive date for each key value.
*/

#include <mdb/DashTable.hpp>
#include <mdb/MdbIdent.hpp>
#include <iostream>

namespace mdb {

template< class Key,
	  class HashFunct,
	  class LessKey >
class DashSet
{

public:

  typedef DashTable< Key, Key, MdbIdent< Key, Key >, HashFunct, LessKey > Table;

  typedef typename Table::size_type		    size_type;
  typedef typename Table::iterator		    iterator;
  typedef typename Table::const_iterator	    const_iterator;
  typedef typename Table::reverse_iterator	    reverse_iterator;
  typedef typename Table::const_reverse_iterator    const_reverse_iterator;
  typedef typename Table::pair_iterator_bool	    pair_iterator_bool;

  typedef typename Table::EffDate		EffDate;

  inline DashSet( MultiMemOffset *	memMgr,
		  const char *		indexFileName,
		  std::ios::openmode	mode = std::ios::in,
		  bool			create = false,
		  unsigned short	permMask = 0 );

  virtual ~DashSet( void ) {};

  inline pair_iterator_bool	insert( const Key & key, EffDate effDate ) {
    return( table.insert( key, effDate, key ) );
  };

  inline bool			erase( const Key & key, EffDate effDate ) {
    return( table.erase( key, effDate ) );
  };


  inline bool			erase( const iterator & pos ) {
    return( table.erase( pos ) );
  };

#if defined( FIXME )
  inline bool			erase( const iterator & first,
				       const iterator & last ) {
    return( table.erase( first, last ) );
  };
#endif

  inline iterator		find( const Key & key, EffDate when ) {
    return( table.find( key, when ) );
  };

  inline const_iterator		find( const Key & key, EffDate when ) const {
    return( table.find( key, when ) );
  };

  inline iterator	    begin( void ) { return( table.begin() ); };
  inline iterator	    end( void ) { return( table.end() ); };

  inline const_iterator	    begin( void ) const { return( table.begin() ); };
  inline const_iterator	    end( void ) const { return( table.end() ); };

  inline reverse_iterator	rbegin( void ) { return( table.rbegin() ); };
  inline reverse_iterator	rend( void ) { return( table.rend() ); };

  inline const_reverse_iterator	rbegin( void ) const {return(table.rbegin());};
  inline const_reverse_iterator	rend( void ) const {return(table.rend());};

  inline size_type	    size( void ) const { return( table.size() ); };
  inline bool		    empty( void ) const { return( table.empty() ); };

  static size_type	    getNodeSize( void ) {
    return( Table::getNodeSize() );
  };

  inline bool		    good( void ) const;
  inline const char *	    error( void ) const;

protected:

private:

  // DashTable will prevent these
  //
  // DashSet( const DashSet & from );
  // DashSet & operator =( const DashSet & from );

  Table	    table;
};


template< class Key, class HashFunct, class LessKey >
inline
DashSet< Key, HashFunct, LessKey >::DashSet(
  MultiMemOffset *	memMgr,
  const char *		indexFileName,
  std::ios::openmode	mode,
  bool			create,
  unsigned short	permMask
  )
  : table( memMgr, indexFileName, mode, permMask, create )
{
}

template< class Key, class HashFunct, class LessKey >
inline
bool
DashSet< Key, HashFunct, LessKey >::good( void ) const
{
  return( table.good() );
}

template< class Key, class HashFunct, class LessKey >
inline
const char *
DashSet< Key, HashFunct, LessKey >::error( void ) const
{
  return( table.error() );
}


}; // namespace mdb

#endif // ! def _mdb_DashSet_hpp_
