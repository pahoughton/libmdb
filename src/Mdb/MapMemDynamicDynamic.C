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
#include <algorithm>
#include <iomanip>
#include <cstring>

#if defined( CLUE_DEBUG )
#include "MapMemDynamicDynamic.ii"
#endif

CLUE_VERSION(
  MapMemDynamicDynamic,
  "$Id$");

const char * MapMemDynamicDynamic::ErrorStrings[] =
{
  ": ok",
  ": ",
  ": Bad size requested",
  0
};

MapMemDynamicDynamic::MapMemDynamicDynamic(
  const char * 	    fileName,
  size_t	    minChunkSize,
  size_t	    allocSize,
  unsigned short    permMask
  
  )
  : MapMem( fileName,
	    0,
	    MM_DYNAMIC,
	    MMD_VERSION,
	    ( sizeof( MapDynamicDynamicInfo ) +
	      DwordAlign( max( allocSize, (size_t)getpagesize() ) ) ),
	    permMask ),
  refCount( 0 )
{
  base = (MapDynamicDynamicInfo *)MapMem::getMapInfo();
  
  nextFree = 0;
  
  if( base != 0 && MapMem::good() )
    {
      base->minChunkSize    = DwordAlign( max( minChunkSize + sizeof( size_t ),
					       sizeof( FreeList ) ));
      base->allocSize	    = DwordAlign( max( allocSize,
					       (size_t)getpagesize() ) );
      base->chunkCount	    = 0;
      base->chunkSize	    = 0;
      base->freeCount	    = 1;
      base->freeSize	    = ( getMapSize() -
				DwordAlign( sizeof( MapDynamicDynamicInfo ) ));
      
      memset( base->keys, 0, sizeof( base->keys ) );      

      base->freeList.next = DwordAlign( sizeof( MapDynamicDynamicInfo )  );
      base->freeList.prev = base->freeList.next;
      base->freeList.size = 0;

      getFreeNode( base->freeList.next )->size = base->freeSize;
      getFreeNode( base->freeList.next )->next = 0;
      getFreeNode( base->freeList.next )->prev = 0;
      
      errorNum = E_OK;
    }
  else
    {
      errorNum = E_MAPMEM;
    }
}

MapMemDynamicDynamic::MapMemDynamicDynamic(
  const char *	    fileName,
  ios::open_mode    mode
  )
  : MapMem( fileName, MM_DYNAMIC, MMD_VERSION, mode ),
    refCount( 0 )
{
  base = (MapDynamicDynamicInfo *)MapMem::getMapInfo();
    
  if( base == 0 || ! MapMem::good() )
    errorNum = E_MAPMEM;
  else
    errorNum = E_OK;
}

MapMemDynamicDynamic::~MapMemDynamicDynamic( void )
{
}

off_t
MapMemDynamicDynamic::getMem( size_t size )
{
  if( ! good() )
    return( 0 );

  size_t    chunkSize = max( DwordAlign( size + sizeof( size_t ) ),
			     base->minChunkSize );

  if( base->freeList.next == 0 )
    expand( chunkSize );

  base->chunkCount++;
  while( good() )
    {
      for( off_t f = base->freeList.next;
	   f;
	   f = getFreeNode( f )->next )
	{
	  FreeList * fnode = getFreeNode( f );

	  if( fnode->size >= chunkSize )
	    {
	      if( fnode->size >= (chunkSize + base->minChunkSize) )
		{
		  // give part of this chunk
		  FreeList * newFnode = getFreeNode( f + chunkSize );
		  
		  setPrevFnodeNext( f, f + chunkSize );
		  setNextFnodePrev( f, f + chunkSize );
		  
		  newFnode->next = fnode->next;
		  newFnode->prev = fnode->prev;
		  newFnode->size = fnode->size - chunkSize;
		  
		  // set this chunk's size
		  setNodeSize( f, chunkSize );

		  base->chunkSize += chunkSize;
		  base->freeSize -= chunkSize;

#if defined( CLUE1_DEBUG )
		  _LLg( LogLevel::App1 )
		    << "getMem (partial): "
		    << setw( 6 ) << f + sizeof( size_t )
		    << setw(6) << getNodeSize( f )
		    << endl;
#endif
		  return( f + sizeof( size_t ) );
		}
	      else
		{
		  setPrevFnodeNext( f, fnode->next );
		  setNextFnodePrev( f, fnode->prev );

		  setNodeSize( f, fnode->size);
		  
		  base->chunkSize += fnode->size;
		  base->freeSize -= fnode->size;
		  base->freeCount --;

#if defined( CLUE1_DEBUG )
		  _LLg( LogLevel::App1 )
		    << "getMem (node):    "
		    << setw( 6 ) << f + sizeof( size_t )
		    << setw(6) << getNodeSize( f )
		    << endl;
#endif
		  return( f + sizeof( size_t ) );
		}
	    }
	}

      // i didn't find a large enough free node
      expand( chunkSize );
    }

  return( 0 );
}

void
MapMemDynamicDynamic::freeMem( off_t offset )
{
  if( ! good() )
    return;

  off_t	    node	= offset - sizeof( size_t );
  off_t     nodeSize	= getNodeSize( node );
  
  base->chunkCount--;
  base->chunkSize -= nodeSize;
  
  if( base->freeList.next == 0 )
    {
#if defined( CLUE1_DEBUG )
      _LLg( LogLevel::App1 )
	<< "Free (empty):     "
	<< setw( 6 ) << offset
	<< setw(6) << nodeSize
	<< endl;
#endif
      
      base->freeList.next = node;
      base->freeList.prev = node;
      
      getFreeNode( node )->next = 0;
      getFreeNode( node )->prev = 0;
      getFreeNode( node )->size = nodeSize;
    }
  else
    {
      // find loc in insert this node into

      off_t nextNode  = base->freeList.next;

      for( ; nextNode < node && getFreeNode( nextNode )->next != 0 ;
	   nextNode = getFreeNode( nextNode )->next );

      if( nextNode > node )
	{	  
	  off_t	    nPrevNode = getFreeNode( nextNode )->prev;

	  if( nPrevNode )
	    { 		
	      off_t    nPrevSize = getFreeNode( nPrevNode )->size;

	      if( (nPrevNode + nPrevSize) == node )
		{
		  if( (node + nodeSize) == nextNode )
		    {
		      // contigious nprev -> node -> next
#if defined( CLUE1_DEBUG )
		      _LLg( LogLevel::App1 )
			<< "Free (p n x):     "
			<< setw( 6 ) << offset
			<< setw(6) << nodeSize
			<< endl;
		      if( getFreeNode( nPrevNode )->next != nextNode )
			{
			  _LLg( LogLevel::Error ) <<
			    "In free( p n x ): p->n != n\n" << endl;
			  exit( 1 );
			}
#endif

		      getFreeNode( nPrevNode )->size +=
			getFreeNode( nextNode )->size + nodeSize;

		      setPrevFnodeNext( nextNode,
					getFreeNode( nextNode )->next );

		      setNextFnodePrev( nextNode,
					getFreeNode( nextNode )->prev );
		      
		      node = 0;
		      base->freeCount--;
		    }
		  else
		    {
#if defined( CLUE1_DEBUG )
		      _LLg( LogLevel::App1 )
			<< "Free (p n):       "
			<< setw( 6 ) << offset
			<< setw(6) << nodeSize
			<< endl;
#endif
		      // contigious nprev -> node
		      getFreeNode( nPrevNode )->size += nodeSize;
		      node = 0;
		    }
		}
	    }

	  if( node )
	    {
	      if( (node + nodeSize) == nextNode )
		{
		  // contigious node -> next
#if defined( CLUE1_DEBUG )
		      _LLg( LogLevel::App1 )
			<< "Free (n x):       "
			<< setw( 6 ) << offset
			<< setw(6) << nodeSize
			<< endl;
#endif
		      
		  getFreeNode( node )->next = getFreeNode( nextNode )->next;
		  getFreeNode( node )->prev = getFreeNode( nextNode )->prev;
		  getFreeNode( node )->size = ( getFreeNode( nextNode )->size +
						nodeSize );
		  
		  setNextFnodePrev( node, node );
		  setPrevFnodeNext( node, node );		  
		}
	      else
		{
#if defined( CLUE1_DEBUG )
		  _LLg( LogLevel::App1 )
		    << "Free (n):         "
		    << setw( 6 ) << offset
		    << setw(6) << nodeSize
		    << endl;
#endif
		  // non contigous
		  getFreeNode( node )->next = nextNode;
		  getFreeNode( node )->prev = getFreeNode( nextNode )->prev;
		  
		  setPrevFnodeNext( node, node );
		  setNextFnodePrev( node, node );
		  
		  base->freeCount++;
		}
	    }
	}
      else
	{
	  // nextNode->next must be 0, so freeNode is now
	  // the last in the list
	  
	  if( (off_t)(nextNode + getFreeNode( nextNode )->size ) ==  node )
	    {
#if defined( CLUE1_DEBUG )
	      _LLg( LogLevel::App1 )
		<< "Free (x l):       "
		<< setw( 6 ) << offset
		<< setw(6) << nodeSize
		<< endl;
#endif
	      // contigious nextNode -> node
	      getFreeNode( nextNode )->size += nodeSize;
	    }
	  else
	    {
	      // non contigious

#if defined( CLUE1_DEBUG )
	      _LLg( LogLevel::App1 )
		<< "Free (l):         "
		<< setw( 6 ) << offset
		<< setw(6) << nodeSize
		<< endl;
#endif
		      
	      getFreeNode( nextNode )->next = node;
	  
	      getFreeNode( node )->prev = nextNode;
	      getFreeNode( node )->next = 0;
	      getFreeNode( node )->size = nodeSize;

	      base->freeList.prev = node;	  
	      base->freeCount++;
	    }
	}    
    }

  base->freeSize += nodeSize;

  {
    // now see if there is enough space at the end to shrink

    if( ( base->freeList.prev + getFreeNode( base->freeList.prev )->size )
	  == getMapSize() &&
	( getFreeNode( base->freeList.prev )->size > (base->allocSize * 2 ) ) )
      {
	// shrink by all but one alloc unit;
#if defined( CLUE1_DEBUG )
	_LLg( LogLevel::App1 ) << "SHRINK\n";
	
	_LLg( LogLevel::App2 )
	  << "SHRINK: pre: " << ( getFreeNode( base->freeList.prev )->size -
			     base->allocSize )
	  << '\n' << dump( " pre: " )
	  << '\n' ;
	
	if( _LibLog->willOutput( LogLevel::App2 ) )
	  dumpFreeList( *_LibLog ) << endl;
#endif
	  
	size_t origSize = getMapSize();
	
	size_t newSize =
	  shrink( ( getFreeNode( base->freeList.prev )->size -
		    base->allocSize ),
		  (caddr_t)(base->base) );

	base = (MapDynamicDynamicInfo *)MapMem::getMapInfo();
  
	base->size = getSize();
	
	base->freeSize -= (origSize - newSize);
		
	getFreeNode( base->freeList.prev )->size -= (origSize - newSize);

#if defined( CLUE1_DEBUG )
	_LLg( LogLevel::App2 )
	  << "SHRINK: post: " << ( getFreeNode( base->freeList.prev )->size -
			     base->allocSize )
	  << '\n' << dump( " pos: " )
	  << '\n' ;
	if( _LibLog->willOutput( LogLevel::App2 ) )
	  dumpFreeList( *_LibLog ) << endl;
#endif
      }
  }
}

void
MapMemDynamicDynamic::expand( size_t minSize )
{
  if( ! good() )
    return;

#if defined( CLUE1_DEBUG )
  _LLg( LogLevel::App1 ) << "EXPAND\n";
  _LLg( LogLevel::App2 )
    << "EXPAND: pre: " << minSize
    << '\n' << dump( " pre: " )
    << '\n' ;
#endif
  
  if( _LibLog->willOutput( LogLevel::App2 ) )
    dumpFreeList( *_LibLog ) << endl;
  
  size_t amount = max( minSize, (size_t)(base->allocSize ));

  size_t origSize = getMapSize();
  
  if( grow( amount, (caddr_t)base->base ) == 0 )
    {
      errorNum = E_MAPMEM;
      return;
    }

  base = (MapDynamicDynamicInfo *)MapMem::getMapInfo();
  
  base->size = getSize();

  // now add these recs to the free list

  if( ( base->freeList.prev + getFreeNode( base->freeList.prev )->size )
      == origSize )
    {       
      getFreeNode( base->freeList.prev )->size += getMapSize() - origSize;
    }
  else
    {
#if defined( CLUE1_DEBUG )
      _LLg( LogLevel::App2 )
	<< "EXPAND: post 1: "
	<< '\n' << dump( " post: " )
	<< '\n' << "  ORIG: " << origSize <<  '\n';
#endif
      if( _LibLog->willOutput( LogLevel::App2 ) )
	dumpNodes( *_LibLog ) << endl;

      getFreeNode( origSize )->next = 0;
      getFreeNode( origSize )->prev = base->freeList.prev;
      getFreeNode( origSize )->size = getMapSize() - origSize;

      if( base->freeList.prev )
	{
	  getFreeNode( base->freeList.prev )->next = origSize;
	  base->freeList.prev = origSize;
	}
      else
	{
	  base->freeList.next = origSize;
	  base->freeList.prev = origSize;
	}
#if defined( CLUE1_DEBUG )
      _LLg( LogLevel::App2 )
	<< "EXPAND: post 2: "
	<< '\n' << dump( " post: " )
	<< '\n' << "  ORIG: " << origSize <<  '\n';
      
      if( _LibLog->willOutput( LogLevel::App2 ) )
	dumpNodes( *_LibLog ) << endl;
#endif
    }
  base->freeSize += getMapSize() - origSize;

#if defined( CLUE1_DEBUG )
  _LLg( LogLevel::App2 )
    << "EXPAND: post 3: "
    << '\n' << dump( " post: " )
    << '\n' << "  ORIG: " << origSize <<  '\n';
#endif
}


      
	      
      
bool
MapMemDynamicDynamic::good( void ) const
{
  return( base != 0 &&
	  errorNum == E_OK &&
	  MapMem::good() );
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
      size_t eSize = errStr.size();

      if( errorNum > E_MAPMEM &&
	  errorNum < E_UNDEFINED )
	errStr << ErrorStrings[ errorNum ];

      if( ! MapMem::good() )
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
  pre << MapMem::getClassName() << "::";

  MapMem::dumpInfo( dest, pre, false );

  dest << prefix << "refCount:     " << refCount << '\n';
  
  if( base )
    {
      dest << prefix << "minChunkSize: " << base->minChunkSize << '\n'
	   << prefix << "allocSize:    " << base->allocSize << '\n'
	   << prefix << "chunkCount:   " << base->chunkCount << '\n'
	   << prefix << "chunkSize:    " << base->chunkSize << '\n'
	   << prefix << "freeCount:    " << base->freeCount << '\n'
	   << prefix << "freeSize:     " << base->freeSize << '\n'
	   << prefix << "freeNext:     " << base->freeList.next << '\n'
	   << prefix << "freePrev:     " << base->freeList.prev << '\n'
	;
      
      for( int k = 0; k < NUM_KEYS; k++ )
	{
	  if( getKey( k ) != 0 )
	    {
	      dest << prefix
		   << "key(" << setw(2) << k << "):      " << getKey(k) << '\n'
		;
	    }
	}
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
  
  dest << "FreeList Dump: count: " << base->freeCount
       << " size: " << base->freeSize
       << " first: " << base->freeList.next
       << " last: " << base->freeList.prev << '\n' ;
  dest << "FreeList Nodes:    node    size    prev    next\n";
  
  for( off_t f = base->freeList.next;
       f;
       f = getFreeNode( f )->next )
    {
      
      dest << "Free Node:       "
	   << setw(6) << f
	   << setw(8) << getFreeNode( f )->size
	   << setw(8) << getFreeNode( f )->prev
	   << setw(8) << getFreeNode( f )->next
	   << '\n'
	;
    }
  return( dest );
}

ostream &
MapMemDynamicDynamic::dumpNodes( ostream & dest ) const
{
  dest << "Node List:         node    size    prev    next\n";
  
  off_t  node = DwordAlign( sizeof( MapDynamicDynamicInfo )  );
  
  for( off_t f = base->freeList.next;
       f;
       f = getFreeNode( f )->next )
    {
      for( ; node < f && node < (off_t)getMapSize();
	   node += getNodeSize( node ) )
	{
	  
	  dest << "Node:            "
	       << setw(6) << node
	       << setw(8) << getNodeSize( node )
	       << endl;
	    ;
	  if( getNodeSize( node ) == 0 )
	    return( dest );
	}
      
      node += getFreeNode( f )->size;
	  
      dest << "Free Node:       "
	   << setw(6) << f
	   << setw(8) << getFreeNode( f )->size
	   << setw(8) << getFreeNode( f )->prev
	   << setw(8) << getFreeNode( f )->next
	   << endl;
	;
    }

  for( ; node < (off_t)getMapSize();
       node += getNodeSize( node ) )
    {
      
      dest << "Node:            "
	   << setw(6) << node
	   << setw(8) << getNodeSize( node )
	   << endl;
	;
      if( getNodeSize( node ) == 0 )
	return( dest );
    }
  
  return( dest );
}
  
// Revision Log:
//
// $Log$
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
