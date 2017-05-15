#ifndef _mdb_MdbIdent_hpp_
#define _mdb_MdbIdent_hpp_
// 1997-07-02 (cc) Paul Houghton <paul4hough@gmail.com>

namespace mdb {

template <class T, class U>
struct MdbIdent {
    const U & operator() (const T & x) const { return x; }
    U & operator() (T & x) { return x; }
};

}; // namespace mdb

#endif // ! def _mdb_MdbIdent_hpp_
