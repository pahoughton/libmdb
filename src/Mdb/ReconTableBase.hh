#ifndef _ReconTableBase_hh_
#define _ReconTableBase_hh_
//
// File:        ReconTableBase.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul Houghton 719-527-7834 - (paul.houghton@wcom.com)
// Created:     07/31/00 05:44
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
#include <MapFile.hh>

#include <FileLock.hh>
#include <Net2Host.hh>
#include <DumpInfo.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif

class FilePath;

class ReconTableBase
{

public:

  typedef unsigned int	    VersionNum;
  typedef unsigned int	    RecordNum;
  typedef long		    size_type;
  
  virtual ~ReconTableBase( void );

  inline RecordNum	size( void ) const;
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline DumpInfo< ReconTableBase >
  dump( const char * preifx = "    ", bool showVer = true ) const;

  static const ClassVersion version;

  static VersionNum	getFileVersion( const FilePath & fn );
  
   // note: this should be protected, but sun compiler doesn't like it.
  struct Header
  {
    VersionNum	    version;
    unsigned int    recSize;
    unsigned int    mapSize;
    RecordNum	    count;
  };

  static const RecordNum    BadRec;
  
protected:

  ReconTableBase( const FilePath &   tableFn,
		  ios::open_mode     mode,
		  bool               create,
		  MapFile::MapMask   permMask,
		  unsigned int	     tableVersion,
		  unsigned int	     recSize );

  bool	    append( const void * rec );
  bool	    update( RecordNum r, const void * rec );
  
  inline const Header & header( void ) const;
  inline Header &       header( void );
  inline size_type      recpos( RecordNum recNum ) const; // 0 is first rec
  inline void *		recptr( RecordNum recNum ); // ditto
  
  MapFile   map;
  FileLock  lock;

  enum ErrorNum
  {
    E_OK,
    E_VERSION,
    E_RECSIZE,
    E_UNDEFINED
  };

  unsigned int	tableVer;
  unsigned int	recSize;
  
  ErrorNum  errorNum;

  
private:

  ReconTableBase( const ReconTableBase & from );
  ReconTableBase & operator =( const ReconTableBase & from );

};

#if !defined( inline )
#include <ReconTableBase.ii>
#else
#undef inline

#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	ReconTableBase	class
//
//  Constructors:
//
//  	ReconTableBase( );
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
//  	    Return the name of this class (i.e. ReconTableBase )
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
//  	operator <<( ostream & dest, const ReconTableBase & src );
//
//	istream &
//	operator >> ( istream & src, ReconTableBase & dest );
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
// Revision 4.1  2001/07/27 00:57:44  houghton
// Change Major Version to 4
//
// Revision 1.1  2000/08/02 11:03:47  houghton
// Initial Version.
//
//
#endif // ! def _ReconTableBase_hh_ 

