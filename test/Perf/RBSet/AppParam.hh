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
  const Str &	    mapType( void ) const { return( mapTypeV ); };
  const FilePath &  tableFn( void ) const { return( tableFnV ); };
  ios::open_mode    tableMode( void ) const { return( tableModeV ); };
  bool		    create( void ) const { return( createV ); };
  unsigned long	    allocSize( void ) const { return( allocSizeV ); };
  long		    qty( void ) const { return( qtyV ); };

  const Str &	    order( void ) const { return( orderV ); };
  bool		    insert( void ) const { return( insertV ); };
  bool		    find( void ) const { return( findV ); };
  
  bool		parseArgs( void );
  bool		parseArgs( int argc, char * argv[] );

protected:

private:

  AppParam( const AppParam & copyFrom );
  AppParam & operator=( const AppParam & assignFrom );

  FilePath	    perfInfoFnV;
  Str		    mapTypeV;
  FilePath	    tableFnV;
  ios::open_mode    tableModeV;
  bool		    createV;
  unsigned long	    allocSizeV;
  long		    qtyV;

  Str		    orderV;
  bool		    insertV;
  bool		    findV;
  
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
// Revision 1.1  1997/07/21 10:29:40  houghton
// Initial version.
//
//

#endif // ! def _AppParam_hh_ 
