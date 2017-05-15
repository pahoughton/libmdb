// 1994-11-11 (cc) Paul Houghton <paul4hough@gmail.com>

#include "MapFile.hpp"

#include <clue/Str.hpp>
#include <clue/Clue.hpp>

#include <cstring>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

using namespace clue;

namespace mdb {

const MapFile::size_type  MapFile::npos = NPOS;
MapFile::size_type	    MapFile::totalMapped( 0 );

MapFile::MapFile()
  : mapFd( -1 ),
    mapMode( std::ios::in ),
    mapSize( 0 ),
    mapBase( 0 ),
    refCount( 0 ),
    pageSize( getpagesize() ),
    osErrno( ENOENT )
{
}

MapFile::MapFile(
  const char *	    fileName,
  MapAddr	    baseAddr,
  std::ios::openmode    mode,
  bool		    create,
  size_type	    size,
  MapMask	    permMask
  )
  : fileStat( fileName ),
    mapFd( -1 ),
    mapMode( create ? (std::ios::openmode)( mode | std::ios::out) : mode ),
    mapSize( 0 ),
    mapBase( 0 ),
    refCount( 0 ),
    pageSize( getpagesize() ),
    osErrno( 0 )
{
  if( create )
    createMap( fileName, baseAddr, size, permMask );
  else
    map( fileName, baseAddr, mode );
}

MapFile::MapFile(
  const char * 	    fileName,
  MapAddr    	    baseAddr,
  std::ios::openmode    mode
  )
  : fileStat( fileName ),
    mapFd( -1 ),
    mapMode( mode ),
    mapSize( 0 ),
    mapBase( 0 ),
    refCount( 0 ),
    pageSize( getpagesize() ),
    osErrno( 0 )
{
  map( fileName, baseAddr, mode );
}

MapFile::MapFile(
  const char * 	    fileName,
  size_type	    size,
  MapAddr    	    baseAddr,
  MapMask	    permMask
  )
  : fileStat( fileName ),
    mapFd( -1 ),
    mapMode( (std::ios::openmode)(std::ios::in | std::ios::out) ),
    mapSize( 0 ),
    mapBase( 0 ),
    refCount( 0 ),
    pageSize( getpagesize() ),
    osErrno( 0 )
{
  createMap( fileName, baseAddr, size, permMask );
}


MapFile::~MapFile( void )
{
  unmap();
}


MapFile::size_type
MapFile::createMap(
  const char *	    fileName,
  MapAddr	    baseAddr,
  size_type	    size,
  MapMask	    permMask
  )
{

  if( mapFd >= 0 )
    unmap();

  unsigned short origMask = 0;

  if( permMask != 0777 )
    {
      origMask = umask( permMask );
    }

  unlink( fileName );

  if( (mapFd = open( fileName,O_CREAT | OpenFlags( mapMode ), 0666 ) ) < 0 )
    {
      osErrno = errno;
      mapFd = -1;
      return( 0 );
    }

  fileStat( mapFd, true );

  if( permMask != 0777 )
    {
      umask( origMask );
    }

  return( setSize( size, baseAddr ) );

}

MapFile::size_type
MapFile::map(
  const char *	    fileName,
  MapAddr    	    baseAddr,
  std::ios::openmode    mode
  )
{

  mapBase = 0;
  osErrno = 0;
  mapMode = mode;

  int mapProt = 0;
  int posixMode = 0;

  if( mapMode & std::ios::in )
    {
      if( mapMode & std::ios::out )
	{
	  mapProt = PROT_READ | PROT_WRITE;
	  posixMode = O_RDWR;
	}
      else
	{
	  mapProt = PROT_READ;
	  posixMode = O_RDONLY;
	}
    }
  else
    {
      if( mapMode & std::ios::out )
	{
	  mapProt = PROT_WRITE;
	  posixMode = O_WRONLY;
	}
      else
	{
	  osErrno = EPERM;
	  return(0);
	}
    }

  if( ! fileStat( fileName ).good() )
    {
      osErrno = fileStat.getSysError();
      return( 0 );
    }

  if( (mapFd = open( fileName, posixMode, 0 ) ) < 0 )
    {
      osErrno = errno;
      mapFd = -1;
      return(0);
    }

  mapSize = fileStat.getSize();

  int mapType = MAP_SHARED;

  if( baseAddr != 0 )
    {
      mapType |= MAP_FIXED;
    }

  mapBase = (char *)mmap( baseAddr, mapSize, mapProt, mapType, mapFd, 0 );

  if( mapBase == MAP_FAILED )
    {
      mapSize = 0;
      osErrno = errno;
    }
  else
    {
      totalMapped += mapSize;
    }

  return( mapSize );
}

void
MapFile::unmap( void )
{
  if( mapBase != 0 && mapSize != 0 )
    {
      munmap( mapBase, mapSize );
      totalMapped -= mapSize;
      mapBase = 0;
      mapSize = 0;
    }

  if( mapFd >= 0 )
    {
      close( mapFd );
      mapFd = -1;
    }
}

bool
MapFile::sync(
  size_type	beg,
  size_type	len,
  bool		async
  )
{
  if( mapBase != 0 && mapSize != 0 )
    {
      if( beg >= mapSize )
	{
	  osErrno = EINVAL;
	  return( false );
	}

      // sync must begin and end on page boundries
      size_t	syncStart = RoundDown( beg, (int)pageSize );
      size_t	syncLen = 0;

      if( len == npos || (beg + len) >= mapSize )
	{
	  syncLen = Align( mapSize - syncStart, pageSize );
	}
      else
	{
	  syncLen = Align( syncStart - beg + len, pageSize );

	  if( syncStart + syncLen > mapSize )
	    {
	      syncLen = Align( mapSize - syncStart, pageSize );
	    }
	}

      if( msync( mapBase + syncStart, syncLen, (async ? MS_ASYNC : MS_SYNC ) ) )
	{
	  osErrno = errno;
	  return( false );
	}
      else
	{
	  return( true );
	}
    }
  else
    {
      return( false );
    }
}

#if defined( Sun5 ) && defined( STUPID )
static char sunWriteData[ 8192 * 8 ];
#endif

MapFile::size_type
MapFile::setSize(
  size_type    	size,
  MapAddr    	baseAddr
  )
{

  if( mapFd == 0 )
    return( 0 );


  if( (mapMode & std::ios::out ) == 0 )
    {
      osErrno = EPERM;
      return( 0 );
    }

  if( mapSize != 0 )
    {
      munmap( mapBase, mapSize );
      totalMapped -= mapSize;
    }



  if( size >= mapSize )
    {

#if defined( Sun5 ) && defined( STUPID )

      // THIS IS SLOW AND STUPID, BUT ...
      //
      // SunOS will randomly cause a SIGBUS when I use 'memory'
      // operations that write to a memory mapped file.
      //
      size_type	newSize = Align( size, pageSize );

      // seek to the current size
      if( lseek( mapFd, mapSize, SEEK_SET ) < 0 )
	{
	  osErrno = errno;
	  return( 0 );
	}

      size_type writeSize = newSize - mapSize;

      size_type written;

      while( writeSize > 0 )
	{
	  if( (written = write( mapFd,
				&sunWriteData,
				min( writeSize,
				     (size_type)sizeof( sunWriteData )) ))
	      < 0 )
	    {
	      if( errno != EINTR )
		{
		  osErrno = errno;
		  return( 0 );
		}
	    }
	  else
	    {
	      writeSize -= written;
	    }
	}

      mapSize = newSize;

#else
      mapSize = Align( size, pageSize );

      char	buf = 0;
      if( lseek( mapFd, mapSize - 1, SEEK_SET ) < 0 ||
	  write( mapFd, &buf, 1 ) != 1 )
	{
	  osErrno = errno;
	  return( 0 );
	}
#endif
    }
  else
    {
      mapSize = Align( size, pageSize );

      if( ftruncate( mapFd, mapSize ) != 0)
	{
	  osErrno = errno;
	  return( 0 );
	}
    }

  int mapType = ( MAP_SHARED | ( baseAddr != 0 ? MAP_FIXED : 0 ) );
  int mapProt = ( PROT_READ | ( ( mapMode | std::ios::out ) ? PROT_WRITE : 0 ) );

  mapBase = (char *)mmap( baseAddr, mapSize, mapProt, mapType, mapFd, 0 );

  if( mapBase == MAP_FAILED )
    {
      mapSize = 0;
      osErrno = errno;
    }
  else
    {
      totalMapped += mapSize;
    }

  return( mapSize );
}

const FileStat &
MapFile::getFileStat( void ) const
{
  return( fileStat );
}

const char *
MapFile::getFileName( void ) const
{
  return( fileStat.getName() );
}

const char *
MapFile::getAccess( void  ) const
{
  if( mapMode & std::ios::in )
    {
      if( mapMode & std::ios::out )
	{
	  return( "RW" );
	}
      else
	{
	  return( "R" );
	}
    }
  else
    {
      return( "W" );
    }
}

MapFile::size_type
MapFile::getPageSize( void )
{
  return( getpagesize() );
}


MapFile::size_type
MapFile::getTotalMapped( void )
{
  return( totalMapped );
}


bool
MapFile::good( void ) const
{
  return( osErrno == 0 );
}

const char *
MapFile::error( void ) const
{
  static Str errStr;
  errStr.reset();

  errStr << "MapFile";

  if( osErrno == 0 )
    {
      errStr << ": ok";
    }
  else
    {
      errStr << ": '" << fileStat.getName() << "' - " << strerror( osErrno );
    }
  return( errStr.cstr() );
}

int
MapFile::getErrno( void ) const
{
  return( osErrno );
}


std::ostream &
MapFile::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! MapFile::good() )
    dest << prefix << "Error: " << MapFile::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  dest << prefix << "name:          " << fileStat.getName() << '\n'
       << prefix << "perm:          " << fileStat.getModeString() << '\n'
       << prefix << "owner/group:   " << fileStat.getUserName() << '/'
       <<                                fileStat.getGroupName() << '\n'
       << prefix << "access:        " << getAccess() << '\n'
       << prefix << "map size:      " << getSize() << '\n'
       << prefix << "base addr:     " << (void *)getBase() << '\n'
       << prefix << "end addr:      " << (void *)getEnd() << '\n'
       << prefix << "total mapped:  " << totalMapped << '\n'
       << prefix << "page size:     " << getPageSize() << std::endl
    ;

  return( dest );
}

}; // namespace mdb
