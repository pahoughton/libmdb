#ifndef _AvlTreeBase_hh_
#define _AvlTreeBase_hh_
//
// File:        AvlTreeBase.hh
// Project:	Mdb
// Desc:        
//
//  NO PUBLIC INTERFACE
//
//  Base class for Avl Trees. Provides proteced construtor to
//  prevent direct instantiation. 
//
// Author:      Paul Houghton - (paul.houghton@mci.com)
//  	    	The source in this class is based on an Avl libraray
//  	    	(avl.c) written by: Brad Appleton on 03/01/89
//
// Created:     12/14/94 06:30 
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <MdbConfig.hh>

class AvlTreeBase
{

public:

  typedef MDB_TYPE_ADDR	    Addr;
  typedef MDB_TYPE_LOC	    Loc;
  typedef MDB_TYPE_SIZE	    size_type;
  
  enum NodeType
  {
    TREE,
    RIGHT_BRANCH,
    LEFT_BRANCH,
    LEAF,
    NULL_NODE
  };

  enum Balance
  {
    LEFT_HEAVY	 = -1,
    BALANCED	 = 0,
    RIGHT_HEAVY	 = 1
  };

  enum SubTree
  {
    LEFT    = 0,
    RIGHT   = 1
  };

protected:

  AvlTreeBase() {};
  
private:
  
};


//
// Notes:
//
//  For more information on the Avl Balanced Tree Algorithims, see:
//    G.M. Adelson-Velskii and E.M. Landis. Doklady Akademia Nauk SSSR, 146,
//    (1962), 263-66; English translation in Soviet Math, 3, 1259-63
//
// Detail Documentation
//
//  Data Types: 
//
//  	AvlTreeBase	class
//
//	Addr	    an address.
//	Loc	    the type used to refer to an offset.
//	size_type   the type used for sizes.
//	
//  	NodeType    enum type of node 
//  	Balance     enum balance of node's sub trees
//  	SubTree	    node subtrees
//
//  Constructors:
//
//  	AvlTreeBase( void );
//
//  Destructors:
//
//  Public Interface: NONE
//
//  Protected Interface:
//
//  Private Methods:
//
//  Associated Functions:
//
// Example:
//
// See Also:
//
// Files:
//
// Documented Ver:
//
// Tested Ver:
//
// Copyright:
//
//              This software is the sole property of
// 
//                 The Williams Companies, Inc.
//                        1 Williams Center
//                          P.O. Box 2400
//        Copyright (c) 1994 by The Williams Companies, Inc.
// 
//                      All Rights Reserved.  
// 
//
// Revision Log:
//
// $Log$
// Revision 4.2  2003/08/09 12:43:23  houghton
// Changed ver strings.
//
// Revision 4.1  2001/07/27 00:57:42  houghton
// Change Major Version to 4
//
// Revision 2.5  1997/09/17 16:55:52  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.4  1997/07/13 10:56:35  houghton
// Cleanup.
//
// Revision 2.3  1997/06/25 12:49:10  houghton
// Added typedef Addr, Loc & size_type.
//
// Revision 2.2  1997/06/19 11:59:52  houghton
// Changed include StlUtilsConfig to include MdbConfig.
//
// Revision 2.1  1995/11/10 12:42:00  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:24  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:31  houghton
// New Style Avl an memory management. Many New Classes
//
//
#endif // ! def _AvlTreeBase_hh_ 
