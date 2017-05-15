#ifndef _mdb_MultiMemOffset_hpp_
#define _mdb_MultiMemOffset_hpp_
// 1995-01-28 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MapMemDynamic.hpp>
#include <cstddef>

namespace mdb {

class MultiMemOffset
{

public:

  typedef off_t	    Loc;
  typedef size_t    size_type;
  typedef char *    Addr;

  typedef MapMemDynamic::KeyValue	KeyValue;


  MultiMemOffset( void );

  virtual ~MultiMemOffset( void );

  virtual Loc		allocate( size_type size ) = 0;
  virtual void		release( Loc offset ) = 0;

  virtual Addr		getBase( void ) = 0;
  virtual const Addr	getBase( void ) const = 0;

  virtual bool		reserveKey( unsigned short key ) = 0;
  virtual bool		setNewKey( unsigned short key, KeyValue value ) = 0;

  virtual bool		setKey( unsigned short key, KeyValue value ) = 0;
  virtual KeyValue	getKey( unsigned short key ) const = 0;

  inline Addr		address( Loc loc );
  inline const Addr	address( Loc loc ) const;

  inline Loc		location( const void * addr ) const;

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  static const Loc	    badLoc;

protected:

private:

};


inline
MultiMemOffset::Addr
MultiMemOffset::address( Loc loc )
{
  return( getBase() + loc );
}

inline
const MultiMemOffset::Addr
MultiMemOffset::address( Loc loc ) const
{
  return( getBase() + loc );
}

inline
MultiMemOffset::Loc
MultiMemOffset::location( const void * addr ) const
{
  return( ((const char *)addr) - getBase() );
}

}; // namespace mdb

#endif // ! def _MultiMemOffset_hh_
