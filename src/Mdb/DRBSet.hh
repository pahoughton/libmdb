#ifndef _DRBSet_hh_
#define _DRBSet_hh_
//
// File:        DRBSet.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     07/16/97 07:30
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
#include <DRBTree.hh>
#include <DumpInfo.hh>
#include <iostream>

template < class Key, class LessKey >
class DRBSet
{

public:

  typedef DRBTree< Key, Key, MdbIdent< Key, Key >, LessKey > Table;
  typedef Table::size_type		size_type;
  typedef Table::iterator		iterator;
  typedef Table::const_iterator		const_iterator;
  typedef Table::reverse_iterator	reverse_iterator;
  typedef Table::const_reverse_iterator	const_reverse_iterator;
  typedef Table::pair_iterator_bool	pair_iterator_bool;
  typedef Table::EffDate		EffDate;
  
  inline DRBSet( MultiMemOffset *   memMgr,
		 unsigned short	    treeKey = 0,
		 bool		    create = false );
  
  inline DRBSet( MultiMemOffset *   memMgr,
		 MultiMemOffset *   histMgr,
		 unsigned short	    treeKey = 0,
		 bool		    create = false );
  
  virtual ~DRBSet( void );

  inline pair_iterator_bool	insert( const Key & key, EffDate eff ) {
    return( table.insert( key, eff ) );
  };
  
  inline iterator		find( const Key & key, EffDate eff ) {
    return( table.find( key, eff ) );
  };

  inline const_iterator		find( const Key & key, EffDate eff ) const {
    return( table.find( key, eff ) );
  };

  inline bool			erase( const Key & key, EffDate eff ) {
    return( table.erase( key, eff ) );
  };

  inline bool			erase( const iterator & pos, EffDate eff ) {
    return( table.erase( *pos, eff ) );
  };

  
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

  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline
  DumpInfo< DRBSet< Key, LessKey > >
  dump( const char *	prefix = "    ",
	bool		showVer = true ) const {
    return( DumpInfo< DRBSet< Key, LessKey > >( *this, prefix, showVer ) );
  };
  
protected:

private:

  // DRBSet( const DRBSet & from );
  // DRBSet & operator =( const DRBSet & from );

  Table	    table;
};

#include <DRBSet.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	DRBSet	class
//
//  Constructors:
//
//  	DRBSet( );
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
//  	    Return the name of this class (i.e. DRBSet )
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
//  	operator <<( ostream & dest, const DRBSet & src );
//
//	istream &
//	operator >> ( istream & src, DRBSet & dest );
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
// Revision 2.2  1997/07/22 19:41:16  houghton
// Cleanup.
// Bug-Fix: fixed method args on some of the methods.
//
// Revision 2.1  1997/07/16 16:36:54  houghton
// Initial Version (work in progress).
//
//
#endif // ! def _DRBSet_hh_ 

