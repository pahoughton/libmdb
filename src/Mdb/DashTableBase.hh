#ifndef _DashTableBase_hh_
#define _DashTableBase_hh_
//
// File:        DashTableBase.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/02/97 08:36
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
#include <HashTableBase.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class DashTableBase : public HashTableBase
{

public:

  typedef long		EffDate;
  
  DashTableBase( ChunkMgr &	chunkMgr,
		 const char *	indexFileName,
		 ios::open_mode	mode = ios::in,
		 unsigned short	permMask = 0,
		 bool		create = false );

  virtual ~DashTableBase( void );

  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  static const ClassVersion version;

  struct DashNodeBase : public HashNodeBase
  {
    Loc		nextEff;
    Loc		prevEff;
    EffDate	when;
  };

protected:

  inline const DashNodeBase &	dashNode( Loc node ) const;
  inline DashNodeBase &		dashNode( Loc node );
  
  Loc		insert( Hash hash, Loc cur, EffDate eff, Loc node );
  Loc		insert( Hash hash, EffDate eff, Loc node );
  
  bool		erase( Hash hash, Loc node );

  inline Loc	next( Hash & hash, Loc & node  ) const;
  inline Loc	prev( Hash & hash, Loc & node ) const;
  
private:

  DashTableBase( const DashTableBase & from );
  DashTableBase & operator =( const DashTableBase & from );

};

#if !defined( inline )
#include <DashTableBase.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	DashTableBase	class
//
//  Constructors:
//
//  	DashTableBase( );
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
//  	    Return the name of this class (i.e. DashTableBase )
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
//  	operator <<( ostream & dest, const DashTableBase & src );
//
//	istream &
//	operator >> ( istream & src, DashTableBase & dest );
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
// Revision 2.2  1997/06/05 13:42:01  houghton
// Changed for AIX: had to make DashNodeBase a public member.
//
// Revision 2.1  1997/06/05 11:29:09  houghton
// Initial Version.
//
//
#endif // ! def _DashTableBase_hh_ 

