#ifndef _AvlTreeBase_hh_
#define _AvlTreeBase_hh_
//
// File:        AvlTreeBase.hh
// Desc:        
//
//  NO PUBLIC INTERFACE
//
//  Base class for Avl Trees. Provides proteced construtor to
//  prevent direct instantiation. 
//
//  DataTypes:
//
//  	NodeType - enum type of node 
//  	Balance  - enum balance of node's sub trees
//  	SubTree  - node subtrees
//
// Notes:
//
//  For more information on the Avl Balanced Tree Algorithims, see:
//    G.M. Adelson-Velskii and E.M. Landis. Doklady Akademia Nauk SSSR, 146,
//    (1962), 263-66; English translation in Soviet Math, 3, 1259-63
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
//  	    	The source in this class is based on an Avl libraray
//  	    	(avl.c) written by: Brad Appleton on 03/01/89
//
// Created:     12/14/94 06:30 
//
// Revision History:
//
// 
// $Log$
// Revision 1.3  1995/11/05 16:32:24  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:31  houghton
// New Style Avl an memory management. Many New Classes
//
//

#include <ClueConfig.hh>

class AvlTreeBase
{

public:

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


#endif // ! def _AvlTreeBase_hh_ 
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
