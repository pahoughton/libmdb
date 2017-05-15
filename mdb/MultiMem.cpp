// 1995-01-31 (cc) Paul Houghton <paul4hough@gmail.com>

#include "MultiMem.hpp"

namespace mdb {

MultiMem MultiMemMalloc;

void *
MultiMem::getMem( size_t size )
{
  return( malloc( size ) );
}

void
MultiMem::freeMem( void * buffer )
{
  free( buffer );
}

}; // namespace mdb
