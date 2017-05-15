// 2017-04-19 (cc) <paul4hough@gmail.com>

#include <valid/verify.hpp>

#define DOTEST( _tfunct_ )						\
{									\
  valid::verify t = _tfunct_;						\
  if( verbose ) std::cout << t << std::endl;				\
  if( t.passing() ) {							\
    std::cout << t.name()  << ' '					\
	      << t.results().size()					\
	      << " validations passed :)"				\
	      << std::endl;						\
  } else {								\
    std::cout << "ERROR:" << t.name() << " one or more failures!" << std::endl;	\
    return( 1 );							\
  }									\
}

extern valid::verify &  v_MapFile01(void);
extern valid::verify &  v_MapFile02(void);
extern valid::verify &  v_MapFile03(void);
extern valid::verify &  v_MapBatch01(void);
extern valid::verify &  v_MapMemDynamicDynamic01(void);
extern valid::verify &  v_MapMemDynamicDynamic02(void);
extern valid::verify &  v_MapMemDynamicFixed01(void);
extern valid::verify &  v_RBTree01(void);
extern valid::verify &  v_RBTree02(void);
extern valid::verify &  v_RBTree03(void);
extern valid::verify &  v_RBSet(void);
extern valid::verify &  v_DRBTree01(void);
extern valid::verify &  v_DRBTree02(void);
extern valid::verify &  v_DRBTree03(void);
extern valid::verify &  v_DRBTree04(void);
extern valid::verify &  v_DashSet01(void);
extern valid::verify &  v_HashSet01(void);

int main( int argc, const char * argv[] ) {

  bool verbose = ! true;

  DOTEST( v_MapFile01() );
  DOTEST( v_MapFile02() );
  DOTEST( v_MapFile03() );
  DOTEST( v_MapBatch01() );
  DOTEST( v_MapMemDynamicDynamic01() );
  DOTEST( v_MapMemDynamicDynamic02() );
  DOTEST( v_MapMemDynamicFixed01() );
  DOTEST( v_RBTree01() );
  DOTEST( v_RBTree02() );
  DOTEST( v_RBTree03() );
  DOTEST( v_RBSet() );
  DOTEST( v_DRBTree01() );
  DOTEST( v_DRBTree02() );
  DOTEST( v_DRBTree03() );
  DOTEST( v_DRBTree04() );
  DOTEST( v_HashSet01() );
  DOTEST( v_DashSet01() );

  return( 0 );
}
