#ifndef _RBSet_hh_
#define _RBSet_hh_
//
// File:        RBSet.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/10/97 05:47
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
#include <MdbUtils.hh>
#include <RBTree.hh>
#include <DumpInfo.hh>
#include <iostream>

template < class Key, class LessKey >
class RBSet
{

public:

  typedef RBTree< Key, Key, MdbIdent< Key, Key >, LessKey > Table;
  typedef Table::size_type		size_type;
  typedef Table::iterator		iterator;
  typedef Table::const_iterator		const_iterator;
  typedef Table::reverse_iterator	reverse_iterator;
  typedef Table::const_reverse_iterator	const_reverse_iterator;
  typedef Table::pair_iterator_bool	pair_iterator_bool;
  

  inline RBSet( MultiMemOffset *    memMgr,
		 unsigned short	    treeKey = 0,
		 bool		    create = false );
		
  virtual ~RBSet( void );

  inline pair_iterator_bool	insert( const Key & key ) {
    return( table.insert( key ) );
  };
  
  inline iterator		find( const Key & key ) {
    return( table.find( key ) );
  };

  inline const_iterator		find( const Key & key ) const {
    return( table.find( key ) );
  };
  
  inline bool			erase( const Key & key ) {
    return( table.erase( key ) );
  };

  inline bool			erase( const iterator & pos ) {
    return( table.erase( pos ) );
  };

  // inline bool			erase( const iterator & first,
  // const iterator & last ) {
  //  return( table.erase( first, last ) );
  // };
  
  inline iterator	    begin( void ) { return( table.begin() ); };
  inline iterator	    end( void ) { return( table.end() ); };
  
  inline const_iterator	    begin( void ) const { return( table.begin() ); };
  inline const_iterator	    end( void ) const { return( table.end() ); };

  inline reverse_iterator	rbegin( void ) { return( table.rbegin() ); };
  inline reverse_iterator	rend( void ) { return( table.rend() ); };

  inline const_reverse_iterator	rbegin( void ) const {return(table.rbegin());};
  inline const_reverse_iterator	rend( void ) const {return(table.rend());};

  inline size_type	    size( void ) const { return( table.size() ); };
  inline bool		    empty( void ) const { return( table.empty() ); };

  static size_type	    getNodeSize( void ) {
    return( Table::getNodeSize() );
  };
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline
  DumpInfo< RBSet< Key, LessKey > >
  dump( const char *	prefix = "    ",
	bool		showVer = true ) const {
    return( DumpInfo< RBSet< Key, LessKey > >( *this, prefix, showVer ) );
  };
  
  static const ClassVersion version;

protected:

private:

  // RBTree will prevent these
  //
  // RBSet( const RBSet & from );
  // RBSet & operator =( const RBSet & from );

  Table	    table;
};

#include <RBSet.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	RBSet	class
//
//  Constructors:
//
//  	RBSet( );
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
//  	    Return the name of this class (i.e. RBSet )
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
//  	operator <<( ostream & dest, const RBSet & src );
//
//	istream &
//	operator >> ( istream & src, RBSet & dest );
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
// Revision 2.1  1997/07/11 17:37:48  houghton
// Initial Version.
//
//
#endif // ! def _RBSet_hh_ 

