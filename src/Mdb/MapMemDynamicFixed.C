//
// File:        MapMemFixedDynamic.C
// Desc:        
//              
//
// Author:      Paul Houghton - (houghton@cworld.wiltel.com)
// Created:     11/28/94 03:53 
//
// Revision History:
//
// $Log$
// Revision 1.2  1995/03/02 16:35:35  houghton
// Linux ports & new Classes
//
// Revision 1.1  1995/02/13  16:08:50  houghton
// New Style Avl an memory management. Many New Classes
//
//
static const char * RcsId =
"$Id$";

#include "MapMemFixedDynamic.hh"
#include "Str.hh"

//
// allocation chunks must be at least 1 page
//

const char *
MapMemFixedDynamic::ErrorStrings[] =
  {
    ": Ok",
    ": ",
    ": Bad size requested",
    0
  };

MapMemFixedDynamic::MapMemFixedDynamic(
  const char * 	    fileName,
  size_t	    recSize,
  unsigned long	    numRecs,
  unsigned short    permMask
  
  )
  : MapMem( fileName,
	    0,
	    MM_FIXED,
	    MMF_VERSION,
	    sizeof( MapInfo ) + ( ( (recSize * numRecs) > getpagesize() ) ?
				  (recSize * numRecs ) :
				  ((getpagesize() / recSize) + 1) * recSize ),
	    permMask )
{
  base = (MapFixedDynamicInfo *)MapMem::getMapInfo();
  
  nextFreeRecOffset = 0;
  
  if( base != 0 && MapMem::good() )
    {      
      base->recSize = DWORD_ALIGN( max( recSize, sizeof( struct FreeList ) ) );

      if( base->recSize * numRecs > getPageSize() )
	{
	  base->chunkSize = numRecs;
	}
      else
	{
	  base->chunkSize = (getPageSize() / base->recSize) + 1;
	}
            
      memset( base->keys, 0, sizeof( base->keys ) );      


      //
      // Initialize the list of avaliable (deleted) records
      //
      
      off_t  baseAddr = (off_t) base;

      base->freeList.next = DWORD_ALIGN( sizeof( MapFixedDynamicInfo )  );      
      base->freeCount = 0;
      
      off_t 	    	freeAddr = baseAddr + base->freeList.next;
      off_t 	    	prevFreeOffset = 0;
      struct FreeList * freeNode = 0;
      
      
      for( ;
	  freeAddr + base->recSize < (off_t)getEnd();
	  freeAddr += base->recSize )
	{
	  freeNode = (struct FreeList *)freeAddr;

	  freeNode->next    = ( freeAddr - baseAddr ) + base->recSize;
	  freeNode->prev    = prevFreeOffset;
	  prevFreeOffset    = ( freeAddr - baseAddr );
	  
	  base->freeCount++;
	}

      // one to many, backup one

      freeNode = (struct FreeList *)(baseAddr + prevFreeOffset);
  
      freeNode->next = 0;
      base->freeList.prev = prevFreeOffset;

      
      base->recCount = base->freeCount;

      mapFixedDynamicError = E_OK;
    }
  else
    {
      mapFixedDynamicError = E_MAPMEM;
    }
}


off_t
MapMemFixedDynamic::getMem( size_t size )
{
  if( ! good() ) return( 0 );
  
  //
  // we can only allocate 1 size of records
  //

  if( size != 0 &&  size > base->recSize )
    {
      mapFixedDynamicError = E_BADSIZE;
      return( 0 );
    }

  if( base->freeList.next == 0 )
    {      
      expand();

      if( base->freeList.next == 0 )
	{
	  return(0);
	}
    }

  off_t     	    baseAddr = (off_t)base;
  off_t     	    freeOffset = base->freeList.next;
  struct FreeList * freeNode = (struct FreeList *)(baseAddr + freeOffset);

  base->freeList.next = freeNode->next;

  if( base->freeList.next == 0 )
    {
      base->freeList.prev = 0;
    }
  else
    {  
      struct FreeList * nextNode = (struct FreeList *)(baseAddr + freeNode->next);
  
      nextNode->prev = 0;
    }
  
  base->freeCount--;

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
MapMemFixedDynamic::freeMem(
  off_t	offset
  )
{

  if( ! good() ) return;
  
  off_t     baseAddr = (off_t)base;
  
  struct FreeList * freeNode = (struct FreeList *)(baseAddr + offset);


  base->freeCount++;

  freeNode->next = 0;
  freeNode->prev = 0;

  if( base->freeList.next == 0 )
    {
      base->freeList.next = offset;
      base->freeList.prev = offset;
    }
  else
    {
      struct FreeList * firstNode= (struct FreeList *)(baseAddr +
						       base->freeList.next);
      
      for( struct FreeList * nextNode = firstNode ;
	   nextNode < freeNode && nextNode->next != 0;
	   nextNode = (struct FreeList *)((off_t)baseAddr + nextNode->next) );

      if( nextNode > freeNode )
	{
	  freeNode->next = (off_t)nextNode - baseAddr;
	  freeNode->prev = nextNode->prev;
	  nextNode->prev = (off_t)freeNode - baseAddr;
	  
	  if( nextNode == firstNode )
	    {
	      base->freeList.next = offset;
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

	  base->freeList.prev = (off_t)freeNode - baseAddr;
	}
    }

  if( base->freeCount > (base->chunkSize * 2) )
    {
      //
      // see if there are enough contigious free recs at the
      // end to release a chunk
      //

      // this is the offset of the last known record
      
      off_t 	endOffset;

      endOffset = ( DWORD_ALIGN( sizeof( MapFixedDynamicInfo ) ) +
		     ( (base->recCount - 1)* base->recSize ) );
		     
      //
      // is the last rec free
      //

      if( base->freeList.prev == endOffset )
	{
	  unsigned long freeRecs = 0;
	  
	  struct FreeList * endNode = (struct FreeList *)(baseAddr + endOffset);
	  
	  for( ; endNode->prev == endOffset - base->recSize;
		 endOffset = endNode->prev,
		 endNode = (struct FreeList *)(baseAddr + endOffset) )
	    {
	      freeRecs++;
	    }

	  if( freeRecs > base->chunkSize )
	    {
	      unsigned long releaseChunks = freeRecs / base->chunkSize;

	      if( releaseChunks > 1 )
		{
		  unsigned long releaseRecs = (releaseChunks - 1) * base->chunkSize;

		  shrink( releaseRecs * base->recSize, (caddr_t)base->base  );

		  base = (MapFixedDynamicInfo *)MapMem::getMapInfo();
		  
		  base->freeCount -= releaseRecs;
		  base->recCount -= releaseRecs;
		}
	    }
	}
    }
}
		

void
MapMemFixedDynamic::expand( void )
{

  if( ! good() ) return;

  unsigned long amount = base->chunkSize * base->recSize;
  
  if( grow( amount, (caddr_t)base->base ) == 0 )
    {
      mapFixedDynamicError = E_MAPMEM;
      return;
    }

  base = (MapFixedDynamicInfo *)MapMem::getMapInfo();
  
  base->size = getSize();

  
  //
  // now add these recs to the free list;
  //
      
  off_t     baseAddr = (unsigned long) base;
  off_t     freeAddr = 0;
  off_t	    prevFreeOffset = 0;
  struct FreeList * freeNode = 0;

  freeAddr = baseAddr + ( DWORD_ALIGN( sizeof( MapFixedDynamicInfo )  ) +
			  (base->recSize * (base->recCount) ) );

  if( base->freeList.next == 0 )
    {
      base->freeList.next = freeAddr - baseAddr;			          
      prevFreeOffset = 0;
    }
  else
    {
      freeNode = (FreeList * ) baseAddr + base->freeList.prev;
      freeNode->next = freeAddr - baseAddr;
      
      prevFreeOffset = base->freeList.prev;
    }
      
  for( ;
	freeAddr + base->recSize < (off_t)getEnd();
	freeAddr += base->recSize )
    {
      freeNode = (FreeList *)freeAddr;
      
      freeNode->next    = ( freeAddr - baseAddr ) + base->recSize;
      freeNode->prev    = prevFreeOffset;
      prevFreeOffset    = freeAddr - baseAddr ;
      
      base->freeCount++;
      base->recCount++;
    }

  // one to many, backup one

  freeNode = (FreeList *)(baseAddr + prevFreeOffset);
  
  freeNode->next = 0;

  base->freeList.prev = prevFreeOffset;
  
}

RecNumber
MapMemFixedDynamic::first( void )
{
  if( base && base->freeCount != base->recCount )
    {
      off_t   first = DWORD_ALIGN( sizeof( MapFixedDynamicInfo ) );

      off_t     	baseAddr = (off_t)base;
      off_t     	freeOffset;
      
      for( freeOffset = base->freeList.next;
	   first == freeOffset;
	   first += base->recSize,
	   freeOffset = ((FreeList *)(baseAddr + freeOffset))->next );

      nextFreeRecOffset = freeOffset;
      
      return( offset2RecNum( first ) );
    }
  return( 0 );  
}
	  
Bool
MapMemFixedDynamic::next( RecNumber & rec )
{
  
  if( rec == 0 )
    {
      rec = first();
      return( rec != 0 );
    }
  else
    {
      off_t nextRec = recNum2Offset( rec );

      nextRec += base->recSize;

      if( nextRec > getEnd() - getBase() )
	{
	  return( FALSE );
	}
      off_t 	baseAddr = (off_t)base;
      
      for( ; nextFreeRecOffset && nextRec > nextFreeRecOffset;
	     nextFreeRecOffset =
	       ((FreeList *)(baseAddr + nextFreeRecOffset))->next );

      for( ; nextFreeRecOffset &&
	       nextRec == nextFreeRecOffset;
	     nextRec += base->recSize,
	       nextFreeRecOffset =
	       ((FreeList*)(baseAddr + nextFreeRecOffset))->next );

      if( nextFreeRecOffset == 0 )
	{
	  return( FALSE );
	}
      else
	{
	  rec = offset2RecNum( nextRec );
	  return( TRUE );
	}
    }
}

const char *
MapMemFixedDynamic::getClassName( void ) const
{
  return( "MapMemFixedDynamic" );
}

Bool
MapMemFixedDynamic::good( void ) const
{
  return( base != 0 &&
	  mapFixedDynamicError == E_OK &&
	  MapMem::good() );
}

const char *
MapMemFixedDynamic::error( void ) const
{
  static Str errStr;
  errStr.reset();

  errStr << MapMemFixedDynamic::getClassName();
  
  if( good() )
    {
      errStr << ": Ok";
    }
  else
    {
      int errCnt = 0;
      
      if( mapFixedDynamicError > E_MAPMEM &&
	  mapFixedDynamicError < E_UNDEFINED )
	{
	  errCnt++;
	  errStr << ErrorStrings[ mapFixedDynamicError ];
	}

      if( ! MapMem::good() )
	{
	  errCnt++;
	  errStr << ": " << MapMem::error();
	}

      if( ! errCnt )
	{
	  errStr << ": unknown error";
	}
    }
  return( errStr.cstr() );
}

ostream &
MapMemFixedDynamic::getStats( ostream & dest ) const
{
  dest << MapMemFixedDynamic::getClassName() << ": stats" << '\n'
       << "    Status:       " << error() << '\n'
       << "    Rec size:     " << getRecSize() << '\n'
       << "    Chunk size:   " << getChunkSize() << '\n'
       << "    Rec count:    " << getRecCount() << '\n'
       << "    Free count:   " << getFreeRecCount() 
    ;

  for( int k = 0; k < NUM_KEYS; k++ )
    {
      if( getKey( k ) != 0 )
	{
	  dest << "\n    Key(" << k << ") = " << getKey(k) ;
	}
    }

  dest << endl;
  
  return( MapMem::getStats( dest ) );
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
