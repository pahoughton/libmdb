#ifndef _DRBSet_hh_
#define _DRBSet_hh_
//
// File:        DRBSet.hh
// Project:	Mdb
// Desc:        
//
//  This class is a Red Black Binary Tree for a single data type (useually
//  a struct) that can store multible instances of a key with different
//  effective dates. It can use mapped memory for data storage and
//  access. It's interaface conforms to the C++ Standard for container
//  classes
//
//  The effective date is the beginning of the period of time that the key
//  value is in effect. For example, if you insert a key with an effective
//  date of 7/16/97 and one with a effective date of 6/1/97, the second 
//  key will be found for all dates from 6/1/97 through 7/15/97. The first
//  one will be found for all dates on or after 7/16/97. If there are no
//  other records with the same key and you try to find a record with and
//  effective date before 6/1/97, end() would be returned.
//
//  The erase method does not actually remove records from the table.
//  It just sets a marker for the period between the effective date
//  it is given and the next effective date for that key. The  trim
//  method actuall removes records.
//
// Notes:
//
//  The destructor does NOT delete the MultiMemOffset object.
//
//  Most of the functionallity provided comes from the DRBTree class.
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
  
  inline const_iterator		find( const Key & key, EffDate eff ) const {
    return( table.find( key, eff ) );
  };

  inline iterator		find( const Key & key, EffDate eff ) {
    return( table.find( key, eff ) );
  };

  inline const_iterator		lower_bound( const Key & key,
					     EffDate eff ) const {
    return( table.lower_bound( key, eff ) );
  };
  
  inline bool			erase( const Key & key, EffDate eff ) {
    return( table.erase( key, eff ) );
  };

  inline bool			erase( const iterator & pos, EffDate eff ) {
    return( table.erase( pos, eff ) );
  };

  inline bool			erase( const iterator & first,
				       const iterator & last,
				       EffDate		eff ) {
    return( table.erase( first, last, eff ) );
  };

  inline bool			trim(  const Key & key, EffDate eff ) {
    return( table.trim( key, eff ) );
  };

  inline bool			trim( EffDate eff ) {
    return( table.trim( eff ) );
  };

  inline EffDate		effective( const const_iterator & it ) const {
    return( table.effective( it ) );
  };
  
  inline const_iterator		begin( void ) const { return(table.begin()); };
  inline const_iterator		end( void ) const { return( table.end() ); };

  inline iterator		begin( void ) { return( table.begin() ); };
  inline iterator		end( void ) { return( table.end() ); };
  
  inline reverse_iterator	rbegin( void ) { return( table.rbegin() ); };
  inline reverse_iterator	rend( void ) { return( table.rend() ); };

  inline const_reverse_iterator	rbegin( void ) const {return(table.rbegin());};
  inline const_reverse_iterator	rend( void ) const {return(table.rend());};

  inline size_type		size( void ) const { return( table.size() ); };
  inline bool			empty( void ) const { return(table.empty()); };

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

  Table	    table;
  
private:

  // DRBSet( const DRBSet & from );
  // DRBSet & operator =( const DRBSet & from );

};

#include <DRBSet.ii>


// Detail Documentation
//
//  Data Types: data types defined by this header
//
//  	DRBSet	class
//
//	Table:
//	    This uses the DRBTree template class to provide most of the
//	    functionallity.
//
//	size_type:		used for size values.
//	iterator:		a bidirectional iterator.
//	const_iterator:		a const bidirectional iterator.
//	reverse_iterator:	a bidirectional iterator.
//	const_revers_iterator:	a const bidirectional iterator.
//	pair_iterator_bool:	an iterator and a bool value.
//
//	EffDate:
//	    This is the type used for effective dates (long). I designed
//	    to use valid time_t values, but it can actually be any number.
//	    It represents the beginning of a period of time that ends
//	    at the EffDate of the next record with a larger EffDate and
//	    with the same key.
//
//  Constructors:
//
//  	DRBSet( MultiMemOffset *    memMgr,
//		MultiMemOffset *    histMgr,
//	        unsigned short	    treeKey = 0,
//	        bool		    create = false );
//	    Instanciate an RBSet container object.
//		'memMgr' is the memory manager used to allocate,
//		    release, and access index records. It is NOT delete(ed)
//		    by the destructor.
//		'histMgr' is the memory manager used to allocate,
//		    release, and access history records. It is NOT delete(ed)
//		    by the destructor. It can be the same instance as the
//		    memMgr.
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
//	insert( const Key & key, EffDate eff )
//	    This inserts a copy of 'key' with an effective date of 'eff'
//	    into the tree. If the insert succeeds, pair_iterator_bool.first
//	    will be true and pair_iterator_bool.second will be an iterator
//	    for the newly inserted value. If it fails,
//	    pair_iterator_bool.first will be false and
//	    pair_iterator_bool.second will be end() if a memory
//	    management error occured.
//
//	inline
//	const_iterator
//	find( const Key & key, EffDate eff ) const
//	    Search the table 'key' that is in effect for at 'eff' and
//	    return an const_iterator to the record found. If no record
//	    is found, end() is returned.
//
//	inline
//	iterator
//	find( const Key & key, EffDate eff )
//	    Search the table 'key' that is in effect for at 'eff' and
//	    return an iterator to the record found. If no record is
//	    found, end() is returned.
//
//	inline
//	bool
//	erase( const Key & key, EffDate eff )
//	    Add a record to the table that indicates the 'key' is
//	    NOT valid for the period of time starting at 'eff'.
//	    Returns true if successful or false if 'key' was not found.
//	    See trim() to actually remove records from the table.
//
//	inline
//	bool
//	erase( const iterator & pos, EffDate eff )
//	    Add a record to the table that indicates the value at 'pos'
//	    is NOT valid for the period of time starting at 'eff'.
//	    Returns true if successful or false if there was an error.
//	    See trim() to actually remove records from the table.
//
//	inline
//	bool
//	erase( const iterator & first, const iterator & last, EffDate eff )
//	    Add records to the table that indicates all values between
//	    'first' and 'last' are NOT valid for the period of time
//	    starting at 'eff'. Returns true if successful or false if an
//	    error occured. See trim() to actually remove records from
//	    the table.
//
//	inline
//	bool
//	trim( const Key & key, EffDate eff )
//	    Remove the history records for 'key' that are before 'eff'.
//	    The record that is in effect at 'eff' is NOT removed unless
//	    it is an 'erase()'ed record. This is because the record is
//	    the one that is in effect after 'eff'. If the newest record
//	    (the one with the largest EffDate value) is before 'eff' and
//	    it is an 'erase()'ed record, the entire history for 'key'
//	    will be removed. Returns true if successful or false if an
//	    error occured.
//	
//	inline
//	bool
//	trim( EffDate eff )
//	    Remove all history records that are before 'eff'. The records
//	    that are in effect at 'eff' are NOT removed unless
//	    they are 'erase()'ed records. This is because those records
//	    are the one that are in effect after 'eff'. If the newest 
//	    record (the one with the largest EffDate value) for a 'key'
//	    is before 'eff' and it is an 'erase()'ed record, the entire
//	    history for that 'key' will be removed. Returns true if
//	    successful or false if an error occured.
//	
//	inline
//	const_iterator
//	begin( void ) const
//	    Return a const_iterator to the first record in the table or
//	    end() if the table is empty. The first record is the one with
//	    the smallest key and the largest EffDate (i.e. newest).
//	    As you iterate through the records use operator ++, you
//	    will first get the entire history for one key from newest
//	    to oldest. Then you will get the newest record for the next key.
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
//	    Return a iterator to the first record in the table or
//	    end() if the table is empty. The first record is the one with
//	    the smallest key and the largest EffDate (i.e. newest).
//	    As you iterate through the records using operator ++, you
//	    will first get the entire history for one key from newest
//	    to oldest. Then you will get the newest record for the next key.
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
//	    This record will be the one with the largest key and the
//	    smallest EffDate (i.e. oldest). As you iterate through the
//	    records using operatore ++, you wil first get the entire
//	    history for one key from oldest to newest. Then you will
//	    get the newest record for the next key.
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
//	    This record will be the one with the largest key and the
//	    smallest EffDate (i.e. oldest). As you iterate through the
//	    records using operatore ++, you wil first get the entire
//	    history for one key from oldest to newest. Then you will
//	    get the newest record for the next key.
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
//	inline
//	DumpInfo< RBSet< Key, LessKey > >
//	dump( const char * prefix = "    ", bool showVer = true ) const
//	    return an object that can be passed to operator << ( ostream & )
//	    which will generate the same output as 'dumpInfo'.
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
// Example: See libMdb/examples/DRBSetExTable.hh
//
// See Also:
//
//  libMdb/docs/design/DRBSet.txt
//
//  DRBTRee(3), RBSet(3), DashSet(3), HashSet(3),
//  MutliMemOffset(3), MutliMemOffsetMapDynamic(3),
//  MapMemDynamicDynamic(3), MapMemDynamciFixed(3)
//
// Files:
//
//  DRBSet.hh, DRBSet.ii, DRBTree.hh, DRBTree.ii, libMdb.a
//
// Documented Ver: 2.3
//
// Tested Ver:
//
// Revision Log:
//
// $Log$
// Revision 2.5  1997/08/17 01:37:41  houghton
// Added lower_bound.
// Added effective.
//
// Revision 2.4  1997/07/25 15:56:04  houghton
// Bug-Fix: lest chaged to last.
//
// Revision 2.3  1997/07/25 13:43:15  houghton
// Cleanup.
// Added documentation.
//
// Revision 2.2  1997/07/22 19:41:16  houghton
// Cleanup.
// Bug-Fix: fixed method args on some of the methods.
//
// Revision 2.1  1997/07/16 16:36:54  houghton
// Initial Version (work in progress).
//
//
#endif // ! def _DRBSet_hh_ 

