//
// File:        ChunkMgr.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for ChunkMgr
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/09/97 04:00
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "ChunkMgr.hh"
#include <Str.hh>

#if defined( MDB_DEBUG )
#include "ChunkMgr.ii"
#endif

MDB_VERSION(
  ChunkMgr,
  "$Id$");

const ChunkMgr::Loc ChunkMgr::badLoc( 0 );

ChunkMgr::ChunkMgr( MapMemDynamicDynamic & mapMemMgr )
  : mapMem( mapMemMgr )
{
}

ChunkMgr::~ChunkMgr( void )
{
}

bool
ChunkMgr::good( void ) const
{
  return( mapMem.good() );
}

const char *
ChunkMgr::error( void ) const
{
  static Str errStr;

  errStr = ChunkMgr::getClassName();

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! mapMem.good() )
	errStr << ": " << mapMem.error();
	  
      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
ChunkMgr::getClassName( void ) const
{
  return( "ChunkMgr" );
}

const char *
ChunkMgr::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
ChunkMgr::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << ChunkMgr::getClassName() << ":\n"
	 << ChunkMgr::getVersion() << '\n';

  if( ! ChunkMgr::good() )
    dest << prefix << "Error: " << ChunkMgr::error() << '\n';
  else
    dest << prefix << "Good" << '\n';


  return( dest );
}

// Revision Log:
//
// $Log$
// Revision 2.1  1997/06/05 11:29:07  houghton
// Initial Version.
//
//
