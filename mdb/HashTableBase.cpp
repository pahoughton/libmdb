// 1997-05-19 (cc) Paul Houghton <paul4hough@gmail.com>

#include "HashTableBase.hpp"
#include <clue/Str.hpp>

#include <iomanip>

namespace mdb {

const unsigned long HashTableBase::hashTableVersion = 0x4d485402;   // MHT02

const HashTableBase::HashValue    HashTableBase::badHash( -1 );

HashTableBase::HashTableBase(
  MultiMemOffset *	memMgr,
  const char *		indexFileName,
  std::ios::openmode    mode,
  bool			create,
  unsigned short	permMask
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
  if( index ) delete index;
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
  static clue::Str errStr;

  errStr = "HashTableBase";

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

      if( index && index->good() && mgr->good() && errorNum != E_OK )
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

std::ostream &
HashTableBase::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! HashTableBase::good() )
    dest << prefix << "Error: " << HashTableBase::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  clue::Str pre;
  pre = prefix;
  pre << "mgr:";
  mgr->dumpInfo( dest, pre );

  if( index )
    {
      pre = prefix;
      pre << "index:";
      index->dumpInfo( dest, pre );
    }

  if( index && index->getBase() )
    {
      dest << prefix << "version:    "
	   << ((const Header *)index->getBase())->version << '\n'
	   << prefix << "count:      "
	   << ((const Header *)index->getBase())->count << '\n'
	   << prefix << "endhash:    " << endHash() << '\n'
	;
    }

  return( dest );
}

std::ostream &
HashTableBase::dumpTable(
  std::ostream &	dest,
  const DumpMethods &	meth
  ) const
{
  dest << "hash      node  prev  next\n";

  for( HashValue hash = first(); hash < endHash(); ++ hash )
    {
      for( Loc node = hashLoc( hash );
	   node != 0;
	   node = hashNode( node ).next )
	{
	  dest << std::setw(8) << hash
	       << std::setw(6) << node
	       << std::setw(6) << hashNode( node ).prev
	       << std::setw(6) << hashNode( node ).next
	       << ' '
	    ;
	  meth.dumpNode( dest, node ) << '\n';
	}
    }
  return( dest );
}

std::ostream &
HashTableBase::dumpNode( std::ostream & dest, Loc  node ) const
{
  return( dest );
}

HashTableBase::Loc
HashTableBase::insert( HashValue hash, Loc node )
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
HashTableBase::erase( HashValue hash, Loc node )
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
  HashValue	    firstHash,
  Loc	    firstNode,
  HashValue	    lastHash,
  Loc	    lastNode
  )
{
  if( ! good() )
    return( false );

  HashValue  curHash  = firstHash;
  Loc	curNode  = firstNode;

  HashValue	nextHash = firstHash;
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

}; // namespace mdb
