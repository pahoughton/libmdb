//
// File:        main.C
// Project:	Mdb
// Desc:        
//
//  Application entry point.
//
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     04/22/98 06:14
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <MapFile.hh>

#include <iostream>


int
main( int argc, char * argv[] )
{

  if( argc < 2 )
    {
      cerr << "need a file name." << endl;
      exit( 1 );
    }

  MapFile::size_type	size( 1024 * 400 * 1024 ); // 400 Meg
  
  MapFile   map( argv[1],
		 0,
		 (ios::open_mode)(ios::in | ios::out),
		 true,
		 size );

  if( ! map.good() )
    {
      cerr << map.error() << endl;
    }
  else
    {
      cerr << "good." << endl;
    }
  
  return( 0 );
}
//
// Revision Log:
//
// $Log$
// Revision 1.1  1998/10/23 13:22:27  houghton
// Initial Version.
//
//

