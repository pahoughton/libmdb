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
  long		    quantity
  )
{
  TimeIt    timer;
  
  Table::size_type startSize( table.size() );
  
  timer.start();
  
  for( ; first != last; ++ first )
    table.insert( *first );
  
  timer.stop();
    
  LogPerfData( perfLog,
	       "RBSet",
	       1,
	       1,
	       dataMap.getClassName(),	       
	       initAlloc,
	       0,
	       table.getNodeSize(),	       
	       table.getClassName(),
	       "insert",
	       order,
	       startSize,
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
  long		    quantity
  )
{
  TimeIt    timer;
  
  Table::size_type startSize( table.size() );
  
  timer.start();
  
  for( ; first != last; ++ first )
    table.erase( *first );
  
  timer.stop();
    
  LogPerfData( perfLog,
	       "RBSet",
	       1,
	       1,
	       dataMap.getClassName(),	       
	       initAlloc,
	       0,
	       table.getNodeSize(),	       
	       table.getClassName(),
	       "erase",
	       order,
	       startSize,
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
  long		    quantity
  )
{
  TimeIt    timer;
  
  Table::size_type startSize( table.size() );
  
  timer.start();
  
  for( ; first != last; ++ first )
    table.find( *first );
  
  timer.stop();
    
  LogPerfData( perfLog,
	       "RBSet",
	       1,
	       1,
	       dataMap.getClassName(),	       
	       initAlloc,
	       0,
	       table.getNodeSize(),	       
	       table.getClassName(),
	       "find",
	       order,
	       startSize,
	       quantity,
	       timer );

  return( table.size() == quantity && table.good() );
}


template< class Data, class List >
pMapType( 
  const Data &	    STLUTILS_UNUSED( x ),
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
  
  AppDebug << table.dump() << endl;
  
  if( ! pFind( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  AppDebug << table.dump() << endl;
  
  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  AppDebug << table.dump() << endl;
  
  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "asc", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  AppDebug << table.dump() << endl;
  
  if( ! pErase( dataMap, table, dataList.rbegin(), dataList.rend(),
	       "rev", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  AppDebug << table.dump() << endl;
  
  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "asc", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  AppDebug << table.dump() << endl;
  
  random_shuffle( dataList.begin(), dataList.end() );
    
  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "rand", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  AppDebug << table.dump() << endl;
  
  sort( dataList.begin(), dataList.end(), greater< Data >() );
    
  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "rev", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  AppDebug << table.dump() << endl;
  
  if( ! pFind( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  AppDebug << table.dump() << endl;
  
  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  AppDebug << table.dump() << endl;
  
  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "rev", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  AppDebug << table.dump() << endl;
  
  random_shuffle( dataList.begin(), dataList.end() );
    
  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "rand", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  AppDebug << table.dump() << endl;
  
  if( ! pInsert( dataMap, table, dataList.begin(), dataList.end(),
		 "rand", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  AppDebug << table.dump() << endl;
  
  if( ! pFind( dataMap, table, dataList.begin(), dataList.end(),
	       "same", perfLog, initAllocNumRecs, quantity ) )
    return( false );
  
  AppDebug << table.dump() << endl;
  
  random_shuffle( dataList.begin(), dataList.end() );
    
  if( ! pErase( dataMap, table, dataList.begin(), dataList.end(),
	       "rand", perfLog, initAllocNumRecs, quantity ) )
    return( false );

  AppDebug << table.dump() << endl;
  
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
	       "RBSet",
	       1,
	       1,
	       "DynamicFixed",
	       Table::getNodeSize() * initAllocNumRecs,
	       0,
	       sizeof( x ),
	       "RBSet",
	       "Iteration",
	       "loop",
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
  long		recSize,
  size_t	initAllocNumRecs,
  long		quantity,
  ostream &	perfLog
 )
{

  switch( recSize )
    {
    case 4:
      {
	Rec_4 d_4; d_4.k = 0;
	
	if( ! pDataType( d_4,
			 fileName,
			 initAllocNumRecs,
			 quantity,
			 perfLog ) )
	  return( false );
	break;
      }

    case 128:
      {
	Rec_128 d_128; d_128.k = 0;
	
	if( ! pDataType( d_128,
			 fileName,
			 initAllocNumRecs,
			 quantity,
			 perfLog ) )
	  return( false );
	break;
      }

    case 512:
      {
	Rec_512 d_512; d_512.k = 0;
	
	if( ! pDataType( d_512,
			 fileName,
			 initAllocNumRecs,
			 quantity,
			 perfLog ) )
	  return( false );
	break;
      }

    case 1024:
      {
	Rec_1024 d_1024; d_1024.k = 0;
	
	if( ! pDataType( d_1024,
			 fileName,
			 initAllocNumRecs,
			 quantity,
			 perfLog ) )
	  return( false );
	break;
      }

    default:
      break;
    }
  
  return( true );
}



  

// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.2  2000/05/27 14:31:56  houghton
// Port: Sun CC 5.0.
//
// Revision 2.1  1997/09/21 19:22:06  houghton
// Changed version to 2
//
// Revision 1.3  1997/09/17 16:56:16  houghton
// Changed for new library rename to StlUtils
//
// Revision 1.2  1997/07/14 10:53:27  houghton
// Added recSize arg.
// Added debugging info to log.
//
// Revision 1.1  1997/07/13 11:36:40  houghton
// Initial Version.
//
//
