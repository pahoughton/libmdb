#ifndef _mdb_MapBatch_hpp_
#define _mdb_MapBatch_hpp_
/* 1997-07-28 (cc) Paul Houghton <paul4hough@gmail.com>

   create and/or accessing a file containing fixed records.
*/

#include <mdb/MapFile.hpp>
#include <clue/DumpInfo.hpp>
#include <iterator>
#include <string>

#include <unistd.h>

namespace mdb {

template <class T>
class MapBatch
{

public:

  typedef T *		iterator;
  typedef const T *	const_iterator;
  typedef T		value_type;
  typedef size_t	size_type;
  typedef ptrdiff_t	difference_type;
  typedef T &		reference;
  typedef const T &	const_reference;

  typedef T		Rec;

  typedef std::reverse_iterator< const_iterator > const_reverse_iterator;
  typedef std::reverse_iterator< iterator > reverse_iterator;

  inline MapBatch( const char *		fileName,
		   std::ios::openmode   mode = std::ios::in );

  inline MapBatch( const char *		fileName,
		   std::ios::openmode   mode,
		   MapFile::MapMask	permMask );

  inline MapBatch( const char *		fileName,
		   std::ios::openmode   mode,
		   bool			create,
		   MapFile::MapMask	permMask );

  virtual ~MapBatch( void );

  inline const_iterator		begin( void ) const {
    return( (const_iterator)map.getBase() );
  };

  inline const_iterator		end( void ) const {
    return( (const_iterator)(map.getBase() + endPos ));
  };

  inline const_reverse_iterator	rbegin( void ) const {
    return( const_reverse_iterator( end() ) );
  };

  inline const_reverse_iterator	rend( void ) const {
    return( const_reverse_iterator( begin() ) );
  };

  inline const_reference    front( void ) const {
    return( *begin() );
  };

  inline const_reference    back( void ) const {
    return( *(end() - 1) );
  };


  inline iterator	    begin( void ) {
    return( (iterator)map.getBase() );
  };

  inline iterator	    end( void ) {
    return( (iterator)(map.getBase() + endPos ));
  };

  inline reverse_iterator   rbegin( void ) {
    return( reverse_iterator( end() ) );
  };

  inline reverse_iterator   rend( void ) {
    return( reverse_iterator( begin() ) );
  };

  inline reference	    front( void ) {
    return( *begin() );
  };

  inline reference	    back( void ) {
    return( *(end() - 1) );
  };

  inline size_type	size( void ) const {
    return( size_type( end() - begin() ) );
  };

  inline size_type	capacity( void ) const {
    return( size_type(((const_iterator)map.getEnd()) - begin()) );
  }

  bool			reserve( size_type n ) {
    if( capacity() < n ) {
      if( ! map.grow( sizeof( value_type ) * (n - capacity()), 0 ) )
	return( false );
    }
    return( true );
  };

  inline bool		append( const_reference rec ) {
    if( endPos + sizeof( value_type ) >= map.getSize() ) {
      if( ! map.grow( sizeof( value_type ), 0 ) )
	return( false );
    }
    iterator tmp( end() );
    *tmp = rec;
    endPos += sizeof( value_type );
    return( true );
  };

  inline iterator	append( void ) {
    if( endPos + sizeof( value_type ) >= map.getSize() ) {
      if( ! map.grow( sizeof( value_type ), 0 ) )
	return( end() );
    }
    iterator tmp( end() );
    endPos += sizeof( value_type );
    return( tmp );
  };

  inline bool		pop( void ) {
    if( endPos >= sizeof( value_type ) )
      {
	endPos -= sizeof( value_type );
	return( true );
      }
    else
      {
	return( false );
      }
  };

  inline const_reference    operator [] ( size_type rec ) const {
    return( *(begin() + rec) );
  };

  inline reference	    operator [] ( size_type rec ) {
    return( *(begin() + rec) );
  };


  inline bool		sync( size_type	beg = 0,
			      size_type	len = MapFile::npos,
			      bool	async = false ) {
    return( map.sync( beg, len, async ) );
  };

  inline const char *	getFileName( void ) const;

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    ") const;

  inline DumpInfo< MapBatch<T> >
  dump( const char * prefix = "    " ) const {
    return( DumpInfo< MapBatch<T> >( *this, prefix ) );
  }


protected:

  MapFile		map;
  MapFile::size_type    endPos;

private:

};


template< class T >
inline
MapBatch<T>::MapBatch(
  const char *		fileName,
  std::ios::openmode    mode
  )
  : map( fileName, 0, mode, false, 02 ),
    endPos( map.getSize() )
{
}

template< class T >
inline
MapBatch<T>::MapBatch(
  const char *		fileName,
  std::ios::openmode    mode,
  MapFile::MapMask	permMask
  )
  : map( fileName,
	 0,
	 mode,
	 false,
	 permMask ),
    endPos( (mode & std::ios::out)
	    && map.getSize() )
{
}

template< class T >
inline
MapBatch<T>::MapBatch(
  const char *		fileName,
  std::ios::openmode    mode,
  bool			create,
  MapFile::MapMask	permMask
  )
  : map( fileName, 0, mode, create, permMask ),
    endPos( create ? 0 : map.getSize() )
{
}

template< class T >
inline
MapBatch<T>::~MapBatch( void )
{
  if( (map.getMode() & std::ios::out ) )
    {
      map.unmap();
      truncate( map.getFileName(), endPos );
    }
}

template< class T >
inline
const char *
MapBatch<T>::getFileName( void ) const
{
  return( map.getFileName() );
}

template< class T >
inline
bool
MapBatch<T>::good( void ) const
{
  return( map.good() );
}

template< class T >
inline
const char *
MapBatch<T>::error( void ) const
{
  return( map.error() );
}

template< class T >
inline
std::ostream &
MapBatch<T>::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! MapBatch<T>::good() )
    dest << prefix << "Error: " << MapBatch<T>::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  std::string pre = prefix;
  pre += "map:";
  map.dumpInfo( dest, pre.c_str() );

  dest << prefix << "endpos:     " << endPos << '\n'
       << prefix << "records:    " << size() << '\n'
    ;

  return( dest );
}


}; // namespace mdb

/* Example:

	  struct Rec {
	    long  a;
	    long  b;
	    char  c[50];
	  };

	  MapBatch<Rec>   batch( "abc.bin" );

	  for( MapBatch<Rec>::iterator them = batch.begin();
	       them != batch.end();
	       ++ them )
	    {
		 ...
	    }
*/

#endif // ! def _clue_MapBatch_hpp_
