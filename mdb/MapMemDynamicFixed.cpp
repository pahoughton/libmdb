// 1994-11-28 (cc) Paul Houghton <paul4hough@gmail.com>

#include "MapMemDynamicFixed.hpp"
#include <clue/Str.hpp>
#include <clue/LibLog.hpp>
#include <iomanip>
#include <cstring>

#include <assert.h>

namespace mdb {

const char *
MapMemDynamicFixed::ErrorStrings[] =
  {
    ": ok",
    ": ",
    ": Bad size requested",
    0
  };

#if defined( MDF_DEBUG )

static long	relOnly = 0;
static long	relLast = 0;
static long	relFirst = 0;
static long	relMiddleEnd = 0;
static long	relMiddleBeg = 0;
static long	shrunkMap = 0;
static long	expandFreeEmpty = 0;
static long	expandFreeNotEmpty = 0;

#endif

//
// allocation chunks must be at least 1 page
//

MapMemDynamicFixed::MapMemDynamicFixed(
  const char *	    fileName,
  std::ios::openmode    mode,
  bool		    create,
  size_type	    recSize,
  size_type	    allocNumRecs,
  MapMask	    permMask
  )
  : MapMemDynamic( fileName,
		   MDF_VERSION,
		   mode,
		   create,
		   (recSize * allocNumRecs) + sizeof( MapDynamicFixedInfo ),
		   permMask )
{
  if( create )
    createMapMemDynamicFixed( recSize, allocNumRecs);
  else
    openMapMemDynamicFixed();
}


MapMemDynamicFixed::MapMemDynamicFixed(
  const char * 	    fileName,
  size_type	    recSize,
  size_type	    allocNumRecs,
  MapMask	    permMask
  )
  : MapMemDynamic( fileName,
		   MDF_VERSION,
		   (recSize * allocNumRecs) + sizeof( MapDynamicFixedInfo ),
		   permMask )
{
  createMapMemDynamicFixed( recSize, allocNumRecs );
}

MapMemDynamicFixed::MapMemDynamicFixed(
  const char * 	    fileName,
  std::ios::openmode    mode,
  bool		    overrideOwner
  )
  : MapMemDynamic( fileName, MDF_VERSION, mode, overrideOwner )
{
  openMapMemDynamicFixed();
}


MapMemDynamicFixed::~MapMemDynamicFixed( void )
{
}

MapMemDynamicFixed::Loc
MapMemDynamicFixed::allocate( size_type size )
{
  if( ! good() )
    return( 0 );

  //
  // we can only allocate fixed size records
  //

  if( size > mapInfo()->recSize )
    {
      errorNum = E_BADSIZE;
      return( 0 );
    }

  if( freeList().nextFree == 0 )
    {
      if( ! expand( mapInfo()->allocNumRecs * mapInfo()->recSize ) )
	return( 0 );
    }

  Loc	f = freeList().nextFree;

  freeList().nextFree = freeNode( f ).nextFree;

  if( freeList().nextFree != 0 )
    freeNode( freeList().nextFree ).prevFree = 0;
  else
    freeList().prevFree = 0;

  -- mapInfo()->freeCount;
  ++ mapInfo()->allocCount;

  return( f );
}




//
// The free list should be kept in order from
// beginning of file to the end of the file.
// allocate() always uses to first available record.
//
// This increases the chances of being able to shrink
// the map file when there are 2 * the chunksize free
// records at the end of the file.
//

void
MapMemDynamicFixed::release( Loc f )
{

  if( ! good() )
    return;

  -- mapInfo()->allocCount;
  ++ mapInfo()->freeCount;

  if( freeList().nextFree == 0 )
    {
      freeList().nextFree = f;
      freeList().prevFree = f;

      freeNode( f ).nextFree = 0;
      freeNode( f ).prevFree = 0;

#if defined( MDF_DEBUG )
      _LLg( LogLevel::Test )
	<< "release ( " << f << " ) only."
	<< std::endl;
      ++ relOnly;
#endif
    }
  else
    {

      if( freeList().prevFree < f )
	{
	  // 'f' is after the last free
	  freeNode( f ).prevFree = freeList().prevFree;
	  freeNode( f ).nextFree = 0;

	  freeNode( freeNode( f ).prevFree ).nextFree = f;

	  freeList().prevFree = f;

#if defined( MDF_DEBUG )
	  _LLg( LogLevel::Test )
	    << "release ( " << f << " ) Last."
	    << std::endl;
	  ++ relLast;
#endif
	}
      else
	{
	  if( freeList().nextFree > f )
	    {
	      // 'f' is before the first free.
	      freeNode( f ).prevFree = 0;
	      freeNode( f ).nextFree = freeList().nextFree;

	      freeNode( freeNode( f ).nextFree ).prevFree = f;

	      freeList().nextFree = f;

#if defined( MDF_DEBUG )
	      _LLg( LogLevel::Test )
		<< "release ( " << f << " ) first."
		<< std::endl;
	      ++ relFirst;
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
#if defined( MDF_DEBUG )
		  _LLg( LogLevel::Test )
		    << "release ( " << f << " ) MidEnd"
		    << " first: " << freeList().nextFree
		    << " last: " << freeList().prevFree
		    << std::endl;
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
#if defined( MDF_DEBUG )
		  _LLg( LogLevel::Test )
		    << "release ( " << f << " ) MidBeg"
		    << " first: " << freeList().nextFree
		    << " last: " << freeList().prevFree
		    << std::endl;
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

  if( mapInfo()->freeCount > (mapInfo()->allocNumRecs * 2) )
    {
      //
      // see if there are enough contigious free recs at the
      // end to release a chunk
      //

      // is the last rec free?

      if( freeList().prevFree == lastNode() )
	{
	  Loc	lastFree;

	  // find the amount of contigious free recs
	  for( lastFree = lastNode();
	       (size_type)freeNode( lastFree ).prevFree ==
		 (lastFree - mapInfo()->recSize);
	       lastFree = freeNode( lastFree ).prevFree );

	  if( (size_type)(lastNode() - lastFree) >=
	      ( mapInfo()->allocNumRecs * mapInfo()->recSize ) &&
	      (size_type)(lastNode() - lastFree) >= getPageSize() )

	    {
	      // ok we have enough free recs at the end to
	      // shrink the map

	      unsigned long endFreeRecs;
	      unsigned long otherFreeRecs;
	      unsigned long shrinkRecs;

	      // endFreeRecs: the number of contigous free records at
	      //		the end of the map
	      // otherFreeRecs: the number of free recs NOT at the end.
	      // shrinkRecs: the number of recs to shrink the map by

	      endFreeRecs = (lastNode() - lastFree) / mapInfo()->recSize;
	      otherFreeRecs = mapInfo()->freeCount - endFreeRecs;

	      // if there are at least 'chunkSize' free recs NOT at
	      // the end of the map, shrink by 'endFreeRecs'. otherwise,
	      // shrink by all by 'chunkSize' recs.

	      if( otherFreeRecs > mapInfo()->allocNumRecs )
		shrinkRecs = endFreeRecs;
	      else
		shrinkRecs = mapInfo()->freeCount - mapInfo()->allocNumRecs;

#if defined( MDF_DEBUG )
	      _LLg( LogLevel::Test )
		<< "SHRINK: pre:\n"
		<< "    amount: " << shrinkRecs * mapInfo()->allocNumRecs << '\n'
		<< "	lastNode:   " << lastNode() << '\n'
		<< "    lastFree:   " << lastFree << '\n'
		<< "    shrinkRecs: " << shrinkRecs << '\n'
		<< '\n' << dump( " pre: " )
		<< '\n' ;

	      if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
		dumpNodes( *_LibLog ) << std::endl;
#endif

	      size_t newSize = shrink( shrinkRecs * mapInfo()->recSize,
				       (caddr_t)mapInfo()->base  );

	      if( ! newSize )
		return;

	      unsigned long newRecCount;

	      mapInfo()->size = getSize();

	      newRecCount = ( ( mapInfo()->size - firstNode() ) /
			      mapInfo()->recSize );

	      mapInfo()->freeCount = newRecCount - mapInfo()->allocCount;

	      freeList().prevFree = lastNode();

	      freeNode( lastNode() ).nextFree = 0;

#if defined( MDF_DEBUG )
	      ++ shrunkMap;

	      _LLg( LogLevel::Test )
		<< "SHRINK: post: "
		<< '\n' << dump( " post: " )
		<< '\n' ;

	      if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
		dumpNodes( *_LibLog ) << std::endl;
#endif
	    }
	}
    }
}

bool
MapMemDynamicFixed::valid( off_t offset ) const
{
  if( (offset - firstNode()) % mapInfo()->recSize )
    return( false );

  Loc f;

  for( f = freeList().nextFree;
       f && f < offset;
       f = freeNode( f ).nextFree );

  return( f == offset ? false : true );
}


bool
MapMemDynamicFixed::expand( size_type minAmount )
{

  if( ! good() )
    return( false );

  unsigned long amount = std::max( minAmount,
			      mapInfo()->allocNumRecs * mapInfo()->recSize );


  Loc	origLastNode = lastNode();

  if( grow( amount, (caddr_t)mapInfo()->base ) == 0 )
    {
      errorNum = E_MAPMEM;
      return( false );
    }

  mapInfo()->size = getSize();

  // f is the first new free node

  Loc f		= origLastNode + mapInfo()->recSize;
  Loc prevF	= freeList().prevFree;

  if( freeList().nextFree == 0 )
    {
      freeList().nextFree = f;
#if defined( MDF_DEBUG )
      _LLg( LogLevel::Test )
	<< "EXPAND: post free list empty: \n";
      ++ expandFreeEmpty;
    }
  else
    {
      _LLg( LogLevel::Test )
	<< "EXPAND: post free list NOT empty: \n";
      ++ expandFreeNotEmpty;
#endif
    }

  freeList().prevFree = lastNode();

  for( ; f < freeList().prevFree; f += mapInfo()->recSize )
    {
      freeNode( f ).nextFree = f + mapInfo()->recSize;
      freeNode( f ).prevFree = prevF;
      prevF = f;
    }

  freeNode( f ).nextFree = 0;
  freeNode( f ).prevFree = prevF;

  mapInfo()->freeCount += (lastNode() - origLastNode) / mapInfo()->recSize;

#if defined( MDF_DEBUG )
  if( _LibLog && _LibLog->willOutput( LogLevel::Test ) )
    {
      dumpInfo( *_LibLog, " post: " ) << std::endl;
      dumpNodes( *_LibLog ) << std::endl;
    }
#endif

  return( true );
}


bool
MapMemDynamicFixed::good( void ) const
{
  return( mapInfo() != 0 &&
	  errorNum == E_OK &&
	  MapMemDynamic::good() );
}

const char *
MapMemDynamicFixed::error( void ) const
{
  static clue::Str errStr;
  errStr.reset();

  errStr << "MapMemDynamicFixed";

  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {
      size_type eSize = errStr.size();

      if( errorNum > E_MAPMEM && errorNum < E_UNDEFINED )
	errStr << ErrorStrings[ errorNum ];


      if( ! MapMemDynamic::good() )
	errStr << ": " << MapMemDynamic::error();

      if( eSize == errStr.size() )
        errStr << ": unknown error";

    }
  return( errStr.cstr() );
}

std::ostream &
MapMemDynamicFixed::dumpInfo(
  std::ostream &    dest,
  const char *	    prefix
  ) const
{

  if( ! MapMemDynamicFixed::good() )
    dest << prefix << "Error: " << MapMemDynamicFixed::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  clue::Str pre;
  pre = prefix;
  pre << "MapMemDynamic::";

  MapMemDynamic::dumpInfo( dest, pre );

  if( mapInfo() )
    {
      dest << prefix << "rec size:     " << getRecSize() << '\n'
	   << prefix << "alloc recs:   " << getAllocNumRecs() << '\n'
	   << prefix << "first free:   " << freeList().nextFree << '\n'
	   << prefix << "last free:    " << freeList().prevFree << '\n'
#if defined( MDF_DEBUG )
	   << prefix << "R only:       " << relOnly << '\n'
	   << prefix << "R first:      " << relFirst << '\n'
	   << prefix << "R last:       " << relLast << '\n'
	   << prefix << "R mid beg:    " << relMiddleBeg << '\n'
	   << prefix << "R mid end:    " << relMiddleEnd << '\n'
	   << prefix << "shrink count: " << shrunkMap << '\n'
	   << prefix << "expand new:   " << expandFreeNotEmpty << '\n'
	   << prefix << "expand empty: " << expandFreeEmpty << '\n'
#endif
	;

    }
  else
    {
      dest << prefix << "No Base Addr!\n";
    }

  return( dest );
}

std::ostream &
MapMemDynamicFixed::dumpFreeList( std::ostream & dest ) const
{
  dest << "Node      prevF      nextF\n";

  dest << std::setw( 6 ) << ' '
       << std::setw( 8 ) << freeList().prevFree
       << std::setw( 8 ) << freeList().nextFree
       << std::endl;

  for( Loc f = freeList().nextFree;
       f ;
       f = freeNode( f ).nextFree )
    {
      dest << std::setw( 6 ) << f
	   << std::setw( 8 ) << freeNode( f ).prevFree
	   << std::setw( 8 ) << freeNode( f ).nextFree
	   << std::endl;
    }

  return( dest );
}

std::ostream &
MapMemDynamicFixed::dumpNodes( std::ostream & dest ) const
{
  Loc n	    = firstNode();
  Loc lastN = lastNode();

  Loc f	    = freeList().nextFree;

  dest << "  Node      prevF     nextF\n";

  dest << std::setw( 6 ) << " "
       << std::setw( 10 ) << freeList().prevFree
       << std::setw( 10 ) << freeList().nextFree
       << std::endl;

  for( ; n < lastN; n += getRecSize() )
    {

      dest << std::setw( 6 ) << n ;

      if( n == f )
	{
	  dest << std::setw( 10 ) << freeNode( f ).prevFree
	       << std::setw( 10 ) << freeNode( f ).nextFree
	    ;
	  f = freeNode( f ).nextFree;
	}

      dest << std::endl;
    }

  dest << std::setw( 6 ) << n ;

  if( n == f )
    {
      dest << std::setw( 10 ) << freeNode( f ).prevFree
	   << std::setw( 10 ) << freeNode( f ).nextFree
	;
    }

  dest << std::endl;

  return( dest );

}

bool
MapMemDynamicFixed::allTested( void )
{
#if defined( MDF_DEBUG )
  return( relOnly	     > 0 &&
	  relLast            > 0 &&
	  relFirst           > 0 &&
	  relMiddleEnd	     > 0 &&
	  relMiddleBeg       > 0 &&
	  shrunkMap	     > 0 &&
	  expandFreeEmpty    > 0 &&
	  expandFreeNotEmpty > 0 );
#else
  return( true );
#endif
}

void
MapMemDynamicFixed::createMapMemDynamicFixed(
  size_type recSize,
  size_type allocNumRecs
  )
{
  if( mapInfo() != 0 && MapMemDynamic::good() )
    {
      mapInfo()->recSize = clue::QwordAlign( std::max( recSize,
					    (size_type)sizeof(FreeNode) ) );

      mapInfo()->allocNumRecs = std::max( allocNumRecs,
				     ( getPageSize() /
				       mapInfo()->recSize) );

      mapInfo()->allocCount = 0;
      mapInfo()->freeCount = ( ( (lastNode() + mapInfo()->recSize) -
				 firstNode() ) /
				 mapInfo()->recSize );

      freeList().nextFree = firstNode();
      freeList().prevFree = lastNode();

      //
      // Initialize the list of free records
      //

      Loc   f;
      Loc   prevF = 0;

      for( f = freeList().nextFree;
	   f < freeList().prevFree;
	   f += mapInfo()->recSize )
	{
	  freeNode( f ).nextFree = f + mapInfo()->recSize;
	  freeNode( f ).prevFree = prevF;
	  prevF = f;
	}

      freeNode( f ).nextFree = 0;
      freeNode( f ).prevFree = prevF;

      errorNum = E_OK;
    }
  else
    {
      errorNum = E_MAPMEM;
    }
}

void
MapMemDynamicFixed::openMapMemDynamicFixed( void )
{
  if( mapInfo() == 0 || ! MapMemDynamic::good() )
    {
      errorNum = E_MAPMEM;
      return;
    }

  errorNum = E_OK;
}

}; // namespace mdb
