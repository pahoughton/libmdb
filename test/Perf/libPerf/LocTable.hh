#ifndef _LocTable_hh_
#define _LocTable_hh_
//
// File:        LocTable.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 14:41
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
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif

class Rating
{
public:
  
  typedef CLUE_U32	    LocationId;
  typedef char		    CityName[16];
  typedef CLUE_U16	    GeoCode;
  typedef CLUE_U16	    RegionId;
  typedef char		    RegionAbbr[4];
  typedef CLUE_16	    TzMinOffset:

  typedef CLUE_16	    s16;
  typedef CLUE_U16	    u16;

  typedef CLUE_32	    s32;
  typedef CLUE_U32	    u32;
  typedef unsigned char	    Boolean;
  
};

class LocTable
{

public:

  struct Key
  {
    Rating::LocationId	loc;
  };
  
  struct Rec : public Key
  {
    Rating::CityName_t	    name;
    Rating::Geo_t           geo;            // Numeric code for cde_rgn

    Rating::RegionId_t      regionId;
    Rating::RegionAbbr_t    regionAbbr;	// state abbr
  
    union {
      Rating::u16	dialPlan;	// international country codes
      Rating::u16 	band;		// Mexico Bands
      Rating::u16	lata;		// Us lata
      Rating::u16	generic;	// generic value
    } lbd; // lbd keeps various data, and meaning of its content is
	   // defined by information in the location field of the key.
    
    Rating::u16		    vert;
    Rating::u16		    horz;

    Rating::TZMinOffset	    tzOffset;  // timesone offset
    Rating::Boolean         honorsDST; // Do I honor DST?
  };

  class RecHash
  {
  public:
    HashTableBase::Hash	operator() ( const Rec & rec ) const {
      return( rec.loc );
    };
  };

  class RecLess
  {
  public:
    bool    operator () ( const Rec & one, const Rec & two ) const {
      return( one.loc < two.loc );
    };
  };
  
  typedef Dash< Rec, RecHash, RecLess >	    Table;
  typedef Table::iterator		    iterator;
  typedef Table::const_iterator		    const_iterator;
  
  LocTable( const char *    datadir,
	    ios::open_mode  mode = ios::in,
	    bool	    create = false );

  virtual ~LocTable( void );

  inline pair_iterator_bool	insert( const Rec & rec );
  inline const_iterator		find( Rating::LocationId loc ) const;
  inline iterator		find( const Rec & key );
  
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

  LocTable( const LocTable & from );
  LocTable & operator =( const LocTable & from );

};

#if !defined( inline )
#include <LocTable.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const LocTable & src );

istream &
operator >> ( istream & src, const LocTable & dest );


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	LocTable	class
//
//  Constructors:
//
//  	LocTable( );
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
//  	    Return the name of this class (i.e. LocTable )
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
//  	operator <<( ostream & dest, const LocTable & src );
//
//	istream &
//	operator >> ( istream & src, LocTable & dest );
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
// Revision 1.1  1997/07/19 12:52:04  houghton
// Initial Version (work in progess).
//
//
#endif // ! def _LocTable_hh_ 

