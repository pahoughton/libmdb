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
// Created:     05/05/99 06:11
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
#include <Str.hh>

class AppParam : public Param
{

public:

  AppParam( int & argv, char ** argc, const char * ver = 0 );

  ~AppParam( void );

  bool		parseArgs( void );
  bool		parseArgs( int argc, char * argv[] );

protected:

private:

  AppParam( const AppParam & copyFrom );
  AppParam & operator=( const AppParam & assignFrom );

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
// Revision 1.1  1999/06/10 11:47:09  houghton
// Initial Version.
//
//

#endif // ! def _AppParam_hh_ 
