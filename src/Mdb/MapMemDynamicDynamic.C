//
// File:        MapMemDynamicDynamic.C
// Project:	Clue
// Desc:        
//
//  Compiled sources for MapMemDynamicDynamic
//  
// Author:      Paul A. Houghton - (houghton@cshore.wcom.com)
// Created:     03/07/97 05:24
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "MapMemDynamicDynamic.hh"
#include <Str.hh>
#include <LibLog.hh>
#include <ClueUtils.hh>
#include <algorithm>
#include <iomanip>
#include <cstring>
#include <assert.h>

#if defined( MDB_DEBUG )
#include "MapMemDynamicDynamic.ii"
#endif

MDB_VERSION(
  MapMemDynamicDynamic,
  "$Id$");

const char * MapMemDynamicDynamic::ErrorStrings[] =
{
  ": ok",
  ": ",
  ": Bad size requested",
  0
};

#if defined( MDD_DEBUG )

static long	allocSplit = 0;
static long	allocWhole = 0;
static long	relOnly = 0;
static long	relJoinPSN = 0;
static long	relJoinPS = 0;
static long	relJoinSN = 0;
static long	relFirst = 0;
static long	relLast = 0;
static long	relMiddleBeg = 0;
static long	relMiddleEnd = 0;
static long	shrunkMap = 0;
static long	expandNewNode = 0;
static long	expandFreeEmpty = 0;
static long	expandLastNode = 0;

#endif

MapMemDynamicDynamic::MapMemDynamicDynamic(
  const char *	    fileName,
  ios::open_mode    mode,
  bool		    create,
  size_type	    minChunkSize,
  size_type	    allocSize,
  MapMask	    permMask
  )
  : MapMemDynamic( fileName,
		   MDD_VERSION,
		   mode,
		   create,
		   ( sizeof( MapDynamicDynamicInfo ) +
		     DwordAlign( max( allocSize, (size_type)getpagesize())) ),
		   permMask )
{
  if( create )
    createMapMemDynamicDynamic( minChunkSize, allocSize);
  else
    openMapMemDynamicDynamic();
}

MapMemDynamicDynamic::MapMemDynamicDynamic(
  const char * 	    fileName,
  size_type	    minChunkSize,
  size_type	    allocSize,
  MapMask    permMask  
  )
  : MapMemDynamic( fileName,
		   MDD_VERSION,
		   ( sizeof( MapDynamicDynamicInfo ) +
		     DwordAlign( max( allocSize, (size_type)getpagesize())) ),
		   permMask )
{
  createMapMemDynamicDynamic( minChunkSize, allocSize );
};

MapMemDynamicDynamic::MapMemDynamicDynamic(
  const char *	    fileName,
  ios::open_mode    mode,
  bool		    overrideOwner
  )
  : MapMemDynamic( fileName, MDD_VERSION, mode, overrideOwner )
{
  openMapMemDynamicDynamic();
}

MapMemDynamicDynamic::~MapMemDynamicDynamic( void )
{
}

MapMemDynamicDynamic::Loc
MapMemDynamicDynamic::allocFreeNode( Loc f, size_type chunkSize )
{
  if( freeNodeSize( f ) >= (chunkSize + mapInfo()->minChunkSize ) )
    {
      // this node is big enough to split into the allocated
      // node and another free node
      
      Loc	nf = f + chunkSize;
      
      freeNode( nf ).next	= freeNode( f ).next;
      freeNode( nf ).prev	= f;
      freeNode( nf ).nextFree	= freeNode( f ).nextFree;
      freeNode( nf ).prevFree	= freeNode( f ).prevFree;
      
      // insert the new free node into the freeList
      setFreePrevNext( f, nf );
      setFreeNextPrev( f, nf );
      
      // change prev->next to non free
      if( freeNode( f ).prev )
	freeNode( freeNode( f ).prev ).next = f;
      
      // change next->prev to free 'nf'
      setNextPrev( f, -nf );
      
      freeNode( f ).next = -nf;	// node's next is a free 'nf'
      
      mapInfo()->chunkSize += chunkSize;
      mapInfo()->freeSize  -= chunkSize;
      
#if defined( MDD_DEBUG )
      _LLg( LogLevel::Test )
	<< "allocate( " << setw( 4 ) << chunkSize << " ) split: "
	<< setw( 6 ) << "n: " << nodeLoc( f )
	<< setw( 6 ) << "a: " << freeNodeSize( f )
	<< endl;
      ++ allocSplit;
#endif
    }
  else
    {
      // not big enough to split so give the whole node
      
      // remove 'f' from free list
      setFreePrevNext( f, freeNode( f ).nextFree );
      setFreeNextPrev( f, freeNode( f ).prevFree );
      
      // change next->prev to non free
      if( freeNode( f ).next )
	freeNode( freeNode( f ).next ).prev = f;
      
      // change prev->next to non free
      if( freeNode( f ).prev )
	freeNode( freeNode( f ).prev ).next = f;
      
      mapInfo()->chunkSize  += freeNodeSize( f );
      mapInfo()->freeSize   -= freeNodeSize( f );
      
      -- mapInfo()->freeCount;
      
#if defined( MDD_DEBUG )
      _LLg( LogLevel::Test )
	<< "allocate( " << setw( 4 ) << chunkSize << " ) whole: "
	<< setw( 6 ) << "n: " << nodeLoc( f )
	<< setw( 6 ) << "a: " << freeNodeSize( f )
	<< endl;
      
      ++ allocWhole;
#endif
    }
  return( nodeLoc( f ) );
}

MapMemDynamicDynamic::Loc
MapMemDynamicDynamic::allocate( size_type size )
{
  if( ! good() )
    return( 0 );

  size_type    chunkSize = max( DwordAlign( size + sizeof( Node ) ),
				mapInfo()->minChunkSize );

  ++ mapInfo()->chunkCount;

  Loc f;
  
  for( f = freeList().nextFree;
       f;
       f = freeNode( f ).nextFree )
    {
      if( freeNodeSize( f ) < chunkSize )
	continue;

  // this node is big enough to satisfy the  request.

      return( allocFreeNode( f, chunkSize ) );
    }
  
  // no free chunks or i didn't find one large enough
  expand( chunkSize );

  if( freeList().prevFree && freeNodeSize( freeList().prevFree ) > chunkSize )
    return( allocFreeNode( freeList().prevFree, chunkSize ) );
  else
    return( 0 );
}

void
MapMemDynamicDynamic::release( Loc offset )
{
  if( ! good() )
    return;
  
  Loc		f = freeLoc( offset );

  -- mapInfo()->chunkCount;

  mapInfo()->chunkSize -= freeNodeSize( f );
  mapInfo()->freeSize  += freeNodeSize( f );
  
  if( freeList().nextFree == 0 )
    {
      // no other free nodes, put this node into the free list
      freeList().nextFree = f;
      freeList().prevFree = f;

      freeNode( f ).nextFree = 0;
      freeNode( f ).prevFree = 0;

      freeNode( f ).prev = abs( freeNode( f ).prev );
      freeNode( f ).next = abs( freeNode( f ).next );

      // change next->prev to a free 'f'
      if( freeNode( f ).next )
	freeNode( freeNode( f ).next ).prev = -f;

      // change prev->next to a free 'f'
      if( freeNode( f ).prev )
	freeNode( freeNode( f ).prev ).next = -f;
      
      ++ mapInfo()->freeCount;
      
#if defined( MDD_DEBUG )
      _LLg( LogLevel::Test )
	<< "release ( " << offset << " ) only."
	<< endl;
      ++ relOnly;
#endif
    }
  else
    {
      if( freeNode( f ).prev < 0 )
	{
	  // prev is a free node, join with this node

	  Loc		prevF = abs( freeNode( f ).prev );
	  
	  if( freeNode( f ).next < 0 )
	    {
	      // and next is a free node, join all three

	      Loc		nextF = abs( freeNode( f ).next );
	      
	      freeNode( prevF ).next	    = freeNode( nextF ).next;
	      freeNode( prevF ).nextFree    = freeNode( nextF ).nextFree;
	      
	      setFreeNextPrev( nextF, prevF );

#if defined( MDD_DEBUG )
	      _LLg( LogLevel::Test )
		<< "release ( " << offset << " ) join prev self next."
		<< endl;
	      ++ relJoinPSN;
#endif
	      -- mapInfo()->freeCount;
	    }
	  else
	    {
	      // no next or next is an allocated node
	      freeNode( prevF ).next = freeNode( f ).next;

#if defined( MDD_DEBUG )
	      _LLg( LogLevel::Test )
		<< "release ( " << offset << " ) join prev self."
		<< endl;
	      ++ relJoinPS;
#endif
	    }
	  
	  // changed next->prev to free 'prevF'
	  setNextPrev( prevF, -prevF );
	}
      else
	{
	  if( freeNode( f ).next < 0 )
	    {
	      // next is a free node, join with this node
	      
	      Loc	    nextF = abs( freeNode( f ).next );
	      
	      // put 'f' in the list where 'nextF' was
	      setFreePrevNext( nextF, f );
	      setFreeNextPrev( nextF, f );
	      
	      freeNode( f ).nextFree	= freeNode( nextF ).nextFree;
	      freeNode( f ).prevFree	= freeNode( nextF ).prevFree;

	      // set my next node to nextFreeNode's next
	      freeNode( f ).next	= freeNode( nextF ).next;

	      // change prev->next to free 'f'
	      if( freeNode( f ).prev )
		freeNode( freeNode( f ).prev ).next = - f;
	      
	      // change next->prev to free 'f'
	      setNextPrev( f, -f );
		  
#if defined( MDD_DEBUG )
	      _LLg( LogLevel::Test )
		<< "release ( " << offset << " ) join self next."
		<< endl;
	      ++ relJoinSN;
#endif
	    }
	  else
	    {
	      // can't join with anybody so just put into free list.

	      ++ mapInfo()->freeCount;
	      
	      // first changed next->prev and prev->next to free 'f'

	      if( freeNode( f ).prev )
		freeNode( freeNode( f ).prev ).next = - f;

	      if( freeNode( f ).next )
		freeNode( freeNode( f ).next ).prev = - f;
	      
	      if( freeList().nextFree > f )
		{
		  // 'f' is before the first free.
		  freeNode( f ).prevFree = 0;
		  freeNode( f ).nextFree = freeList().nextFree;

		  freeNode( freeNode( f ).nextFree ).prevFree = f;

		  freeList().nextFree = f;
		  
#if defined( MDD_DEBUG )
		  _LLg( LogLevel::Test )
		    << "release ( " << offset << " ) first."
		    << endl;
		  ++ relFirst;
#endif
		}
	      else
		{
		  if( freeList().prevFree < f )
		    {
		      // 'f' is after the last free.

		      freeNode( f ).prevFree = freeList().prevFree;
		      freeNode( f ).nextFree = 0;

		      freeNode( freeNode( f ).prevFree ).nextFree = f;

		      freeList().prevFree = f;
		      
#if defined( MDD_DEBUG )
		      _LLg( LogLevel::Test )
			<< "release ( " << offset << " ) Last."
			<< endl;
		      ++ relLast;
#endif
		    }
		  else
		    {
		      // firstFree < f < lastFree so ...
		      
		      // find out if I'm closer to the begining or
		      // end of the free list

		      Loc nextF;
		      Loc prevF;
		      
		      if( ( freeList().prevFree - f ) <
			  ( f - freeList().nextFree ) )
			{
			  // closer to the last of the free list
			  prevF = freeList().prevFree;
			  
			  for( prevF =  freeNode( prevF ).prevFree;
			       prevF > f;
			       prevF =  freeNode( prevF ).prevFree );

			  nextF = freeNode( prevF ).nextFree;
#if defined( MDD_DEBUG )
			  _LLg( LogLevel::Test )
			    << "release ( " << offset << " ) Middle end."
			    << endl;
			  ++ relMiddleEnd;

			  assert( prevF < f && f < nextF );
#endif
			}
		      else
			{
			  nextF = freeList().nextFree;

			  for( nextF = freeNode( nextF ).nextFree;
			       nextF < f;
			       nextF = freeNode( nextF ).nextFree );

			  prevF = freeNode( nextF ).prevFree;
#if defined( MDD_DEBUG )
			  _LLg( LogLevel::Test )
			    << "release ( " << offset << " ) Middle beg."
			    << endl;
			  ++ relMiddleBeg;
			  
			  assert( prevF < f && f < nextF );
#endif
			}

		      // now put f in the free list
		      freeNode( f ).nextFree = nextF;
		      freeNode( f ).prevFree = prevF;

		      freeNode( nextF ).prevFree = f;
		      freeNode( prevF ).nextFree = f;
		    }
		}
	    }
	}
    }

  // now see if there is enough space at the end to shrink

  if( ! freeNode( freeList().prevFree ).next &&
      ( getMapSize() - freeList().prevFree ) >
      ( mapInfo()->allocSize * 2 ) )
    {
      size_type	shrinkAmount = ( freeNodeSize(  freeList().prevFree ) -
				 mapInfo()->allocSize );
      
      // shrink by all but one alloc unit;
#if defined( MDB_DEBUG )
      _LLg( LogLevel::Test )
	<< "SHRINK: pre: amount: " << shrinkAmount
	<< '\n' << dump( " pre: " )
	<< '\n' ;
      
      if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
	dumpNodes( *_LibLog ) << endl;
#endif
      
      size_type origSize = getMapSize();
      
      size_type newSize =
	shrink( shrinkAmount, (caddr_t)(mapInfo()->base) );
  
      mapInfo()->size = getSize();
  
      mapInfo()->freeSize -= (origSize - newSize);
  
#if defined( MDB_DEBUG )
      ++ shrunkMap;
      
      _LLg( LogLevel::Test )
	<< "SHRINK: post: "
	<< '\n' << dump( " post: " )
	<< '\n' ;
      
      if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
	dumpNodes( *_LibLog ) << endl;

#endif
    }
}


void
MapMemDynamicDynamic::expand( size_type minAmount )
{
  if( ! good() )
    return;

  size_type	origSize = getMapSize();
  
  size_type	lastFreeSize = ( freeList().prevFree ?
				 freeNodeSize( freeList().prevFree ) : 0 );
  
  size_type	amount = max( ( minAmount > lastFreeSize ?
				( (minAmount - lastFreeSize) +  1 ) :
				minAmount ),
			      (size_type)(mapInfo()->allocSize ));

  // Note: if the minAmount is > lastFreeSize, then expand is probably
  //	being called because there is not enough room for a specific node.
  //    With this in mind, I only expand just enough to ensure the last
  //    free node is big enough to accomidate an allocate request for
  //	'minAmount'.
  
#if defined( MDB_DEBUG )
  _LLg( LogLevel::Test )
    << "EXPAND: pre: \n"
    << "    minAmount:     " << minAmount << '\n'
    << "    amount:        " << amount << '\n'
    << "    origSize:      " << origSize << '\n'
    << "    lastFree:      " << freeList().prevFree << '\n'
    << "    lastFreeSize:  " << lastFreeSize << '\n'
    << dump( " pre: " )
    << endl; ;

  if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
    dumpNodes( *_LibLog ) << endl;
  
#endif
  
  if( grow( amount, (caddr_t)mapInfo()->base ) == 0 )
    {
      errorNum = E_MAPMEM;
      return;
    }

  mapInfo()->size = getSize();

  mapInfo()->freeSize += getMapSize() - origSize;
  
  // now add these recs to the free list

  if( lastFreeSize + freeList().prevFree != origSize )
    {       
#if defined( MDB_DEBUG )
      if( freeList().prevFree )
	{
	  _LLg( LogLevel::Test )
	    << "EXPAND: post last not free: \n";
	    ++ expandNewNode;
	}
      else
	{
	  _LLg( LogLevel::Test )
	    << "EXPAND: post empty free list: \n";
	  ++ expandFreeEmpty;
	}
      
      if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
	{
	  dumpInfo( *_LibLog, " post: " ) << endl;
	  dumpNodes( *_LibLog ) << endl;
	}

#endif

      freeNode( origSize ).next = 0;
      freeNode( origSize ).prev = freeList().prev;
      
      if( freeList().prev )
	{
	  freeNode( freeList().prev ).next = - origSize;
	  freeList().prev = origSize;
	}
      else
	{
	  freeList().next = origSize;
	  freeList().prev = origSize;
	}

      freeNode( origSize ).nextFree = 0;
      freeNode( origSize ).prevFree = freeList().prevFree;
      
      if( freeList().prevFree )
	{
	  freeNode( freeList().prevFree ).nextFree = origSize;
	  freeList().prevFree = origSize;
	}
      else
	{
	  freeList().nextFree = origSize;
	  freeList().prevFree = origSize;
	}
      
      ++ mapInfo()->freeCount;
      
#if defined( MDB_DEBUG )
      
      _LLg( LogLevel::Test )
	<< "EXPAND: post last not free done: "
	<< '\n' << dump( " post: " ) << '\n';
      
      if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
	dumpNodes( *_LibLog ) << endl;

#endif
    }
#if defined( MDB_DEBUG )
  else
    {
      ++ expandLastNode;
      
      _LLg( LogLevel::Test )
	<< "EXPAND: post last is free done: "
	<< '\n' << dump( " post: " ) << '\n';
      
      if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
	dumpNodes( *_LibLog ) << endl;

    }
#endif
 
}

bool
MapMemDynamicDynamic::good( void ) const
{
  return( mapInfo() != 0 &&
	  errorNum == E_OK &&
	  MapMemDynamic::good() );
}

const char *
MapMemDynamicDynamic::error( void ) const
{
  static Str errStr;

  errStr = MapMemDynamicDynamic::getClassName();

  if( good() )
    {
      errStr << ": ok";
    }
  else
    {
      size_type eSize = errStr.size();

      if( errorNum > E_MAPMEM && errorNum < E_UNDEFINED )
	errStr << ErrorStrings[ errorNum ];

      
      if( ! MapMemDynamic::good() )
	  errStr << ": " << MapMem::error();

      if( eSize == errStr.size() )
        errStr << ": unknown error";
    }

  return( errStr.c_str() );
}

const char *
MapMemDynamicDynamic::getClassName( void ) const
{
  return( "MapMemDynamicDynamic" );
}

const char *
MapMemDynamicDynamic::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
}


ostream &
MapMemDynamicDynamic::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << MapMemDynamicDynamic::getClassName() << ":\n"
	 << MapMemDynamicDynamic::getVersion() << '\n';

  if( ! MapMemDynamicDynamic::good() )
    dest << prefix << "Error: " << MapMemDynamicDynamic::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << MapMemDynamic::getClassName() << "::";

  MapMemDynamic::dumpInfo( dest, pre, false );

  if( mapInfo() )
    {
      dest << prefix << "minChunkSize: " << mapInfo()->minChunkSize << '\n'
	   << prefix << "allocSize:    " << mapInfo()->allocSize << '\n'
	   << prefix << "chunkSize:    " << mapInfo()->chunkSize << '\n'
	   << prefix << "freeSize:     " << mapInfo()->freeSize << '\n'
	   << prefix << "next:         " << freeList().next << '\n'
	   << prefix << "prev:         " << freeList().prev << '\n'
	   << prefix << "freeNext:     " << freeList().nextFree << '\n'
	   << prefix << "freePrev:     " << freeList().prevFree << '\n'
#if defined( MDD_DEBUG )
	   << prefix << "A split:      " << allocSplit << '\n'
	   << prefix << "A whole:      " << allocWhole << '\n'
	   << prefix << "R only:       " << relOnly << '\n'
	   << prefix << "R join psn:   " << relJoinPSN << '\n'
	   << prefix << "R join ps:    " << relJoinPS << '\n'
	   << prefix << "R join sn:    " << relJoinSN << '\n'
	   << prefix << "R first:      " << relFirst << '\n'
	   << prefix << "R last:       " << relLast << '\n'
	   << prefix << "R mid beg:    " << relMiddleBeg << '\n'
	   << prefix << "R mid end:    " << relMiddleEnd << '\n'
	   << prefix << "shrink count: " << shrunkMap << '\n'
	   << prefix << "expand new:   " << expandNewNode << '\n'
	   << prefix << "expand empty: " << expandFreeEmpty << '\n'
	   << prefix << "expand last:  " << expandLastNode << '\n'
#endif
	;      
    }
  else
    {
      dest << prefix << "No Base Addr!\n";
    }

  return( dest );
}

ostream &
MapMemDynamicDynamic::dumpFreeList( ostream & dest ) const
{
  if( ! good() )
    {
      dest << error() << endl;
      return( dest );
    }
  
  dest << "FreeList Dump: count: " << mapInfo()->freeCount
       << " size: " << mapInfo()->freeSize
       << " first: " << freeList().nextFree
       << " last: " << freeList().prevFree << '\n' ;
  dest << "FreeList Nodes:    node    size    prev    next\n";
  
  for( off_t f = freeList().nextFree;
       f;
       f = freeNode( f ).nextFree )
    {
      
      dest << "Free Node:       "
	   << setw(6) << f
	   << setw(8) << freeNodeSize( f )
	   << setw(8) << freeNode( f ).prevFree
	   << setw(8) << freeNode( f ).nextFree
	   << '\n'
	;
    }
  return( dest );
}

ostream &
MapMemDynamicDynamic::dumpNodes( ostream & dest ) const
{
  dest << "Node List:         node    size    prev    next   prevF   nextF\n";
  
  for( Loc n = ( (freeList().next == freeList().nextFree) ?
		 - freeList().next : freeList().next );
       n;
       n = freeNode( abs( n ) ).next )
    {
      dest << "Node:            "
	   << setw(6) << n
	   << setw(8) << freeNodeSize( abs( n ) )
	   << setw(8) << freeNode( abs( n ) ).prev
	   << setw(8) << freeNode( abs( n ) ).next
	;
      if( n < 0 )
	{
	  dest << setw( 8 ) << freeNode( abs( n ) ).prevFree
	       << setw( 8 ) << freeNode( abs( n ) ).nextFree
	    ;
	}
      dest << endl;
    }

  return( dest );
}
  
void
MapMemDynamicDynamic::createMapMemDynamicDynamic(
  size_type	minChunkSize,
  size_type	allocSize
  )
{
  if( mapInfo() != 0 && MapMemDynamic::good() )
    {
      mapInfo()->minChunkSize	=
	DwordAlign( max( (size_type) (minChunkSize + sizeof( Node ) ),
			 (size_type) sizeof( FreeNode) ) );
      
      mapInfo()->allocSize	= DwordAlign( max( allocSize,
						   (size_type)getpagesize()) );
      mapInfo()->chunkSize	= 0;
      mapInfo()->freeSize	= ( getMapSize() -
				    DwordAlign(
				      sizeof( MapDynamicDynamicInfo ) ));

      mapInfo()->freeCount  = 1;
      
      freeList().next	    = DwordAlign( sizeof( MapDynamicDynamicInfo ) );
      freeList().prev	    = freeList().next;
      freeList().nextFree   = freeList().next; 
      freeList().prevFree   = freeList().next;

      freeNode( freeList().next ).next = 0;
      freeNode( freeList().next ).prev = 0;
      freeNode( freeList().next ).nextFree = 0;
      freeNode( freeList().next ).prevFree = 0;
      
      errorNum = E_OK;
    }
  else
    {
      errorNum = E_MAPMEM;
    }
}

void
MapMemDynamicDynamic::openMapMemDynamicDynamic( void )
{

  if( mapInfo() == 0 || ! MapMemDynamic::good() )
    {
      errorNum = E_MAPMEM;
      return;
    }

  errorNum = E_OK;

}

  
// Revision Log:
//
// $Log$
// Revision 2.12  1997/06/27 12:15:30  houghton
// Major rework to speed up 'release'.
//
// Revision 2.11  1997/06/25 12:55:26  houghton
// Cleanup.
//
// Revision 2.10  1997/06/19 12:02:02  houghton
// Changed to be part of libMdb.
//
// Revision 2.9  1997/06/18 14:14:36  houghton
// Added include ClueUtils.
//
// Revision 2.8  1997/06/09 11:58:26  houghton
// Bug-Fix: check if there is a _LibLog before I try to use it.
//
// Revision 2.7  1997/06/05 11:27:59  houghton
// Cleanup.
// Change to be part of libMdb (vs Clue1).
// Changed to be a subclass of MapMemDynamic (vs MapMem).
// Added constructor that can create or open existing.
// Changed to use new MapFile types.
// Removed addRef, getRefCount & delRef methods (now in base class).
// Changed getMem to allocate.
// Changed freeMem to release.
// Added createMapMemDynamicDynamic and openMapMemDynamicDynamic methods.
//
// Revision 2.6  1997/04/22 10:34:39  houghton
// Added a check to seek if someone is trying to free an already free'd
//     node. If so, log a warning and return.
//
// Revision 2.5  1997/04/04 20:50:09  houghton
// Cleanup.
// Added map owner to prevent to progs from opening the map in write
//     mode at the same time.
//
// Revision 2.4  1997/03/19 16:23:40  houghton
// Bug-Fix: stray character.
//
// Revision 2.3  1997/03/18 16:56:08  houghton
// Bug-Fix: free p n x was not setting the next node's prev.
// Added Lots of debug output using LibLog Levels App1 & App2. ifdef'd
//     with CLUE1_DEBUG.
//
// Revision 2.2  1997/03/13 02:24:37  houghton
// Bug-Fix: change calls to getSize to getMapSize.
// Added refCount.
// Minor changes for AIX port.
//
// Revision 2.1  1997/03/08 10:29:52  houghton
// Initial partially tested version.
//
//
