//
// File:        MultiMemOffsetMapDynamic.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for MultiMemOffsetMapDynamic
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/30/97 07:07
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "MultiMemOffsetMapDynamic.hh"
#include "MapMemDynamic.hh"
#include <Str.hh>

#if defined( MDB_DEBUG )
#include "MultiMemOffsetMapDynamic.ii"
#endif

MDB_VERSION(
  MultiMemOffsetMapDynamic,
  "$Id$");


MultiMemOffsetMapDynamic::MultiMemOffsetMapDynamic(
  MapMemDynamic *   mapMemMgr,
  bool		    delMemMgr
  )
  : mem( mapMemMgr ),
    delMem( delMemMgr )
{
}

MultiMemOffsetMapDynamic::~MultiMemOffsetMapDynamic( void )
{
  if( delMem )
    delete mem;
}

MultiMemOffsetMapDynamic::Loc
MultiMemOffsetMapDynamic::allocate( size_type size )
{
  return( mem->allocate( size ) );
}

void
MultiMemOffsetMapDynamic::release( Loc loc )
{
  mem->release( loc );
}

MultiMemOffsetMapDynamic::Addr
MultiMemOffsetMapDynamic::getBase( void )
{
  return( mem->getBase() );
}

const MultiMemOffsetMapDynamic::Addr
MultiMemOffsetMapDynamic::getBase( void ) const
{
  return( mem->getBase() );
}

MultiMemOffsetMapDynamic::KeyValue
MultiMemOffsetMapDynamic::getKey( unsigned short key ) const
{
  return( mem->getKey( key ) );
}

bool
MultiMemOffsetMapDynamic::reserveKey( unsigned short  key )
{
  return( mem->reserveKey( key ) );
}

bool
MultiMemOffsetMapDynamic::setNewKey( 
  unsigned short    key,
  KeyValue	    value
  )
{
  return( mem->setNewKey( key, value ) );
}
	    
bool
MultiMemOffsetMapDynamic::setKey(
  unsigned short    key,
  KeyValue	    value
  )
{
  return( mem->setKey( key, value ) );
}


bool
MultiMemOffsetMapDynamic::good( void ) const
{
  return( mem && mem->good() );
}

bool
MultiMemOffsetMapDynamic::allocCopyStr( Loc & loc, const Str & src )
{
  bool	status( true );
  
  if( src.size() )
    {
      if( (loc = allocate( src.size() + 5 )) != 0 )
	{
	  char * tmp( address( loc ) );
	  
	  src.copy( tmp, Str::npos );
	}
      else
	{
	  status = false;
	}
    }
  else
    {
      loc = 0;
    }
  
  return( status );
}  
const char *
MultiMemOffsetMapDynamic::error( void ) const
{
  static Str errStr;

  errStr = MultiMemOffsetMapDynamic::getClassName();

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! mem )
	{
	  errStr << ": no mem mgr.";
	}
      else
	{
	  if( ! mem->good() )
	    errStr << ": " << mem->error();
	}
  
      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
MultiMemOffsetMapDynamic::getClassName( void ) const
{
  return( "MultiMemOffsetMapDynamic" );
}

const char *
MultiMemOffsetMapDynamic::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
MultiMemOffsetMapDynamic::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << MultiMemOffsetMapDynamic::getClassName() << ":\n"
	 << MultiMemOffsetMapDynamic::getVersion() << '\n';

  if( ! MultiMemOffsetMapDynamic::good() )
    dest << prefix << "Error: " << MultiMemOffsetMapDynamic::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  if( mem )
    {
      Str pre;
      pre = prefix;
      pre << "mem::";
      mem->dumpInfo( dest, pre, false );
    }
  else
    {
      dest << prefix << "mem:      (NONE)" << '\n';
    }
  
  return( dest );
}

// Revision Log:
//
// $Log$
// Revision 4.2  2003/07/19 09:11:13  houghton
// Port to 64 bit.
//
// Revision 4.1  2001/07/27 00:57:44  houghton
// Change Major Version to 4
//
// Revision 2.3  1999/10/28 14:22:21  houghton
// Added allocCopyStr().
//
// Revision 2.2  1997/10/01 14:03:02  houghton
// Chaged so that 'keys' have to be reserved to be set.
// Increased the number of keys from 16 to 32.
// Changed to use portable multi platform types.
//
// Revision 2.1  1997/07/11 17:37:39  houghton
// Initial Version.
//
//
