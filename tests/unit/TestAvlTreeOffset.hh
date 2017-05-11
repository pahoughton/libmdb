#ifndef _TestAvlTreeOffset_hh_
#define _TestAvlTreeOffset_hh_
//
// File:        TestAvlTreeOffset.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/01/97 05:18
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <MdbConfig.hh>
#include <AvlTreeOffset.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class TestAvlTreeOffset
{

public:

  struct Key
  {
    char    k[ 32 ];
  };

  struct Rec : public Key
  {
    long	l;
    char	d[16];
  };

  class Tree : public AvlTreeOffset< Rec >
  {
  public:

    Tree( const char * fileName,
	  ios::open_mode    mode,
	  bool		    create );

    ~Tree( void );

    ostream &	dumpKey( ostream & dest, const Loc nodeOffset ) const;

    static int	    compKey( const Rec & one, const Rec & two );
    static Rec &    copyRec( Rec & dest, const Rec & src );
    
  private:

  };
  
  TestAvlTreeOffset( const char *	fileName,
	       ios::open_mode	mode,
	       bool		create );

  virtual ~TestAvlTreeOffset( void );

  Tree &	tree( void );
  const Tree &	tree( void ) const;
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;

protected:

private:

  TestAvlTreeOffset( const TestAvlTreeOffset & from );
  TestAvlTreeOffset & operator =( const TestAvlTreeOffset & from );

  Tree	avlTree;
  
};

#if !defined( inline )
#include <TestAvlTreeOffset.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const TestAvlTreeOffset & src );

istream &
operator >> ( istream & src, const TestAvlTreeOffset & dest );


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	TestAvlTreeOffset	class
//
//  Constructors:
//
//  	TestAvlTreeOffset( );
//
//  Destructors:
//
//  Public Interface:
//
//	virtual ostream &
//	write( ostream & dest ) const;
//	    write the data for this class in binary form to the ostream.
//
//	virtual istream &
//	read( istream & src );
//	    read the data in binary form from the istream. It is
//	    assumed it stream is correctly posistioned and the data
//	    was written to the istream with 'write( ostream & )'
//
//	virtual ostream &
//	toStream( ostream & dest ) const;
//	    output class as a string to dest (used by operator <<)
//
//	virtual istream &
//	fromStream( istream & src );
//	    Set this class be reading a string representation from
//	    src. Returns src.
//
//  	virtual Bool
//  	good( void ) const;
//  	    Return true if there are no detected errors associated
//  	    with this class, otherwise false.
//
//  	virtual const char *
//  	error( void ) const;
//  	    Return a string description of the state of the class.
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. TestAvlTreeOffset )
//
//  	virtual const char *
//  	getVersion( bool withPrjVer = true ) const;
//  	    Return the version string of this class.
//
//	virtual ostream &
//	dumpInfo( ostream & dest, const char * prefix, bool showVer );
//	    output detail info to dest. Includes instance variable
//	    values, state info & version info.
//
//	static const ClassVersion version
//	    Class and project version information. (see ClassVersion.hh)
//
//  Protected Interface:
//
//  Private Methods:
//
//  Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const TestAvlTreeOffset & src );
//
//	istream &
//	operator >> ( istream & src, TestAvlTreeOffset & dest );
//
// Example:
//
// See Also:
//
// Files:
//
// Documented Ver:
//
// Tested Ver:
//
// Revision Log:
//
// $Log$
// Revision 4.1  2001/07/27 00:57:45  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/07/11 17:38:43  houghton
// Initial Version.
//
//
#endif // ! def _TestAvlTreeOffset_hh_ 

