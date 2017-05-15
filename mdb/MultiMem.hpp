#ifndef _mdb_MultiMem_hpp_
#define _mdb_MultiMem_hpp_
// 1995-01-31 (cc) Paul Houghton <paul4hough@gmail.com>

#include <cstdlib>

namespace mdb {

class MultiMem
{

public:

  MultiMem() { };
  virtual ~MultiMem() { };

  virtual void *    getMem( size_t size );
  virtual void 	    freeMem( void * buffer );

protected:

private:

  MultiMem( const MultiMem & copyFrom );
  MultiMem & operator=( const MultiMem & assignFrom );

};

extern MultiMem MultiMemMalloc;

}; // namespace mdb

#endif // ! def _mdb_MultiMem_hpp_
