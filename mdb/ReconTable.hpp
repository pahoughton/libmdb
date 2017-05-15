#ifndef _mdb_ReconTable_hpp_
#define _mdb_ReconTable_hpp_
// 2000-07-31 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/ReconTableBase.hpp>
#include <clue/DumpInfo.hpp>
#include <iostream>

namespace mdb {

template< class Rec, class LessKey >
class ReconTable : public ReconTableBase
{

public:

  typedef ReconTableBase::size_type	size_type;
  typedef const Rec *			const_iterator;
  typedef ReconTableBase::RecordNum	RecordNum;
  typedef ReconTableBase::VersionNum	VersionNum;

  inline ReconTable( const FilePath &	tableFn,
		     VersionNum		versionNum,
		     std::ios::openmode	mode = std::ios::in,
		     bool		create = false,
		     MapFile::MapMask	permMask = 02 );

  virtual ~ReconTable( void ) {};

  inline bool	    append( const Rec & rec );

  inline const_iterator	    begin( void ) const;
  inline const_iterator	    end( void ) const;

  inline RecordNum	    rfind( const Rec & key ) const;

  inline bool		    update( RecordNum r, const Rec & src );

  inline const Rec &	    operator [] ( RecordNum r ) const;

protected:

  inline const Rec &    record( RecordNum recNum ) const; // 0 is first rec
  inline Rec &          record( RecordNum recNum );       // ditto

  LessKey	lessKey;

private:

  ReconTable( const ReconTable & from );
  ReconTable & operator =( const ReconTable & from );

};

template< class Rec, class LessKey >
inline
ReconTable< Rec, LessKey >::ReconTable(
  const FilePath &	tableFn,
  VersionNum		versionNum,
  ios::openmode	mode,
  bool			create,
  MapFile::MapMask	permMask
  )
  : ReconTableBase( tableFn,
		    mode,
		    create,
		    permMask,
		    versionNum,
		    sizeof( Rec ) )
{
}

template< class Rec, class LessKey >
inline
bool
ReconTable< Rec, LessKey >::append( const Rec & rec )
{
  return( ReconTableBase::append( &rec ) );
}

template< class Rec, class LessKey >
inline
ReconTable< Rec, LessKey >::const_iterator
ReconTable< Rec, LessKey >::begin( void ) const
{
  return( &(record( 0 )) );
}

template< class Rec, class LessKey >
inline
ReconTable< Rec, LessKey >::const_iterator
ReconTable< Rec, LessKey >::end( void ) const
{
  if( ! good() )
    return( &(record( 0 ) ) );
  else
    return( &(record( Net2Host( header().count ) )) );
}

template< class Rec, class LessKey >
inline
ReconTable< Rec, LessKey >::RecordNum
ReconTable< Rec, LessKey >::rfind( const Rec & key ) const
{
  for( RecordNum r = size() - 1; r >= 0; -- r )
    {
      if( ! lessKey( record( r ), key )
	  && ! lessKey( key, record( r ) ) )
	return( r );
    }
  return( BadRec );
}

template< class Rec, class LessKey >
inline
bool
ReconTable< Rec, LessKey >::update( RecordNum r, const Rec & rec )
{
  return( ReconTableBase::update( r, &rec ) );
}

template< class Rec, class LessKey >
inline
const Rec &
ReconTable< Rec, LessKey >::operator [] ( RecordNum r ) const
{
  return( record( r ) );
}


template< class Rec, class LessKey >
inline
const Rec &
ReconTable< Rec, LessKey >::record( RecordNum recNum ) const
{
  return( *((const Rec *)(map.getBase() + recpos( recNum ) ) ) );
}

template< class Rec, class LessKey >
inline
Rec &
ReconTable< Rec, LessKey >::record( RecordNum recNum )
{
  return( *((Rec *)(map.getBase() + recpos( recNum ) ) ) );
}


}; // namespace mdb
#endif // ! def _ReconTable_hh_
