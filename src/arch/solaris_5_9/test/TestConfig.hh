#ifndef _TestConfig_hh_
#define _TestConfig_hh_
/**
  File:		TestConfig.hh
  Project:	Mdb 
  Desc:

    
  
  Notes:
    
  Author(s):	Paul Houghton <<paul.houghton@mci.com>>
  Created:	07/19/2003 04:15
  
  Revision History: (See ChangeLog for details)
  
    $Author$
    $Date$
    $Name$
    $Revision$
    $State$

  $Id$

**/

#include <MdbConfig.hh>

#if defined( TEST_DATA_DIR )
#error test data dir defined
#endif

#define TEST_DATA_DIR	"../data"

#endif /* ! def _TestConfig_hh_ */
