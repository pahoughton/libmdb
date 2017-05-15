#ifndef _mdb_MultiMemOffsetMapDynamic_hpp_
#define _mdb_MultiMemOffsetMapDynamic_hpp_
// 1997-06-30 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MultiMemOffset.hpp>

namespace mdb {

class MultiMemOffsetMapDynamic : public MultiMemOffset
{

public:

  MultiMemOffsetMapDynamic( MapMemDynamic * mapMemMgr,
			    bool	    delMemMgr = false );

  virtual ~MultiMemOffsetMapDynamic( void );

  virtual Loc		allocate( size_type size );
  virtual void		release( Loc loc );

  virtual Addr		getBase( void );
  virtual const Addr	getBase( void ) const;

  virtual bool		reserveKey( unsigned short key );
  virtual bool		setNewKey( unsigned short key, KeyValue value );

  virtual bool		setKey( unsigned short key, KeyValue value );
  virtual KeyValue	getKey( unsigned short key ) const;

  virtual bool		allocCopyStr( MultiMemOffset::Loc & loc,
				      const clue::Str &	    src );

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

protected:

private:

  MultiMemOffsetMapDynamic( const MultiMemOffsetMapDynamic & from );
  MultiMemOffsetMapDynamic & operator =( const MultiMemOffsetMapDynamic & from );

  MapMemDynamic *   mem;
  bool		    delMem;

};


}; // namespace mdb
#endif // ! def _mdb_MultiMemOffsetMapDynamic_hpp_
