#ifndef _mdb_DashTableBase_hpp_
#define _mdb_DashTableBase_hpp_
// 1997-06-02 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/HashTableBase.hpp>
#include <iostream>

namespace mdb {

class DashTableBase : public HashTableBase
{

public:

  typedef long		EffDate;

  DashTableBase( MultiMemOffset *   memMgr,
		 const char *	    indexFileName,
		 std::ios::openmode mode = std::ios::in,
		 bool		    create = false,
		 unsigned short	    permMask = 02 );

  virtual ~DashTableBase( void );

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  struct DashNodeBase : public HashNodeBase
  {
    Loc		nextEff;
    Loc		prevEff;
    EffDate	when;
  };

protected:

  inline const DashNodeBase &	dashNode( Loc node ) const;
  inline DashNodeBase &		dashNode( Loc node );

  Loc		insert( HashValue hash, Loc cur, EffDate eff, Loc node );
  Loc		insert( HashValue hash, EffDate eff, Loc node );

  bool		erase( HashValue hash, Loc node );

  inline Loc	next( HashValue & hash, Loc & node  ) const;
  inline Loc	prev( HashValue & hash, Loc & node ) const;

private:

  DashTableBase( const DashTableBase & from );
  DashTableBase & operator =( const DashTableBase & from );

};

inline
const DashTableBase::DashNodeBase &
DashTableBase::dashNode( Loc node ) const
{
  return( *( (const DashNodeBase *)(mgr->address( node )) ) );
}

inline
DashTableBase::DashNodeBase &
DashTableBase::dashNode( Loc node )
{
  return( *( (DashNodeBase *)(mgr->address( node )) ) );
}

inline
HashTableBase::Loc
DashTableBase::next( HashValue & hash, Loc & node ) const
{
  if( dashNode( node ).nextEff )
    {
      node = dashNode( node ).nextEff;
      return( node );
    }
  else
    {
      // rewind to the first node with the same key
      for( Loc prevEff = dashNode( node ).prevEff;
	   prevEff;
	   node = prevEff, prevEff = dashNode( node ).prevEff );

      return( HashTableBase::next( hash, node ) );
    }
}

inline
DashTableBase::Loc
DashTableBase::prev( HashValue & hash, Loc & node ) const
{
  if( dashNode( node ).prevEff )
    {
      node = dashNode( node ).prevEff;
      return( node );
    }
  else
    {
      return( HashTableBase::prev( hash, node ) );
    }
}


}; // namespace mdb
#endif // ! def _mdb_DashTableBase_hpp_
