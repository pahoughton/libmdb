//
// File:        DashTableBase.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for DashTableBase
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/02/97 08:40
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "DashTableBase.hh"
#include <Str.hh>

#if defined( MDB_DEBUG )
#include "DashTableBase.ii"
#endif

MDB_VERSION(
  DashTableBase,
  "$Id$");


DashTableBase::DashTableBase(
  MultiMemOffset *  memMgr,
  const char *	    indexFileName,
  ios::open_mode    mode,
  bool		    create,
  unsigned short    permMask
  )
  : HashTableBase( memMgr, indexFileName, mode, permMask, create )
{
}

DashTableBase::~DashTableBase( void )
{
}

bool
DashTableBase::good( void ) const
{
  return( HashTableBase::good() );
}

const char *
DashTableBase::error( void ) const
{
  static Str errStr;

  errStr = DashTableBase::getClassName();

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! HashTableBase::good() )
	errStr << HashTableBase::error() ;
      
      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
DashTableBase::getClassName( void ) const
{
  return( "DashTableBase" );
}

const char *
DashTableBase::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
DashTableBase::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << DashTableBase::getClassName() << ":\n"
	 << DashTableBase::getVersion() << '\n';

  if( ! DashTableBase::good() )
    dest << prefix << "Error: " << DashTableBase::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << HashTableBase::getClassName() << "::";

  HashTableBase::dumpInfo( dest, pre, false );
    
  return( dest );
}

//
// insert a new node into an existing list of nodes with the same key
//
HashTableBase::Loc
DashTableBase::insert( Hash hash, Loc cur, EffDate effDate, Loc node )
{
  Loc	    prev;
  
  for( ; cur; cur = dashNode( cur ).nextEff )
    {
      // if cur is before the new node
      // insert the new node in front of cur
      if( dashNode( cur ).when < effDate )
	{
	  dashNode( node ).nextEff = cur;
	  dashNode( node ).prevEff = dashNode( cur ).prevEff;

	  if( dashNode( cur ).prevEff )
	    dashNode( dashNode( cur ).prevEff ).next = node;

	  dashNode( cur ).prevEff = node;

	  // if cur was the first node for this key
	  // make the new node the first node for this key
	  if( dashNode( cur ).next || dashNode( cur ).prev )
	    {
	      dashNode( node ).next = dashNode( cur ).next;
	      dashNode( node ).prev = dashNode( cur ).prev;
	      
	      if( dashNode( node ).next )
		dashNode( dashNode( node ).next ).prev = node;

	      if( dashNode( node ).prev )
		dashNode( dashNode( node ).prev ).next = node;
	      
	      dashNode( cur ).next = 0;
	      dashNode( cur ).prev = 0;

	      // if cur was the first node off the hash index
	      // make the new node the first node off the hash index
	      if( hashLoc( hash ) == cur )
		{
		  hashLoc( hash ) = node;
		}	      
	    }
	  else
	    {
	      dashNode( node ).next = 0;
	      dashNode( node ).prev = 0;
	    }
	  
	  break;
	}
		
      prev = cur;
    }
  
  if( ! cur )
    {
      // all current nodes are after the new node
      dashNode( node ).nextEff = 0;
      dashNode( node ).prevEff = prev;
      
      dashNode( prev ).nextEff = node;
      
      dashNode( node ).next = 0;
      dashNode( node ).prev = 0;
    }

  dashNode( node ).when = effDate;

  ++ header().count;
  
  return( node );
}

//
// insert a new node when there is no other nodes with the same key
HashTableBase::Loc
DashTableBase::insert( Hash hash, EffDate effDate, Loc node )
{
  if( HashTableBase::insert( hash, node ) )
    {
      dashNode( node ).when = effDate;
      dashNode( node ).nextEff = 0;
      dashNode( node ).prevEff = 0;
      return( node );
    }
  return( 0 );
}

bool
DashTableBase::erase( Hash hash, Loc node )
{
  if( dashNode( node ).nextEff )
    dashNode( dashNode( node ).nextEff ).prevEff = dashNode( node ).prevEff;

  if( dashNode( node ).prevEff )
    dashNode( dashNode( node ).prevEff ).nextEff = dashNode( node ).nextEff;

  if( dashNode( node ).next || dashNode( node ).prev )
    {
      return( HashTableBase::erase( hash, node ) );
    }
  else
    {
      mgr->release( node );
      -- header().count;
      return( true );
    }
}

// Revision Log:
//
// $Log$
// Revision 2.2  1997/07/13 11:05:33  houghton
// Changed constructor args.
// Cleanup.
//
// Revision 2.1  1997/06/05 11:29:09  houghton
// Initial Version.
//
//
