//
// File:        MapMemDynamicFixed.C
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/28/94 03:53 
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//

#include "MapMemDynamicFixed.hh"
#include <Str.hh>
#include <LibLog.hh>
#include <iomanip>
#include <cstring>

#include <assert.h>

#if defined( MDB_DEBUG )
#include "MapMemDynamicFixed.ii"
#endif


MDB_VERSION(
  MapMemDynamicFixed,
  "$Id$");


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
  ios::open_mode    mode,
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
  ios::open_mode    mode,
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
	<< endl;
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
	    << endl;
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
		<< endl;
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
#if defined( MDF_DEBUG )
		  _LLg( LogLevel::Test )
		    << "release ( " << f << " ) MidBeg"
		    << " first: " << freeList().nextFree
		    << " last: " << freeList().prevFree
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
		dumpNodes( *_LibLog ) << endl;
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
		dumpNodes( *_LibLog ) << endl;
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

  unsigned long amount = max( minAmount,
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
      dumpInfo( *_LibLog, " post: " ) << endl;
      dumpNodes( *_LibLog ) << endl;
    }
#endif
  
  return( true );
}


const char *
MapMemDynamicFixed::getClassName( void ) const
{
  return( "MapMemDynamicFixed" );
}

const char *
MapMemDynamicFixed::getVersion( bool withPrjVer ) const
{
  return( version.getVer( withPrjVer ) );
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
  static Str errStr;
  errStr.reset();

  errStr << MapMemDynamicFixed::getClassName();
  
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

ostream &
MapMemDynamicFixed::dumpInfo(
  ostream &	dest,
  const char *	prefix,
  bool		showVer
  ) const
{
  if( showVer )
    dest << MapMemDynamicFixed::getClassName() << ":\n"
	 << MapMemDynamicFixed::getVersion() << '\n' ;
      
  if( ! MapMemDynamicFixed::good() )
    dest << prefix << "Error: " << MapMemDynamicFixed::error() << '\n';
  else
    dest << prefix << "Good" << '\n';

  Str pre;
  pre = prefix;
  pre << MapMemDynamic::getClassName() << "::";

  MapMemDynamic::dumpInfo( dest, pre, false );

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

ostream &
MapMemDynamicFixed::dumpFreeList( ostream & dest ) const
{
  dest << "Node      prevF      nextF\n";
  
  dest << setw( 6 ) << ' '
       << setw( 8 ) << freeList().prevFree
       << setw( 8 ) << freeList().nextFree
       << endl;
  
  for( Loc f = freeList().nextFree;
       f ;
       f = freeNode( f ).nextFree )
    {
      dest << setw( 6 ) << f
	   << setw( 8 ) << freeNode( f ).prevFree
	   << setw( 8 ) << freeNode( f ).nextFree
	   << endl;
    }

  return( dest );
}

ostream &
MapMemDynamicFixed::dumpNodes( ostream & dest ) const
{
  Loc n	    = firstNode();
  Loc lastN = lastNode();

  Loc f	    = freeList().nextFree;

  dest << "  Node      prevF     nextF\n";
  
  dest << setw( 6 ) << " "
       << setw( 10 ) << freeList().prevFree
       << setw( 10 ) << freeList().nextFree
       << endl;
  
  for( ; n < lastN; n += getRecSize() )
    {
      
      dest << setw( 6 ) << n ;

      if( n == f )
	{
	  dest << setw( 10 ) << freeNode( f ).prevFree
	       << setw( 10 ) << freeNode( f ).nextFree
	    ;
	  f = freeNode( f ).nextFree;
	}
	    
      dest << endl;
    }

  dest << setw( 6 ) << n ;

  if( n == f )
    {
      dest << setw( 10 ) << freeNode( f ).prevFree
	   << setw( 10 ) << freeNode( f ).nextFree
	;
    }
	    
  dest << endl;

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
      mapInfo()->recSize = QwordAlign( max( recSize,
					    (size_type)sizeof(FreeNode) ) );

      mapInfo()->allocNumRecs = max( allocNumRecs,
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

// Revision Log:
//
// $Log$
// Revision 4.2  2003/07/19 09:11:13  houghton
// Port to 64 bit.
//
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.18  1997/09/17 16:56:07  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.17  1997/07/28 16:50:24  houghton
// Changed expand() to virtual and to take an 'size' arg.
//
// Revision 2.16  1997/07/13 11:27:16  houghton
// Cleanup.
// Major rework to make more readable and improve performance.
// Added lots of testing & debuging code.
//
// Revision 2.15  1997/06/25 12:56:13  houghton
// Bug-Fix: chunkCount was not being updated correctly.
//
// Revision 2.14  1997/06/19 12:02:28  houghton
// Class was renamed from MapMemFixedDynamic to MapMemDynamicFixed.
//
// Revision 2.13  1997/06/18 14:15:29  houghton
// Rework to use MapMemDynamic as base Class.
// Rework to be part of libMdb.
//
// Revision 2.12  1997/05/01 11:54:37  houghton
// Bug-Fix: valid() would get in an infinite loop if the free list was
//     empty.
//
// Revision 2.11  1997/04/25 22:25:27  houghton
// Added valid( off_t ) - returns true if the off_t is a valid usable
//     offset for this map.
//
// Revision 2.10  1997/04/21 12:12:40  houghton
// Cleanup.
//
// Revision 2.9  1997/04/04 20:50:19  houghton
// Cleanup.
// Added map owner to prevent to progs from opening the map in write
//     mode at the same time.
//
// Revision 2.8  1997/03/13 02:39:42  houghton
// Added getVersion.
// Added free list info to dumpInfo output.
//
// Revision 2.7  1997/03/08 10:28:59  houghton
// Cleanup.
// Added ClassVersion.
//
// Revision 2.6  1997/03/07 11:49:46  houghton
// Add dumpInfo.
//
// Revision 2.5  1997/03/03 14:32:42  houghton
// Added virtual destructor.
//
// Revision 2.4  1996/11/06 18:07:27  houghton
// Renamed StlUtils.hh to StlUtilsUtils.hh.
//
// Revision 2.3  1996/03/04 11:38:38  houghton
// Bug Fix: set baseAddr to base after remap in freeMem
// Bug Fix: if freeRecs == 0 do not decrement.
//
// Revision 2.2  1996/02/29 19:09:57  houghton
// *** empty log message ***
//
// Revision 2.1  1995/11/10 12:42:30  houghton
// Change to Version 2
//
// Revision 1.6  1995/11/05  16:32:34  houghton
// Revised
//
// Revision 1.2  1995/03/02  16:35:35  houghton
// Linux ports & new Classes
//
// Revision 1.1  1995/02/13  16:08:50  houghton
// New Style Avl an memory management. Many New Classes
//
//
