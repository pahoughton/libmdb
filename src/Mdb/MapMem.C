//
// File:        MapMem.C
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/21/94 10:59 
//
// Revision History:
//
// $Log$
// Revision 1.4  1995/11/05 16:32:33  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:49  houghton
// New Style Avl an memory management. Many New Classes
//
//
static const char * RcsId =
"$Id$";

#include "MapMem.hh"
#include "Str.hh"

#ifdef Linux
#include <fcntl.h>
#endif

const char * MapMem::ErrorStrings[] =
{
  ": Ok",
  ": OS - ",
  ": ",
  ": Bad file",
  ": Bad version",
  ": Bad map type",
  0
};

const char * MapMem::TypeStrings[] =
{
  "MapFixed",
  "MapDynamic",
  0
};


// constructor for opening an existing map
MapMem::MapMem(
  const char *	    fileName,
  MapType   	    type,
  unsigned long	    version,
  ios::open_mode    mode
  )
{
  mapError = E_OK;

  mapFileName = fileName;
  
  int fd = open( fileName, O_RDONLY, 0 );
  
  if( fd == -1 )
    {
      mapError = E_OSERROR;
      osErrno = errno;
      return;
    }

  MapInfo   info;

  errno = 0;
  
  if( read( fd, &info, sizeof( info ) ) != sizeof( info ) )
    {
      if( errno != 0 )
	{
	  mapError = E_OSERROR;
	  osErrno = errno;
	}
      else
	{
	  mapError = E_BADFILE;
	}      
      return;
    }


  close( fd );
  
  if( info.type != type )
    {
      mapError = E_BADTYPE;
      return;      
    }

  if( info.version != version )
    {
      mapError = E_BADVER;
      return;
    }

  map( fileName, (caddr_t)info.base, mode );

  mapInfo = (MapInfo *)MapFile::getBase();
}


const char *
MapMem::error( void ) const
{
  static Str errStr;
  errStr.reset();
  
  errStr << MapMem::getClassName();
  
  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {
      int errCnt = 0;
      
      if( mapError == E_OSERROR )
	{
	  errCnt++;
	  errStr << ": OS Error - " << mapFileName << " - " << strerror( osErrno );
	}
      
      if( mapError > E_MAPFILE && mapError < E_UNDEFINED )
	{
	  errCnt++;
	  errStr << ErrorStrings[ mapError ];
	}
	  
      if( ! MapFile::good() )
	{
	  errCnt++;
	  errStr << ": " << MapFile::error();
	}

      if( ! errCnt )
	{
	  errStr << ": unknown error";
	}
    }
  return( errStr.cstr() );
}


ostream &
MapMem::getStats( ostream & dest ) const
{
  dest << MapMem::getClassName() << ": stats" << '\n'
       << "    Status:   " << error() << '\n'
    ;
  if( mapInfo != 0 )
    {
      dest << "    Map Ver:  " << getVersion() << '\n'
	   << "    Type:     " << getTypeName() << '\n'
	   << "    Map to:   " << getMapToAddr() << '\n'
	   << "    Map Size: " << getMapSize() << endl
	;
    }
  else
    {
      dest << "    No Map info available!" << endl;
    }

  return( MapFile::getStats( dest ) );
}
  
	

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
