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
MapMemDynamicDynamic::allocate( size_type size )
{
  if( ! good() )
    return( 0 );

  size_type    chunkSize = max( DwordAlign( size + sizeof( size_type ) ),
			     mapInfo()->minChunkSize );

  if( mapInfo()->freeList.next == 0 )
    expand( chunkSize );

  mapInfo()->chunkCount++;
  while( good() )
    {
      for( off_t f = mapInfo()->freeList.next;
	   f;
	   f = getFreeNode( f )->next )
	{
	  FreeList * fnode = getFreeNode( f );

	  if( fnode->size >= chunkSize )
	    {
	      if( fnode->size >= (chunkSize + mapInfo()->minChunkSize) )
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

		  mapInfo()->chunkSize += chunkSize;
		  mapInfo()->freeSize -= chunkSize;

#if defined( MDB_DEBUG )
		  _LLg( LogLevel::App1 )
		    << "getMem (partial): "
		    << setw( 6 ) << f + sizeof( size_type )
		    << setw(6) << getNodeSize( f )
		    << endl;
#endif
		  return( f + sizeof( size_type ) );
		}
	      else
		{
		  setPrevFnodeNext( f, fnode->next );
		  setNextFnodePrev( f, fnode->prev );

		  setNodeSize( f, fnode->size);
		  
		  mapInfo()->chunkSize += fnode->size;
		  mapInfo()->freeSize -= fnode->size;
		  mapInfo()->freeCount --;

#if defined( MDB_DEBUG )
		  _LLg( LogLevel::App1 )
		    << "getMem (node):    "
		    << setw( 6 ) << f + sizeof( size_type )
		    << setw(6) << getNodeSize( f )
		    << endl;
#endif
		  return( f + sizeof( size_type ) );
		}
	    }
	}

      // i didn't find a large enough free node
      expand( chunkSize );
    }

  return( 0 );
}

void
MapMemDynamicDynamic::release( Loc offset )
{
  if( ! good() )
    return;

  off_t	    node	= offset - sizeof( size_type );
  off_t     nodeSize	= getNodeSize( node );
  
  mapInfo()->chunkCount--;
  mapInfo()->chunkSize -= nodeSize;
  
  if( mapInfo()->freeList.next == 0 )
    {
#if defined( MDB_DEBUG )
      _LLg( LogLevel::App1 )
	<< "Free (empty):     "
	<< setw( 6 ) << offset
	<< setw(6) << nodeSize
	<< endl;
#endif
      
      mapInfo()->freeList.next = node;
      mapInfo()->freeList.prev = node;
      
      getFreeNode( node )->next = 0;
      getFreeNode( node )->prev = 0;
      getFreeNode( node )->size = nodeSize;
    }
  else
    {
      // find loc to insert this node into

      off_t nextNode  = mapInfo()->freeList.next;

      for( ; nextNode < node && getFreeNode( nextNode )->next != 0 ;
	   nextNode = getFreeNode( nextNode )->next );

      if( nextNode == node )
	{
	  _LLg( LogLevel::Warn )
	    << "Trying to free a free node: "
	    << node << " ignored." << endl;
	  return;
	}
	  
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
#if defined( MDB_DEBUG )
		      _LLg( LogLevel::App1 )
			<< "Free (p n x):     "
			<< setw( 6 ) << offset
			<< setw(6) << nodeSize
			<< endl;
		      if( (off_t)getFreeNode( nPrevNode )->next != nextNode )
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
		      mapInfo()->freeCount--;
		    }
		  else
		    {
#if defined( MDB_DEBUG )
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
#if defined( MDB_DEBUG )
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
#if defined( MDB_DEBUG )
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
		  
		  mapInfo()->freeCount++;
		}
	    }
	}
      else
	{
	  // nextNode->next must be 0, so freeNode is now
	  // the last in the list
	  
	  if( (off_t)(nextNode + getFreeNode( nextNode )->size ) ==  node )
	    {
#if defined( MDB_DEBUG )
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

#if defined( MDB_DEBUG )
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

	      mapInfo()->freeList.prev = node;	  
	      mapInfo()->freeCount++;
	    }
	}    
    }

  mapInfo()->freeSize += nodeSize;

  {
    // now see if there is enough space at the end to shrink

    if( ( mapInfo()->freeList.prev +
	  getFreeNode( mapInfo()->freeList.prev )->size )
	  == getMapSize() &&
	( getFreeNode( mapInfo()->freeList.prev )->size >
	  (mapInfo()->allocSize * 2 ) ) )
      {
	// shrink by all but one alloc unit;
#if defined( MDB_DEBUG )
	_LLg( LogLevel::App1 ) << "SHRINK\n";
	
	_LLg( LogLevel::App2 )
	  << "SHRINK: pre: "
	  << ( getFreeNode( mapInfo()->freeList.prev )->size -
	       mapInfo()->allocSize )
	  << '\n' << dump( " pre: " )
	  << '\n' ;
	
	if( _LibLog->willOutput( LogLevel::App2 ) )
	  dumpFreeList( *_LibLog ) << endl;
#endif
	  
	size_type origSize = getMapSize();
	
	size_type newSize =
	  shrink( ( getFreeNode( mapInfo()->freeList.prev )->size -
		    mapInfo()->allocSize ),
		  (caddr_t)(mapInfo()->base) );

	mapInfo()->size = getSize();
	
	mapInfo()->freeSize -= (origSize - newSize);
		
	getFreeNode( mapInfo()->freeList.prev )->size -= (origSize - newSize);

#if defined( MDB_DEBUG )
	_LLg( LogLevel::App2 )
	  << "SHRINK: post: "
	  << ( getFreeNode( mapInfo()->freeList.prev )->size -
	       mapInfo()->allocSize )
	  << '\n' << dump( " pos: " )
	  << '\n' ;
	if( _LibLog->willOutput( LogLevel::App2 ) )
	  dumpFreeList( *_LibLog ) << endl;
#endif
      }
  }
}

void
MapMemDynamicDynamic::expand( size_type minSize )
{
  if( ! good() )
    return;

#if defined( MDB_DEBUG )
  _LLg( LogLevel::App1 ) << "EXPAND\n";
  _LLg( LogLevel::App2 )
    << "EXPAND: pre: " << minSize
    << '\n' << dump( " pre: " )
    << '\n' ;
#endif
  
  if( _LibLog && _LibLog->willOutput( LogLevel::App2 ) )
    dumpFreeList( *_LibLog ) << endl;
  
  size_type amount = max( minSize, (size_type)(mapInfo()->allocSize ));

  size_type origSize = getMapSize();
  
  if( grow( amount, (caddr_t)mapInfo()->base ) == 0 )
    {
      errorNum = E_MAPMEM;
      return;
    }

  mapInfo()->size = getSize();

  // now add these recs to the free list

  if( ( mapInfo()->freeList.prev +
	getFreeNode( mapInfo()->freeList.prev )->size )
      == origSize )
    {       
      getFreeNode( mapInfo()->freeList.prev )->size += getMapSize() - origSize;
    }
  else
    {
#if defined( MDB_DEBUG )
      _LLg( LogLevel::App2 )
	<< "EXPAND: post 1: "
	<< '\n' << dump( " post: " )
	<< '\n' << "  ORIG: " << origSize <<  '\n';
#endif
      if( _LibLog && _LibLog->willOutput( LogLevel::App2 ) )
	dumpNodes( *_LibLog ) << endl;

      getFreeNode( origSize )->next = 0;
      getFreeNode( origSize )->prev = mapInfo()->freeList.prev;
      getFreeNode( origSize )->size = getMapSize() - origSize;

      if( mapInfo()->freeList.prev )
	{
	  getFreeNode( mapInfo()->freeList.prev )->next = origSize;
	  mapInfo()->freeList.prev = origSize;
	}
      else
	{
	  mapInfo()->freeList.next = origSize;
	  mapInfo()->freeList.prev = origSize;
	}
#if defined( MDB_DEBUG )
      _LLg( LogLevel::App2 )
	<< "EXPAND: post 2: "
	<< '\n' << dump( " post: " )
	<< '\n' << "  ORIG: " << origSize <<  '\n';
      
      if( _LibLog && _LibLog->willOutput( LogLevel::App2 ) )
	dumpNodes( *_LibLog ) << endl;
#endif
    }
  mapInfo()->freeSize += getMapSize() - origSize;

#if defined( MDB_DEBUG )
  _LLg( LogLevel::App2 )
    << "EXPAND: post 3: "
    << '\n' << dump( " post: " )
    << '\n' << "  ORIG: " << origSize <<  '\n';
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
	   << prefix << "freeNext:     " << mapInfo()->freeList.next << '\n'
	   << prefix << "freePrev:     " << mapInfo()->freeList.prev << '\n'
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
       << " first: " << mapInfo()->freeList.next
       << " last: " << mapInfo()->freeList.prev << '\n' ;
  dest << "FreeList Nodes:    node    size    prev    next\n";
  
  for( off_t f = mapInfo()->freeList.next;
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
  
  for( off_t f = mapInfo()->freeList.next;
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
  
void
MapMemDynamicDynamic::createMapMemDynamicDynamic(
  size_type	minChunkSize,
  size_type	allocSize
  )
{
  if( mapInfo() != 0 && MapMemDynamic::good() )
    {
      mapInfo()->minChunkSize	=
	DwordAlign( max( (minChunkSize + sizeof( size_type) ),
			 (size_type)sizeof( FreeList) ) );
      mapInfo()->allocSize	= DwordAlign( max( allocSize,
						   (size_type)getpagesize()) );
      mapInfo()->chunkSize	= 0;
      mapInfo()->freeSize	= ( getMapSize() -
				    DwordAlign(
				      sizeof( MapDynamicDynamicInfo ) ));
      
      mapInfo()->freeList.next = DwordAlign( sizeof( MapDynamicDynamicInfo ) );
      mapInfo()->freeList.prev = mapInfo()->freeList.next;
      mapInfo()->freeList.size = 0;

      getFreeNode( mapInfo()->freeList.next )->size = mapInfo()->freeSize;
      getFreeNode( mapInfo()->freeList.next )->next = 0;
      getFreeNode( mapInfo()->freeList.next )->prev = 0;
      
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
