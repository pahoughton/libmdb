//
// File:        ExRBSet.C
// Project:	Mdb
// Desc:        
//
//  Example source for using the RBSet< class Rec, class LessRec >
//  template class.
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/23/97 04:54
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include <RBSet.hh>

class RBSetExTable
{
public:

  ExTable( const char *	    fileName,
	   ios::open_mode   mode = ios::in,
	   bool		    create );
    

// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/25 13:40:39  houghton
// Initial Version.
//
//
