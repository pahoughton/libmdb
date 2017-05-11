#ifndef _AppParam_hh_
#define _AppParam_hh_
//
// File:        AppParam.hh
// Project:	Mdb
// Desc:        
//
//
//
// Author:      Paul Houghton - (paul.houghton@wcom.com)
// Created:     07/20/97 09:57
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <Param.hh>
#include <FilePath.hh>
#include <iostream>

class AppParam : public Param
{

public:

  AppParam( int & argv, char ** argc, const char * ver = 0 );

  ~AppParam( void );

  const FilePath &  perfInfoFn( void ) const { return( perfInfoFnV ); };

  long		    passes( void ) const { return( passesV ); };
  const FilePath &  tableDir( void ) const { return( tableDirV ); };
  const Str &	    mapType( void ) const { return( mapTypeV ); };
  long		    recSize( void ) const { return( recSizeV ); };
  
  ios::open_mode    tableMode( void ) const { return( tableModeV ); };
  bool		    create( void ) const { return( createV ); };
  unsigned long	    initSize( void ) const { return( initSizeV ); };
  unsigned long	    allocSize( void ) const { return( allocSizeV ); };
  long		    qty( void ) const { return( qtyV ); };

  const Str &	    order( void ) const { return( orderV ); };
  bool		    insert( void ) const { return( insertV ); };
  bool		    find( void ) const { return( findV ); };
  bool		    erase( void ) const { return( eraseV ); };
  bool		    iterate( void ) const { return( iterateV ); };
  
  bool		parseArgs( void );
  bool		parseArgs( int argc, char * argv[] );

protected:

private:

  AppParam( const AppParam & copyFrom );
  AppParam & operator=( const AppParam & assignFrom );

  FilePath	    perfInfoFnV;

  long		    passesV;
  FilePath	    tableDirV;
  Str		    mapTypeV;
  long		    recSizeV;
  ios::open_mode    tableModeV;
  bool		    createV;
  unsigned long	    initSizeV;
  unsigned long	    allocSizeV;
  long		    qtyV;

  Str		    orderV;
  bool		    insertV;
  bool		    findV;
  bool		    eraseV;
  bool		    iterateV;
  
};

extern AppParam * App;


//  Data Types: - data types defined by this header
//
//  	AppParam	class
//
//  Constructors:
//
//  	AppParam( );
//
//  Destructors:
//
//  Public Interface:
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. AppParam )
//
//  	virtual Bool
//  	good( void ) const;
//  	    Returns true if there are no detected errors associated
//  	    with this class, otherwise FALSE.
//
//  	virtual const char *
//  	error( void ) const
//  	    Returns as string description of the state of the class.
//
//  Protected Interface:
//
//  Private Methods:
//
//  Other Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const AppParam & obj );
//
// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:07  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/25 13:41:38  houghton
// Initial Version.
//
//
//

#endif // ! def _AppParam_hh_ 
