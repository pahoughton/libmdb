//
// File:        pRBSet.C
// Project:	Mdb
// Desc:        
//
//  Compiled sources for pRBSet
//  
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/11/97 04:42
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
#include "AppParam.hh"
#include "Perf.hh"
#include <MapMemDynamicFixed.hh>
#include <MapMemDynamicDynamic.hh>
#include <MultiMemOffsetMapDynamic.hh>
#include <RBSet.hh>
#include <vector>
#include <algorithm>


template< class Table, class Iterator >
inline
bool
pInsert(
  MapMemDynamic &   dataMap,
  Table &	    table,
  Iterator 	    first,
  Iterator	    last,
  const char *	    order,
  ostream &	    perfLog,
  size_t	    initAlloc,
  size_t	    quantity
  )
{
  TimeIt    timer;
  
  timer.start();
  
  for( ; first != last; ++ first )
    table.insert( *first );
  
  timer.stop();
    
  LogPerfData( perfLog,
	       dataMap.getClassName(),
	       table.getClassName(),
	       "insert",
	       order,
	       initAlloc,
	       table.getNodeSize(),
	       quantity,
	       timer );

  return( table.size() == quantity && table.good() );
}

template< class Table, class Iterator >
inline
bool
pErase(
  MapMemDynamic &   dataMap,
  Table &	    table,
  Iterator 	    first,
  Iterator	    last,
  const char *	    order,
  ostream &	    perfLog,
  size_t	    initAlloc,
  size_t	    quantity
  )
{
  TimeIt    timer;
  
  timer.start();
  
  for( ; first != last; ++ first )
    table.erase( *first );
  
  timer.stop();
    
  LogPerfData( perfLog,
	       dataMap.getClassName(),
	       table.getClassName(),
	       "erase",
	       order,
	       initAlloc,
	       table.getNodeSize(),
	       quantity,
	       timer );

  return( table.size() == 0 && table.good() );
}

template< class Table, class Iterator >
inline
bool
pFind(
  MapMemDynamic &   dataMap,
  Table &	    table,
  Iterator 	    first,
  Iterator	    last,
  const char *	    order,
  ostream &	    perfLog,
  size_t	    initAlloc,
  size_t	    quantity
  )
{
  TimeIt    timer;
  
  timer.start();
  
  for( ; first != last; ++ first )
    table.find( *first );
  
  timer.stop();
    
  LogPerfData( perfLog,
	       dataMap.getClassName(),
	       table.getClassName(),
	       "find",
	       order,
	       initAlloc,
	       table.getNodeSize(),
	       quantity,
	       timer );

  return( table.size() == quantity && table.good() );
}


template< class Data, class List >
pMapType( 
  const Data &	    CLUE_UNUSED( x ),
  List &	    dataList,
  MapMemDynamic &   dataMap,
  size_t	    initAllocNumRecs,
  long		    quantity,
  ostream &	    perfLog
  )
{
  typedef RBSet< Data, less< Data > > Table;
  
  MultiMemOffsetMapDynamic	dataMgr( &dataMap, false );

  if( ! dataMgr.good() )
    {
      AppError << dataMgr.error() << endl;
      return( false );
    }

  Table   table( &dataMgr, 0, true );

  if( ! table.good() )
    {
      AppError << table.error() << endl;
      return( false );
    }

  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "asc", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  if( ! pFind( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "asc", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  if( ! pErase( dataMap, table, dataList.rbegin(), dataList.rend(),
	       "rev", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "asc", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  random_shuffle( dataList.begin(), dataList.end() );
    
  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "rand", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  sort( dataList.begin(), dataList.end(), greater< Data >() );
    
  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "rev", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  if( ! pFind( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "rev", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  random_shuffle( dataList.begin(), dataList.end() );
    
  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "rand", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "rand", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  if( ! pFind( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  random_shuffle( dataList.begin(), dataList.end() );
    
  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "rand", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  return( true );
}
  

template< class Data >
inline
bool
pDataType(
  const Data &	    x,
  const char *	    fileName,
  size_t	    initAllocNumRecs,
  long		    quantity,
  ostream &	    perfLog
 )
{
  
  typedef RBSet< Data, less<Data> >	    Table;
  typedef vector< Data >    List;
  
  List    dataList;
  
  {
    Data d;
    for( long k = 0; k < quantity; ++ k )
      {
	d.k = k;
	dataList.push_back( d );
      }
  }

  TimeIt  timer;

  timer.start();
  for( List::iterator them = dataList.begin();
       them != dataList.end();
       ++ them );
  timer.stop();
    
  LogPerfData( perfLog,
	       "N/A",
	       "Iteration",
	       "loop",
	       "N/A",
	       0,
	       0,
	       quantity,
	       timer );

  {
    MapMemDynamicFixed	dataMap( fileName,
				 Table::getNodeSize(),
				 initAllocNumRecs,
				 02 );
    
    if( ! dataMap.good() )
      {
	AppError << dataMap.error() << endl;
	return( false );
      }

    if( ! pMapType( x,
		    dataList,
		    dataMap,
		    initAllocNumRecs,
		    quantity,
		    perfLog ) )
      return( false );
  }

  {
    MapMemDynamicDynamic	dataMap( fileName,
					 Table::getNodeSize(),
					 initAllocNumRecs,
					 02 );
    
    if( ! dataMap.good() )
      {
	AppError << dataMap.error() << endl;
	return( false );
      }

    if( ! pMapType( x,
		    dataList,
		    dataMap,
		    initAllocNumRecs,
		    quantity,
		    perfLog ) )
      return( false );
  }

  return( true );
}
   

bool
pRBSet( 
  const char *	fileName,
  size_t	initAllocNumRecs,
  long		quantity,
  ostream &	perfLog
 )
{
  Rec_4 d_4; d_4.k = 0;
  
  if( ! pDataType( d_4,
		   fileName,
		   initAllocNumRecs,
		   quantity,
		   perfLog ) )
    return( false );
  
  Rec_128 d_128; d_128.k = 0;
  
  if( ! pDataType( d_128,
		   fileName,
		   initAllocNumRecs,
		   quantity,
		   perfLog ) )
    return( false );
  
#if defined( Linux )
  if( quantity > 20000 )
    return( true );
#endif
  
  Rec_512 d_512; d_512.k = 0;
  
  if( ! pDataType( d_512,
		   fileName,
		   initAllocNumRecs,
		   quantity,
		   perfLog ) )
    return( false );
  
  Rec_1024 d_1024; d_1024.k = 0;
  
  if( ! pDataType( d_1024,
		   fileName,
		   initAllocNumRecs,
		   quantity,
		   perfLog ) )
    return( false );

  return( true );
}



  

// Revision Log:
//
// $Log$
// Revision 1.1  1997/07/13 11:36:40  houghton
// Initial Version.
//
//
