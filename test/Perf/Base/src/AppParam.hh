#ifndef _AppParam_hh_
#define _AppParam_hh_
//
// File:        AppParam.hh
// Project:	Mdb
// Desc:        
//
//
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/10/97 06:03
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

class AppParam : public Param
{

public:

  AppParam( int & argv, char ** argc, const char * ver = 0 );

  ~AppParam( void );

  const FilePath &  perfLogFn( void ) const { return( perfLogFnV ); };
  
  bool		parseArgs( void );
  bool		parseArgs( int argc, char * argv[] );

protected:

private:

  FilePath  perfLogFnV;
  
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
// Revision 4.1  2001/07/27 00:57:46  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:22:04  houghton
// Changed version to 2
//
// Revision 1.2  1997/07/14 10:50:53  houghton
// Added perfLogFn arg.
//
// Revision 1.1  1997/07/13 11:36:39  houghton
// Initial Version.
//
//

#endif // ! def _AppParam_hh_ 
