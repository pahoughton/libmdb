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
#include <ClueUtils.hh>
#include <Str.hh>
#include <iomanip>
#include <cstring>

#if defined( MDB_DEBUG )
#include "MapMemDynamicFixed.ii"
#endif


MDB_VERSION(
  MapMemDynamicFixed,
  "$Id$");

//
// allocation chunks must be at least 1 page
//

const char *
MapMemDynamicFixed::ErrorStrings[] =
  {
    ": ok",
    ": ",
    ": Bad size requested",
    0
  };

MapMemDynamicFixed::MapMemDynamicFixed(
  const char *	    fileName,
  ios::open_mode    mode,
  bool		    create,
  size_type	    recSize,
  size_type	    numRecs,
  MapMask	    permMask
  )
  : MapMemDynamic( fileName,
		   MDF_VERSION,
		   mode,
		   create,
		   ( sizeof( MapDynamicFixedInfo ) +
		     ( recSize * numRecs > (size_type)getpagesize()  ?
		       (recSize * numRecs) :
		       ((getpagesize() / recSize) + 1) * recSize ) ),
		   permMask )
{
  if( create )
    createMapMemDynamicFixed( recSize, numRecs);
  else
    openMapMemDynamicFixed();
}

  
MapMemDynamicFixed::MapMemDynamicFixed(
  const char * 	    fileName,
  size_type	    recSize,
  size_type	    numRecs,
  MapMask	    permMask
  )
  : MapMemDynamic( fileName,
		   MDF_VERSION,
		   ( sizeof( MapDynamicFixedInfo ) +
		     ( recSize * numRecs > (size_type)getpagesize()  ?
		       (recSize * numRecs) :
		       ((getpagesize() / recSize) + 1) * recSize ) ),
		   permMask )
{
  createMapMemDynamicFixed( recSize, numRecs );
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
  if( mapInfo() && mapInfo()->owner == getpid() )
    mapInfo()->owner = 0;
}

off_t
MapMemDynamicFixed::allocate( size_type size )
{
  if( ! good() ) return( 0 );
  
  //
  // we can only allocate 1 size of records
  //

  if( size != 0 &&  size > mapInfo()->recSize )
    {
      errorNum = E_BADSIZE;
      return( 0 );
    }

  if( mapInfo()->freeList.next == 0 )
    {      
      expand();

      if( mapInfo()->freeList.next == 0 )
	{
	  return(0);
	}
    }

  off_t     	    baseAddr = (off_t)mapInfo();
  off_t     	    freeOffset = mapInfo()->freeList.next;
  struct FreeList * freeNode = (struct FreeList *)(baseAddr + freeOffset);

  mapInfo()->freeList.next = freeNode->next;

  if( mapInfo()->freeList.next == 0 )
    {
      mapInfo()->freeList.prev = 0;
    }
  else
    {  
      struct FreeList * nextNode = (struct FreeList *)(baseAddr + freeNode->next);
  
      nextNode->prev = 0;
    }

  -- mapInfo()->freeCount;

  return( freeOffset );
}




//
// The free list should be kept in order from
// beginning of file to the end of the file.
// getMem always uses to first available record.
//
// This increases the chances of being able to shrink
// the map file when there are 2 * the chunksize free
// records at the end of the file.
//

void
MapMemDynamicFixed::release( Loc offset )
{

  if( ! good() ) return;
  
  off_t     baseAddr = (off_t)mapInfo();
  
  struct FreeList * freeNode = (struct FreeList *)(baseAddr + offset);


  ++ mapInfo()->freeCount;

  freeNode->next = 0;
  freeNode->prev = 0;

  if( mapInfo()->freeList.next == 0 )
    {
      mapInfo()->freeList.next = offset;
      mapInfo()->freeList.prev = offset;
    }
  else
    {
      struct FreeList * firstNode= (struct FreeList *)(baseAddr +
						       mapInfo()->freeList.next);

      struct FreeList * nextNode = firstNode ;
      for( ;
	   nextNode < freeNode && nextNode->next != 0;
	   nextNode = (struct FreeList *)((off_t)baseAddr + nextNode->next) );

      if( nextNode > freeNode )
	{
	  freeNode->next = (off_t)nextNode - baseAddr;
	  freeNode->prev = nextNode->prev;
	  nextNode->prev = (off_t)freeNode - baseAddr;
	  
	  if( nextNode == firstNode )
	    {
	      mapInfo()->freeList.next = offset;
	    }
	  else
	    {
	      struct FreeList * prevNode = (struct FreeList *)( baseAddr +
								freeNode->prev );

	      prevNode->next = (off_t)freeNode - baseAddr;
	    }
	}
      else
	{
	  // nextNode->next must be 0, so freeNode is now
	  // the last in the list

	  nextNode->next = (off_t)freeNode - baseAddr;
	  freeNode->prev = (off_t)nextNode - baseAddr;

	  mapInfo()->freeList.prev = (off_t)freeNode - baseAddr;
	}
    }

  if( mapInfo()->freeCount > (mapInfo()->chunkSize * 2) )
    {
      //
      // see if there are enough contigious free recs at the
      // end to release a chunk
      //

      // this is the offset of the last known record
      
      off_t 	endOffset;

      endOffset = ( DwordAlign( sizeof( MapDynamicFixedInfo ) ) +
		     ( (mapInfo()->chunkCount - 1)* mapInfo()->recSize ) );
		     
      //
      // is the last rec free
      //

      if( mapInfo()->freeList.prev == (unsigned long)endOffset )
	{
	  unsigned long freeRecs = 0;
	  
	  struct FreeList * endNode = (struct FreeList *)(baseAddr + endOffset);
	  
	  for( ; endNode->prev == endOffset - mapInfo()->recSize;
		 endOffset = endNode->prev,
		 endNode = (struct FreeList *)(baseAddr + endOffset) )
	    {
	      freeRecs++;
	    }

	  if( freeRecs > 0 )
	    freeRecs --;
	  
	  if( freeRecs > mapInfo()->chunkSize )
	    {
	      unsigned long releaseChunks = freeRecs / mapInfo()->chunkSize;

	      if( releaseChunks )
		{
		  unsigned long releaseRecs = releaseChunks * mapInfo()->chunkSize;

		  size_t newSize = shrink( releaseRecs * mapInfo()->recSize,
					   (caddr_t)mapInfo()->base  );

		  if( ! newSize )
		    return;
		  
		  baseAddr = (off_t)mapInfo();

		  releaseRecs++;
		  
		  mapInfo()->size = getSize();
		  mapInfo()->freeCount -= releaseRecs;
		  mapInfo()->chunkCount -= releaseRecs;
		  mapInfo()->freeList.prev = ( DwordAlign( sizeof( MapDynamicFixedInfo ) ) +
					  ( (mapInfo()->chunkCount - 1)* mapInfo()->recSize ) );

		  struct FreeList * lastNode = (struct FreeList *)
		    (baseAddr + mapInfo()->freeList.prev);
		  lastNode->next = 0;
		  
		}
	    }
	}
    }
}
		
bool
MapMemDynamicFixed::valid( off_t offset ) const
{
  off_t   first = DwordAlign( sizeof( MapDynamicFixedInfo ) );

  if( (offset - first) % mapInfo()->recSize )
    return( false );
  
  off_t     	baseAddr = (off_t)mapInfo();
  off_t     	freeOffset;
      
  for( freeOffset = mapInfo()->freeList.next;
       freeOffset && freeOffset < offset;
       freeOffset = ((FreeList *)(baseAddr + freeOffset))->next );

  return( freeOffset == offset ? false : true );
}


void
MapMemDynamicFixed::expand( void )
{

  if( ! good() ) return;

  unsigned long amount = mapInfo()->chunkSize * mapInfo()->recSize;
  
  if( grow( amount, (caddr_t)mapInfo()->base ) == 0 )
    {
      errorNum = E_MAPMEM;
      return;
    }

  mapInfo()->size = getSize();

  
  //
  // now add these recs to the free list;
  //
      
  off_t     baseAddr = (unsigned long) mapInfo();
  off_t     freeAddr = 0;
  off_t	    prevFreeOffset = 0;
  struct FreeList * freeNode = 0;

  freeAddr = baseAddr + ( DwordAlign( sizeof( MapDynamicFixedInfo )  ) +
			  (mapInfo()->recSize * (mapInfo()->chunkCount) ) );

  if( mapInfo()->freeList.next == 0 )
    {
      mapInfo()->freeList.next = freeAddr - baseAddr;
      prevFreeOffset = 0;
    }
  else
    {
      freeNode = (FreeList * ) baseAddr + mapInfo()->freeList.prev;
      freeNode->next = freeAddr - baseAddr;
      
      prevFreeOffset = mapInfo()->freeList.prev;
    }
      
  for( ;
	freeAddr + (off_t)mapInfo()->recSize < (off_t)getEnd();
	freeAddr += mapInfo()->recSize )
    {
      freeNode = (FreeList *)freeAddr;
      
      freeNode->next    = ( freeAddr - baseAddr ) + mapInfo()->recSize;
      freeNode->prev    = prevFreeOffset;
      prevFreeOffset    = freeAddr - baseAddr ;
      
      ++ mapInfo()->freeCount;
      ++ mapInfo()->chunkCount;
    }

  // one to many, backup one

  freeNode = (FreeList *)(baseAddr + prevFreeOffset);
  
  freeNode->next = 0;

  mapInfo()->freeList.prev = prevFreeOffset;
  
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
      int errCnt = 0;
      
      if( errorNum > E_MAPMEM &&
	  errorNum < E_UNDEFINED )
	{
	  errCnt++;
	  errStr << ErrorStrings[ errorNum ];
	}

      if( ! MapMemDynamic::good() )
	{
	  errCnt++;
	  errStr << ": " << MapMemDynamic::error();
	}

      if( ! errCnt )
	{
	  errStr << ": unknown error";
	}
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
	   << prefix << "chunk size:   " << getChunkSize() << '\n'
	   << prefix << "free first:   " << mapInfo()->freeList.next << '\n'
	   << prefix << "free last:    " << mapInfo()->freeList.prev << '\n'
	;
      
    }
  else
    {
      dest << prefix << "No Base Addr!\n";
    }

  return( dest );
}

void
MapMemDynamicFixed::createMapMemDynamicFixed(
  size_type recSize,
  size_type numRecs
  )
{
  if( mapInfo() != 0 && MapMemDynamic::good() )
    {
      mapInfo()->recSize =
	DwordAlign( max( recSize, (size_type)sizeof( struct FreeList ) ) );

      if( mapInfo()->recSize * numRecs > getPageSize() )
	mapInfo()->chunkSize = numRecs;
      else
	mapInfo()->chunkSize = (getPageSize() / mapInfo()->recSize) + 1;
      
            
      //
      // Initialize the list of avaliable (deleted) records
      //
      
      off_t  baseAddr = (off_t) mapInfo();

      mapInfo()->freeList.next = DwordAlign( sizeof( MapDynamicFixedInfo ) );
      mapInfo()->freeCount = 0;
      
      off_t 	    	freeAddr = baseAddr + mapInfo()->freeList.next;
      off_t 	    	prevFreeOffset = 0;
      struct FreeList * freeNode = 0;
      
      
      for( ;
	  (off_t)(freeAddr + mapInfo()->recSize) < (off_t)getEnd();
	  freeAddr += mapInfo()->recSize )
	{
	  freeNode = (struct FreeList *)freeAddr;

	  freeNode->next    = ( freeAddr - baseAddr ) + mapInfo()->recSize;
	  freeNode->prev    = prevFreeOffset;
	  prevFreeOffset    = ( freeAddr - baseAddr );
	  
	  mapInfo()->freeCount++;
	}

      // one to many, backup one

      freeNode = (struct FreeList *)(baseAddr + prevFreeOffset);
  
      freeNode->next = 0;
      mapInfo()->freeList.prev = prevFreeOffset;

      mapInfo()->chunkCount = mapInfo()->freeCount;
      
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
// Renamed Clue.hh to ClueUtils.hh.
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
