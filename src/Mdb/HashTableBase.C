//
// File:        HashTableBase.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for HashTableBase
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/19/97 05:08
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "HashTableBase.hh"
#include <Str.hh>
#include <iomanip>

#if defined( MDB_DEBUG )
#include "HashTableBase.ii"
#endif

MDB_VERSION(
  HashTableBase,
  "$Id$");

const unsigned long HashTableBase::hashTableVersion = 0x4d485402;   // MHT02

const HashTableBase::Hash    HashTableBase::badHash( -1 );

HashTableBase::HashTableBase(
  MultiMemOffset *  memMgr,
  const char *	    indexFileName,
  ios::open_mode    mode,
  bool		    create,
  unsigned short    permMask
  )
  : mgr( memMgr ),
    index( 0 )
{
  if( mgr && mgr->good() )
    {
      if( create )
	{
	  index = new MapFile( indexFileName,
			       MapFile::getPageSize(),
			       0,
			       permMask );
	  
	  if( index && index->good() )
	    {
	      Header * hdr = (Header *)index->getBase();
	      hdr->version = hashTableVersion;
	      hdr->count = 0;
	  
	      for( Loc * l = (Loc *)(index->getBase() + sizeof( Header ));
		   l < (Loc *)index->getEnd();
		   ++ l )
		*l = MultiMemOffset::badLoc;
	      
	      setError( E_OK );
	    }
	  
	}
      else
	{
	  index = new MapFile( indexFileName,
			       0,
			       mode );
	  
	  if( index && index->good() )
	    {
	      Header * hdr = (Header *)index->getBase();
	      if( hdr->version != hashTableVersion )
		setError( E_VERSION );
	      else
		setError( E_OK );
	    }
	}
    }
}

HashTableBase::~HashTableBase( void )
{
}

bool
HashTableBase::good( void ) const
{
  return( errorNum == E_OK && index && index->good() &&
	  mgr && mgr->good() );
}

const char *
HashTableBase::error( void ) const
{
  static Str errStr;

  errStr = HashTableBase::getClassName();

  if( good() )
    {
       errStr += ": ok";
    }
  else
    {
      size_t eSize = errStr.size();

      if( ! index )
	errStr << ": no index";

      if( index && ! index->good() )
	errStr << ": " << index->error();

      if( ! mgr->good() )
	errStr << ": " << mgr->error();

      if( errorNum != E_OK )
	{
	  switch( errorNum )
	    {
	    case E_VERSION:
	      errStr << ": version mismatch: "
		     << ( index && index->getBase() ?
			  ((const Header *)index->getBase())->version :
			  0 )
		     << " expected: " << hashTableVersion
		;
	      break;
	      
	    default:
	      break;
	    }
	}
      
      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
HashTableBase::getClassName( void ) const
{
  return( "HashTableBase" );
}

const char *
HashTableBase::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
HashTableBase::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << HashTableBase::getClassName() << ":\n"
	 << HashTableBase::getVersion() << '\n';

  if( ! HashTableBase::good() )
    dest << prefix << "Error: " << HashTableBase::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << "mgr:";
  mgr->dumpInfo( dest, pre, false );
  
  if( index )
    {
      pre = prefix;
      pre << "index:";
      index->dumpInfo( dest, pre, false );
    }

  if( index && index->getBase() )
    {
      dest << prefix << "version:    "
	   << ((const Header *)index->getBase())->version << '\n'
	   << prefix << "count:      " 
	   << ((const Header *)index->getBase())->count << '\n'
	;
    }
  
  return( dest );
}

ostream &
HashTableBase::dumpTable(
  ostream &		dest,
  const DumpMethods &	meth
  ) const
{
  dest << "hash      node  prev  next\n";
  
  for( Hash hash = first(); hash < endHash(); ++ hash )
    {
      for( Loc node = hashLoc( hash );
	   node != 0;
	   node = hashNode( node ).next )
	{
	  dest << setw(8) << hash
	       << setw(6) << node
	       << setw(6) << hashNode( node ).prev
	       << setw(6) << hashNode( node ).next
	       << ' '
	    ;
	  meth.dumpNode( dest, node ) << '\n';
	}
    }
  return( dest );
}

ostream &
HashTableBase::dumpNode( ostream & dest, Loc CLUE_UNUSED( node ) ) const
{
  return( dest );
}

HashTableBase::Loc
HashTableBase::insert( Hash hash, Loc node )
{
  if( ! good() )
    return( MultiMemOffset::badLoc );

  Loc hLoc = (hash * sizeof( Loc )) + sizeof( Header );

  if( hLoc + sizeof( Loc ) >= index->getSize() )
    {
      size_t oSize = index->getSize();
      
      if( index->setSize( hLoc + sizeof( Loc ), 0 ) < hLoc + sizeof( Loc ) )
	return( MultiMemOffset::badLoc );

      for( Loc * nLoc = (Loc *)(index->getBase() + oSize);
	   nLoc < (Loc *)index->getEnd();
	   ++ nLoc )
	*nLoc = MultiMemOffset::badLoc;

    }

  hashNode( node ).prev = 0;
  hashNode( node ).next = hashLoc( hash );
      
  if( hashNode( node ).next )
    hashNode( hashNode( node ).next ).prev = node;

  hashLoc( hash ) = node;
  
  ++ header().count;
  
  return( node );
}

bool
HashTableBase::erase( Hash hash, Loc node )
{
  if( ! good() )
    return( false );

  if( hashNode( node ).prev )
    hashNode( hashNode( node ).prev ).next = hashNode( node ).next;
  else
    hashLoc( hash ) = hashNode( node ).next;

  if( hashNode( node ).next )
    hashNode( hashNode( node ).next ).prev = hashNode( node ).prev;

  mgr->release( node );
  
  -- header().count;
  return( true );
}

bool
HashTableBase::erase(
  Hash	    firstHash,
  Loc	    firstNode,
  Hash	    lastHash,
  Loc	    lastNode
  )
{
  if( ! good() )
    return( false );

  Hash  curHash  = firstHash;
  Loc	curNode  = firstNode;
  
  Hash	nextHash = firstHash;
  Loc	nextNode = firstNode;

  for( next( nextHash, nextNode );
       curHash != lastHash && curNode != lastNode;
       next( nextHash, nextNode ) )
    {
      erase( curHash, curNode );
      curHash = nextHash;
      curNode = nextNode;
    }

  return( true );
}

  
bool
HashTableBase::setError( ErrorNum err )
{
  errorNum = err;
  return( good() );
}
      
// Revision Log:
//
// $Log$
// Revision 2.2  1997/07/13 11:14:05  houghton
// Changed to use MultiMemOffset.
// Changed dumpTable().
// Added erase( first, last ).
//
// Revision 2.1  1997/06/05 11:29:10  houghton
// Initial Version.
//
//
