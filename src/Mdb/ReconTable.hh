#ifndef _ReconTable_hh_
#define _ReconTable_hh_
//
// File:        ReconTable.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul Houghton 719-527-7834 - (paul.houghton@wcom.com)
// Created:     07/31/00 06:13
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
#include <ReconTableBase.hh>

#include <DumpInfo.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif

template< class Rec, class LessKey >
class ReconTable : public ReconTableBase
{

public:

  typedef ReconTableBase::size_type	size_type;
  typedef const Rec *			const_iterator;
  typedef ReconTableBase::RecordNum	RecordNum;
  typedef ReconTableBase::VersionNum	VersionNum;
  
  inline ReconTable( const FilePath &	tableFn,
		     VersionNum		versionNum,
		     ios::open_mode	mode = ios::in,
		     bool		create = false,
		     MapFile::MapMask	permMask = 02 );

  virtual ~ReconTable( void ) {};

  inline bool	    append( const Rec & rec );

  inline const_iterator	    begin( void ) const;
  inline const_iterator	    end( void ) const;

  inline RecordNum	    rfind( const Rec & key ) const;

  inline bool		    update( RecordNum r, const Rec & src );

  inline const Rec &	    operator [] ( RecordNum r ) const;
  
protected:

  inline const Rec &    record( RecordNum recNum ) const; // 0 is first rec
  inline Rec &          record( RecordNum recNum );       // ditto
  
  LessKey	lessKey;
  
private:

  ReconTable( const ReconTable & from );
  ReconTable & operator =( const ReconTable & from );

};

#if !defined( inline )
#include <ReconTable.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const ReconTable & src );

istream &
operator >> ( istream & src, const ReconTable & dest );


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	ReconTable	class
//
//  Constructors:
//
//  	ReconTable( );
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
//  	    Return the name of this class (i.e. ReconTable )
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
//  	operator <<( ostream & dest, const ReconTable & src );
//
//	istream &
//	operator >> ( istream & src, ReconTable & dest );
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
// Revision 1.1  2000/08/02 11:03:47  houghton
// Initial Version.
//
//
#endif // ! def _ReconTable_hh_ 

