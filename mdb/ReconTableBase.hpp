#ifndef _mdb_ReconTableBase_hpp_
#define _mdb_ReconTableBase_hpp_
// 2000-07-31 (cc) Paul Houghton <paul4hough@gmail.com>

#include <mdb/MapFile.hpp>

#include <clue/FileLock.hpp>
#include <clue/DumpInfo.hpp>
#include <iostream>

namespace mdb {

class ReconTableBase
{

public:

  typedef unsigned int	    VersionNum;
  typedef unsigned int	    RecordNum;
  typedef long		    size_type;

  virtual ~ReconTableBase( void );

  inline RecordNum	size( void ) const;

  virtual bool		    good( void ) const;
  virtual const char *	    error( void ) const;
  virtual std::ostream &    dumpInfo( std::ostream &	dest = std::cerr,
				      const char *	prefix = "    " ) const;

  inline DumpInfo< ReconTableBase >
  dump( const char * prefix = "    " ) const {
    return( DumpInfo< ReconTableBase >( *this, prefix ) );
  }


  static VersionNum	getFileVersion( const char * fn );

   // note: this should be protected, but sun compiler doesn't like it.
  struct Header
  {
    VersionNum	    version;
    unsigned int    recSize;
    unsigned int    mapSize;
    RecordNum	    count;
  };

  static const RecordNum    BadRec;

protected:

  ReconTableBase( const char *	     tableFn,
		  std::ios::openmode mode,
		  bool               create,
		  MapFile::MapMask   permMask,
		  unsigned int	     tableVersion,
		  unsigned int	     recSize );

  bool	    append( const void * rec );
  bool	    update( RecordNum r, const void * rec );

  inline const Header & header( void ) const;
  inline Header &       header( void );
  inline size_type      recpos( RecordNum recNum ) const; // 0 is first rec
  inline void *		recptr( RecordNum recNum ); // ditto

  MapFile	    map;
  clue::FileLock    lock;

  enum ErrorNum
  {
    E_OK,
    E_VERSION,
    E_RECSIZE,
    E_UNDEFINED
  };

  unsigned int	tableVer;
  unsigned int	recSize;

  ErrorNum  errorNum;


private:

  ReconTableBase( const ReconTableBase & from );
  ReconTableBase & operator =( const ReconTableBase & from );

};


inline
ReconTableBase::RecordNum
ReconTableBase::size( void ) const
{
  return( header().count  );
}


inline
const ReconTableBase::Header &
ReconTableBase::header( void ) const
{
  return( *( (const Header *)(map.getBase())));
}

inline
ReconTableBase::Header &
ReconTableBase::header( void )
{
  return( *( (Header *)(map.getBase())));
}

inline
ReconTableBase::size_type
ReconTableBase::recpos( RecordNum recNum ) const
{
  return( sizeof( Header ) + ( header().recSize  * recNum ) );
}

inline
void *
ReconTableBase::recptr( RecordNum recNum )
{
  return( map.getBase() + recpos( recNum ) );
}

}; // namespace mdb

#endif // ! def _mdb_ReconTableBase_hpp_
