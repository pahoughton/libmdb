#ifndef _RBSet_hh_
#define _RBSet_hh_
//
// File:        RBSet.hh
// Project:	Mdb
// Desc:        
//
//  The RBSet<> template class is a Red Black Binary Tree for a single
//  data type (ussually a struct). It can use mapped memory for data
//  storage and access. It's interaface mimics the Standard C++ STL
//  set<> template class. The only significant differance is the
//  constructore which requires a MultiMemOffset object for memory
//  managment.
//
// Notes:
//
//  The destructor does NOT delete the MultiMemOffset object.
//
//  All of the functionallity is obtain from the RBTree<> template class.
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
  
  inline const_iterator		find( const Key & key ) const {
    return( table.find( key ) );
  };
  
  inline iterator		find( const Key & key ) {
    return( table.find( key ) );
  };

  inline bool			erase( const Key & key ) {
    return( table.erase( key ) );
  };

  inline bool			erase( const iterator & pos ) {
    return( table.erase( pos ) );
  };

  inline bool			erase( const iterator & first,
				       const iterator & last ) {
    return( table.erase( first, last ) );
  };
  
  inline const_iterator		begin( void ) const { return(table.begin());};
  inline const_iterator		end( void ) const { return( table.end() ); };

  inline iterator		begin( void ) { return( table.begin() ); };
  inline iterator		end( void ) { return( table.end() ); };
  
  inline const_reverse_iterator	rbegin( void ) const {return(table.rbegin());};
  inline const_reverse_iterator	rend( void ) const {return(table.rend());};

  inline reverse_iterator	rbegin( void ) { return( table.rbegin() ); };
  inline reverse_iterator	rend( void ) { return( table.rend() ); };

  inline size_type		size( void ) const { return( table.size() ); };
  inline bool			empty( void ) const { return( table.empty());};

  static size_type		getNodeSize( void ) {
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
  
protected:

  Table	    table;
  
private:

  // RBTreeBase will prevent these
  //
  // RBSet( const RBSet & from );
  // RBSet & operator =( const RBSet & from );

};

#include <RBSet.ii>


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	RBSet		    class
//
//	RBSet::size_type
//	RBSet::iterator
//	RBSet::const_iterator
//	RBSet::reverse_iterator
//	RBSet::const_reverse_iterator
//	RBSet::pair_iterator_bool
//
//	Note: the iterators are bidirectoinal, so both ++ and -- are
//	    available.
//
//  Constructors:
//
//  	RBSet( MultiMemOffset *	    memMgr,
//	       unsigned short	    treeKey = 0,
//	       bool		    create = false );
//	    Instanciate an RBSet container object.
//		'memMgr' is the memory manager used to allocate,
//		    release, and access records. It is NOT delete(ed)
//		    by the destructor.
//		'treeKey' this should almost always be 0. It is passed
//		    to either MultiMemOffset::setKey() or
//		    MultiMemOffset::getKey() to get/set the tree's
//		    header location within the memMgr's memory range.
//		'create' if this is true, a new tree will be created.
//
//  Public Interface:
//
//	inline
//	pair_iterator_bool
//	insert( const Key & key )
//	    This inserts a copy of 'key' into the tree. If the insert
//	    succeeds, pair_iterator_bool.first will be true and
//	    pair_iterator_bool.second will be an iterator for the newly
//	    inserted value. If it fails, pair_iterator_bool.first will be
//	    false and pair_iterator_bool.second will be either an iterator
//	    to the existing record with the same key, or end() if a memory
//	    management error occured.
//
//	inline
//	const_iterator
//	find( const Key & key ) const
//	    Search the table 'key' and return an iterator to the record
//	    found. If no record is found, end() is returned.
//
//	inline
//	iterator
//	find( const Key & key )
//	    Search the table 'key' and return an iterator to the record
//	    found. If no record is found, end() is returned.
//
//	inline
//	bool
//	erase( const Key & key )
//	    Remove the record that matches 'key' from the table. Returns
//	    true if the record was removed or false if it was not found.
//
//	inline
//	bool
//	erase( const iterator & pos )
//	    Remove the record at 'pos' from the table. Returns
//	    true if the record was removed or false if an error occured.
//
//	inline
//	bool
//	erase( const iterator & first, const iterator & last )
//	    Remove all the recoreds between 'first' and 'last'. The record
//	    at 'first' is removed and the record at 'last' is NOT removed.
//	    Returns true if the records were erased or false if an error
//	    occured.
//
//	inline
//	const_iterator
//	begin( void ) const
//	    Return a const_iterator to the first record in the table or
//	    end() if the table is empty.
//
//	inline
//	const_iterator
//	end( void ) const
//	    Return a const_iterator that is past the last recored in
//	    the table. It points to an invalid value and is used to
//	    verify other const_intreators are valid.
//
//	inline
//	iterator
//	begin( void )
//	    Return an iterator to the first record in the table or
//	    end() if the table is empty.
//
//	inline
//	iterator
//	end( void )
//	    Return an iterator that is past the last recored in
//	    the table. It points to an invalid value and is used to
//	    verify other const_intreators are valid.
//
//	inline
//	const_reverse_iterator
//	rbegin( void ) const
//	    Return a const_reverse_iterator to the first reverse
//	    order recored in the table or rend() if the table is empty.
//
//	inline
//	const_reverse_iterator
//	rend( void ) const
//	    Return a const_reverse_iterator that is past the last
//	    reverse order recored in the table. It points to an
//	    invalid value and is used to verify other
//	    const_reverse_intreators are valid.
//
//	inline
//	reverse_iterator
//	rbegin( void ) 
//	    Return a reverse_iterator to the first reverse
//	    order recored in the table or rend() if the table is empty.
//
//	inline
//	reverse_iterator
//	rend( void )
//	    Return a reverse_iterator that is past the last
//	    reverse order recored in the table. It points to an
//	    invalid value and is used to verify other
//	    const_reverse_intreators are valid.
//
//	inline
//	size_type
//	size( void ) const
//	    Return the number of records in the table.
//
//	inline
//	bool
//	empty( void ) const
//	    Return true if the table is empty, otherwise return false.
//
//	static
//	size_type
//	getNodeSize( void )
//	    Return the number of bytes required to allocate a record.
//	    this will be sizeof( Key ) + the size of the other fields
//	    in an RBTree node.
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
//	inline
//	DumpInfo< RBSet< Key, LessKey > >
//	dump( const char * prefix = "    ", bool showVer = true ) const
//	    return an object that can be passed to operator << ( ostream & )
//	    which will generate the same output as 'dumpInfo'.
//
//	static const ClassVersion version
//	    Class and project version information. (see ClassVersion.hh)
//
// Example: See libMdb/examples/RBSetExTable.hh
//
// See Also:
//
//  libMdb/docs/design/RBSet.txt
//
//  RBTree(3), DRBSet(3), HashSet(3), DashSet(3),
//  MutliMemOffset(3), MutliMemOffsetMapDynamic(3),
//  MapMemDynamicDynamic(3), MapMemDynamciFixed(3)
//
// Files:
//
//  RBSet.hh, RBSet.ii, RBTree.hh, RBTree.ii, libMdb.a
//
// Documented Ver: 2.3
//
// Tested Ver: 2.3
//
// Revision Log:
//
// $Log$
// Revision 2.3  1997/07/25 13:48:08  houghton
// Cleanup.
// Added documentation.
//
// Revision 2.2  1997/07/16 16:37:47  houghton
// Cleanup.
//
// Revision 2.1  1997/07/11 17:37:48  houghton
// Initial Version.
//
//
#endif // ! def _RBSet_hh_ 

