// 1997-06-02 (cc) Paul Houghton <paul4hough@gmail.com>

#include "DashTableBase.hpp"
#include <clue/Str.hpp>

namespace mdb {

DashTableBase::DashTableBase(
  MultiMemOffset *	memMgr,
  const char *		indexFileName,
  std::ios::openmode    mode,
  bool			create,
  unsigned short	permMask
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
  static clue::Str errStr;

  errStr = "DashTableBase";

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


std::ostream &
DashTableBase::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! DashTableBase::good() )
    dest << prefix << "Error: " << DashTableBase::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  clue::Str pre;
  pre = prefix;
  pre << "HashTableBase::";

  HashTableBase::dumpInfo( dest, pre );

  return( dest );
}

//
// insert a new node into an existing list of nodes with the same key
//
HashTableBase::Loc
DashTableBase::insert( HashValue hash, Loc cur, EffDate effDate, Loc node )
{
  Loc	    prev;

  for( prev = cur ; cur; cur = dashNode( cur ).nextEff )
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
DashTableBase::insert( HashValue hash, EffDate effDate, Loc node )
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
DashTableBase::erase( HashValue hash, Loc node )
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

}; // namespace mdb
