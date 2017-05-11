//
// File:        TestAvlTreeOffset.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for TestAvlTreeOffset
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/01/97 05:23
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "TestAvlTreeOffset.hh"
#include <MultiMemOffsetMapDynamic.hh>
#include <MapMemDynamicFixed.hh>

#include <Str.hh>

#if defined( MDB_DEBUG )
#include "TestAvlTreeOffset.ii"
#endif

MDB_VERSION(
  TestAvlTreeOffset,
  "$Id$");


TestAvlTreeOffset::Tree::Tree(
  const char *	    fileName,
  ios::open_mode    mode,
  bool		    create
  )
  : AvlTreeOffset< TestAvlTreeOffset::Rec >(
    compKey,
    copyRec,
    new MultiMemOffsetMapDynamic(
      new MapMemDynamicFixed(
	fileName,
	mode,
	create,
	getNodeSize(),
	1,
	02 ),
      true ),
    0,
    create )
{
}

TestAvlTreeOffset::Tree::~Tree( void )
{
  delete getMemMgr();
}

ostream &
TestAvlTreeOffset::Tree::dumpKey(
  ostream &	dest,
  const Loc	nodeLoc
  ) const
{
  dest << getNodeData( nodeLoc ).k;
  return( dest );
}

int
TestAvlTreeOffset::Tree::compKey(
  const Rec &	one,
  const Rec &	two
  )
{
  return( ::compare( one.k, two.k ) );
}

TestAvlTreeOffset::Rec &
TestAvlTreeOffset::Tree::copyRec( Rec & dest, const Rec & src )
{
  dest = src;
  return( dest );
}

TestAvlTreeOffset::TestAvlTreeOffset(
  const char *	    fileName,
  ios::open_mode    mode,
  bool		    create
  )
  : avlTree( fileName, mode, create )
{
}

TestAvlTreeOffset::~TestAvlTreeOffset( void )
{
}

TestAvlTreeOffset::Tree &
TestAvlTreeOffset::tree( void )
{
  return( avlTree );
}

const TestAvlTreeOffset::Tree &
TestAvlTreeOffset::tree( void ) const
{
  return( avlTree );
}

bool
TestAvlTreeOffset::good( void ) const
{
  return( avlTree.good() );
}

const char *
TestAvlTreeOffset::error( void ) const
{
  static Str errStr;

  errStr = TestAvlTreeOffset::getClassName();

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! avlTree.good() )
	errStr << ": " << avlTree.error();
      
      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
TestAvlTreeOffset::getClassName( void ) const
{
  return( "TestAvlTreeOffset" );
}

const char *
TestAvlTreeOffset::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
TestAvlTreeOffset::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << TestAvlTreeOffset::getClassName() << ":\n"
	 << TestAvlTreeOffset::getVersion() << '\n';

  if( ! TestAvlTreeOffset::good() )
    dest << prefix << "Error: " << TestAvlTreeOffset::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << "tree:";
  avlTree.dumpInfo( dest, pre, false );
  
  return( dest );
}

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/07/11 17:38:39  houghton
// Initial Version.
//
//
