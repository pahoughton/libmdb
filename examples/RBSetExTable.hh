#ifndef _RBSetExTable_hh_
#define _RBSetExTable_hh_
//
// File:        RBSetExTable.hh
// Project:	Mdb
// Desc:        
//
//  This is an example table that uses RBSet<> for data storage and
//  indexing. This is not the only way to use RBSet<>, but I recommend
//  it.
//
//  There are a number of advantages to utilizing the continer classes
//  this way. The first is that you would be able to change the container
//  class out without changing the interaface to the table. Another one
//  is you can create specialized methods for the table.
//
//  In this example, I am methods for all of the funtionallity available
//  in the container class. This is not really neccessary.
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/23/97 05:00
//
// Revision History: (See end of file for Revision Log)
//
//  Last Mod By:    $Author$
//  Last Mod:	    $Date$
//  Version:	    $Revision$
//
//  $Id$
//

#include <RBSet.hh>
#include <DumpInfo.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class RBSetExTable
{

public:

  class Rec
  {
    long    key;
    char    data[16];
  };

  class LessRec
  {
  public:
    bool    operator () ( const Rec & one, const Rec & two ) const {
      return( one.key < two.key );
    };
  };

  typedef RBSet< Rec, LessRec >	    Table;

  typedef Table::size_Type		size_type;
  typedef Table::iterator		iterator;
  typedef Table::const_iterator		const_iterator;
  typedef Table::reverse_iterator	reverse_iterator;
  typedef Table::const_reverse_iterator	const_reverse_iterator;
  typedef Table::pair_iterator_bool	pair_iterator_bool;
  
  RBSetExTable( const char *	fileName,
		ios::open_mode	mode = ios::in,
		bool		create );

  virtual ~RBSetExTable( void );

  inline pair_iterator_bool	insert( const Rec & rec );

  inline interator		find( const Rec & key );  
  inline const_interator	find( const Rec & key ) const;

  inline bool			erase( const Rec & key );
  inline bool			erase( iterator pos );
  inline bool			erase( iterator first, iterator last );

  inline const_iterator		begin( void ) const;
  inline const_iterator		end( void ) const;
  inline const_reverse_iterator	rbegin( void ) const;
  inline const_reverse_iterator	rend( void ) const;

  inline iterator		begin( void );
  inline iterator		end( void );
  inline reverse_iterator	rbegin( void );
  inline reverse_iterator	rend( void );

  inline size_type		size( void ) const;
  inline bool			empty( void ) const;

  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline DumpInfo< RBSetExTable >
  dump( const char * preifx = "    ", bool showVer = true ) const;

  static const ClassVersion version;

protected:

  MultiMemOffsetMapDynamci  mmo;
  Table			    table;
  
private:

  RBSetExTable( const RBSetExTable & from );
  RBSetExTable & operator =( const RBSetExTable & from );

};

#if !defined( inline )
#include <RBSetExTable.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const RBSetExTable & src );

istream &
operator >> ( istream & src, const RBSetExTable & dest );


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	RBSetExTable	class
//
//  Constructors:
//
//  	RBSetExTable( );
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
//  	    Return the name of this class (i.e. RBSetExTable )
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
//  	operator <<( ostream & dest, const RBSetExTable & src );
//
//	istream &
//	operator >> ( istream & src, RBSetExTable & dest );
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
// Revision 4.1  2001/07/27 00:57:42  houghton
// Change Major Version to 4
//
// Revision 2.1  1997/09/21 19:21:31  houghton
// Changed version to 2
//
// Revision 1.1  1997/07/25 13:40:44  houghton
// Initial Version.
//
//
#endif // ! def _RBSetExTable_hh_ 

