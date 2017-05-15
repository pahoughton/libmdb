#ifndef _mdb_MultiMemOffsetMapFixed_hpp_
#define _mdb_MultiMemOffsetMapFixed_hpp_
// 1995-01-28 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MultiMemOffset.hpp>
#include <mdb/MapMemDynamicFixed.hpp>

#include <iostream>

namespace mdb {

class MultiMemOffsetMapFixed : public MultiMemOffset
{

public:

  typedef MapMemDynamic::MapMask	MapMask;

  MultiMemOffsetMapFixed( const char *		fileName,
			  std::ios::openmode    mode = std::ios::in );

  MultiMemOffsetMapFixed( const char *	    fileName,
			  size_type    	    recSize,
			  size_type    	    numRec = 0,
			  MapMask	    permMask = 0777 );

  virtual ~MultiMemOffsetMapFixed( void );

  virtual Loc		allocate( size_type size = 0 );
  virtual void		release( Loc loc );

  virtual Addr		address( Loc offset );
  virtual const Addr	address( Loc offset ) const;

  virtual Loc		location( const Addr addr ) const;

  virtual Addr		getBase( void );
  virtual const Addr    getBase( void ) const;

  MapMemDynamicFixed &     getMap( void );

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

protected:

private:

  MultiMemOffsetMapFixed( const MultiMemOffsetMapFixed & from );
  MultiMemOffsetMapFixed & operator =( const MultiMemOffsetMapFixed & from );

  MapMemDynamicFixed	    mem;

};


inline
MultiMemOffsetMapFixed::Loc
MultiMemOffsetMapFixed::allocate( size_type size )
{
  return( mem.allocate( size ) );
}

inline
void
MultiMemOffsetMapFixed::release( Loc offset )
{
  mem.release( offset );
}

inline
MultiMemOffsetMapFixed::Addr
MultiMemOffsetMapFixed::address( Loc loc )
{
  return( mem.address( loc ) );
}

inline
const MultiMemOffsetMapFixed::Addr
MultiMemOffsetMapFixed::address( Loc loc ) const
{
  return( mem.address( loc ) );
}

inline
MultiMemOffsetMapFixed::Loc
MultiMemOffsetMapFixed::location( const Addr addr ) const
{
  return( mem.location( addr ) );
}

inline
MultiMemOffsetMapFixed::Addr
MultiMemOffsetMapFixed::getBase( void )
{
  return( mem.getBase() );
}

inline
const MultiMemOffsetMapFixed::Addr
MultiMemOffsetMapFixed::getBase( void ) const
{
  return( mem.getBase() );
}

inline
MapMemDynamicFixed &
MultiMemOffsetMapFixed::getMap( void )
{
  return( mem );
}

}; // namespace mdb
#endif // ! def _MultiMemOffsetMapFixed_hh_
