//
// File:        MultiMemOffset.C
// Desc:        
//              
//
// Author:      Paul Houghton x2309 - (houghton@shoe.wiltel.com)
// Created:     01/28/95 08:25 
//
// Revision History:
//
// $Log$
// Revision 2.1  1995/11/10 12:42:37  houghton
// Change to Version 2
//
// Revision 1.3  1995/11/05  16:32:36  houghton
// Revised
//
// Revision 1.1  1995/02/13  16:08:51  houghton
// New Style Avl an memory management. Many New Classes
//
//
static const char * RcsId =
"$Id$";

#include "MultiMemOffset.hh"
#include "Str.hh"
#include <cerrno>

MultiMemOffset MultiMemOffsetMalloc;

off_t
MultiMemOffset::getMem( size_t size )
{
  void * mem = malloc( size );
  if( ! mem )
    {
      oserrno = errno;
    }
  
  return( (off_t)mem );
}

void
MultiMemOffset::freeMem( off_t offset )
{
  free( (void *)offset );
}

void *
MultiMemOffset::getAddr( off_t offset )
{
  return( (void *)offset );
}

void *
MultiMemOffset::getBase( void )
{
  return( (void *)0 );
}

const char *
MultiMemOffset::getClassName( void ) const
{
  return( "MultiMemOffset" );
}

bool
MultiMemOffset::good( void ) const
{
  return( oserrno == 0 );
}

const char *
MultiMemOffset::error( void ) const
{
  static Str errStr;
  errStr.reset();

  errStr << MultiMemOffset::getClassName();
  
  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {
      errStr << ": " << strerror( oserrno );
    }
  return( errStr.cstr() );
}


ostream &
MultiMemOffset::getStats( ostream & dest ) const
{
  dest << MultiMemOffset::getClassName() << ": stats" << '\n'
       << "    Status:  " << error() << '\n'
       << "    Using:   malloc" << endl
    ;
  
  return( dest );
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

