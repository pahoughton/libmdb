#ifndef _HashSet_hh_
#define _HashSet_hh_
//
// File:        HashSet.hh
// Project:	Mdb
// Desc:        
//
//  This template class provides a hash index for a single data type
//  (ususally a struct) that can use mapped memory for data storage and
//  access. It's interaface conforms to the C++ Standard for container
//  classes. It stores the hash index in a MapFile (indexFile) and the
//  data in the MultiMemOffset passed to it.
//
// Notes:
//
//  The destructor does NOT delete the MultiMemOffset object.
//
//  Most of the functionallity provided comes from the HashTable class.
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     06/02/97 05:13
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
#include <HashTable.hh>
#include <DumpInfo.hh>
#include <iostream>

template< class Key,
	  class HashFunct,
	  class LessKey >
class HashSet
{

public:

  typedef HashTable< Key, Key, MdbIdent< Key, Key >, HashFunct, LessKey > Table;
  typedef typename Table::size_type		size_type;
  typedef typename Table::iterator		iterator;
  typedef typename Table::const_iterator		const_iterator;
  typedef typename Table::reverse_iterator	reverse_iterator;
  typedef typename Table::const_reverse_iterator	const_reverse_iterator;
  typedef typename Table::pair_iterator_bool	pair_iterator_bool;

  inline HashSet( MultiMemOffset *  memMgr,
		  const char *	    indexFileName,
		  ios::open_mode    mode = ios::in,
		  bool		    create = false,
		  unsigned short    permMask = 02 );
  
  virtual ~HashSet( void );

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
  
  inline const_iterator		begin( void ) const { return(table.begin()); };
  inline const_iterator		end( void ) const { return( table.end() ); };

  inline iterator		begin( void ) { return( table.begin() ); };
  inline iterator		end( void ) { return( table.end() ); };
  
  inline const_reverse_iterator	rbegin( void ) const {return(table.rbegin());};
  inline const_reverse_iterator	rend( void ) const {return(table.rend());};

  inline reverse_iterator	rbegin( void ) { return( table.rbegin() ); };
  inline reverse_iterator	rend( void ) { return( table.rend() ); };

  inline size_type		size( void ) const { return( table.size() ); };
  inline bool			empty( void ) const { return(table.empty()); };

  static size_type	    getNodeSize( void ) {
    return( Table::getNodeSize() );
  };
  
  virtual bool		good( void ) const;
  virtual const char *	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  inline
  DumpInfo< HashSet< Key, HashFunct, LessKey > >
  dump( const char *  prefix = "    ",
	bool	    showVer = true ) const {
    return( DumpInfo< HashSet< Key, HashFunct, LessKey> >(
      *this, prefix, showVer ) );
  };

  // Debuging & Testing methods
  inline ostream &	dumpTable( ostream & dest ) const;

  virtual ostream &	dumpKey( ostream & dest, const Key & key ) const;
  virtual ostream &	dumpValue( ostream & dest, const Key & value ) const;

  class HashSetDumpMethods : public Table::TableDumpMethods
  {
  public:
    HashSetDumpMethods( const Table & table,
		     const HashSet< Key, HashFunct, LessKey > & me ) :
      Table::TableDumpMethods( table ), self( me ) {};

    ostream &  dumpKey( ostream & dest, const Key & key ) const {
      return( self.dumpKey( dest, key ) );
    };
    
    ostream &  dumpValue( ostream & dest, const Key & value ) const {
      return( self.dumpValue( dest, value ) );
    };

  private:
    const HashSet< Key, HashFunct, LessKey > & self;
    
  };
  
protected:

  Table		table;
  
private:

  // HashTable will prevent these
  //
  // HashSet( const HashSet & from );
  // HashSet & operator =( const HashSet & from );

};

#include <HashSet.ii>

//
// Detail Documentation
//
//  Data Types:
//
//  	HashSet< Key, HashFunct, LessKey >:	    template class
//
//	    This is the primary template class defined by this header.
//	    It uses the following three types:
//
//	    'Key' is the data type being indexed and stored. It should
//		be struct. It must not contain a virtual table.
//
//	    'HashFunct' is a class that returns a
//		'HashTableBase::HashValue' for a 'Key' value.
//
//	    'LessKey' is a class that takes two 'Key' values and compares
//		them. If the first is less that the second, true should
//		be returned.
//
//	Table:
//	    This uses the HashTable template class to provide most of the
//	    functionallity.
//
//	size_type:		used for size values.
//	iterator:		a bidirectional iterator.
//	const_iterator:		a const bidirectional iterator.
//	reverse_iterator:	a bidirectional iterator.
//	const_revers_iterator:	a const bidirectional iterator.
//	pair_iterator_bool:	an iterator and a bool value.
//
//  Constructors:
//
//  	HashSet( MultiMemOffset *   memMgr,
//	         const char *	    indexFileName,
//	         ios::open_mode	    mode = ios::in,
//	         bool		    create = false,
//	         unsigned short	    permMask = 02 )
//	    Construct a 'HashSet' object that will either use existing
//	    data maps or create empty ones.
//		'memMgr' is the object used to store and manage the
//		    'Key' ojects.
//		'indexFileName' is the name of the MapFile to use for
//		    the Hash index.
//		'mode' is the mode for the index, ios::in == read only
//		    (ios::in | ios::out) == read/write (note if create == true
//		    the mode argument is ignored and the file is opened
//		    with (ios::in | ios::out) ).
//		'create' if this is true, a new empty index will be created.
//		    Any existing file will be removed.
//		'permMask' is the 'umask(2)' to use when creating the
//		    index file. A value of '02' will create a file with
//		    the mode set to '-rw-rw-r--'.
//
//  Destructors:
//
//	~HashSet()
//	    The destructor will close the indexFile, but it does NOT
//	    delete (or do anything else with) the 'memMgr'.
//
//  Public Interface:
//
//	inline
//	pair_iterator_bool
//	insert( cosnt Key & key )
//	    Insert a new 'key' into the hash table. Duplicate keys
//	    (according to LessKey) are inserted. On return,
//	    pair_iterator_bool.second will be true if the value was
//	    inserted. pair_iterator_bool.first will contain a iterator
//	    for the inserted value. 
//
//	inline
//	iterator
//	find( const Key & key )
//	    Find 'key' in the hash table and return an iterator to it.
//	    If 'key' is not found, end() is returned.
//
//	inline
//	const_iterator
//	find( const Key & key ) const
//	    Find 'key' in the hash table and return a const_iterator to it.
//	    If 'key' is not found, end() is returned.
//
//	inline
//	bool
//	erase( const Key & key )
//	    Removes the value. 'true' is returned if the value is removed.
//
//	inline
//	bool
//	erase( const iterator & pos )
//	    Removes the value at 'pos'. 'true' is returned if the value is
//	    removed.
//
//	inline
//	bool
//	erase( const iterator & first, const iterator & last )
//	    Removes the first through last values. 'first' is removed and
//	    'last' is not removed. Returns 'true' if the values were
//	    removed.
//
//	inline
//	iterator
//	begin( void )
//	    Returns an iterator to the first value in the table.
//
//	inline
//	iterator
//	end( void )
//	    Returns an invalid iterator that should be used to verify the
//	    iterator returned by find and when incrementing the iterator
//	    returned by begin.
//
//	inline
//	const_iterator
//	begin( void ) const
//	    Returns a const_iterator to the first value in the table.
//
//	inline
//	const_iterator
//	end( void ) const
//	    Returns an invalid iterator that should be used to verify the
//	    iterator returned by find and when incrementing the iterator
//	    returned by begin.
//
//	inline
//	iterator
//	rbegin( void )
//	    Returns an iterator to the last value in the table.
//
//	inline
//	iterator
//	rend( void )
//	    Returns an invalid iterator that should be used to verify the
//	    iterator when incrementing the iterator returned by rbegin.
//
//	inline
//	const_iterator
//	rbegin( void ) const
//	    Returns an iterator to the last value in the table.
//
//	inline
//	const_iterator
//	rend( void ) const
//	    Returns an invalid iterator that should be used to verify the
//	    iterator when incrementing the iterator returned by rbegin.
//
//	inline
//	size_type
//	size( void ) const
//	    Return the number of entries in the table.
//
//	inline
//	bool
//	empty( void ) const
//	    Returns true if size() == 0.
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
//  	    Return the name of this class (i.e. HashSet )
//
//  	virtual const char *
//  	getVersion( bool withPrjVer = true ) const;
//  	    Return the version string of this class.
//
//	virtual ostream &
//	dumpInfo( ostream & dest, const char * prefix, bool showVer ) const
//	    output detail info to dest. Includes instance variable
//	    values, state info & version info.
//
//	inline
//	DumpInfo< MapMemDynamicDynamic >
//	dump( const char * previx, bool showVer = true ) const
//	    return an object that can be passed to operator << ( ostream & )
//	    that will generate the same output as 'dumpInfo'.
//
//	inline
//	ostream &
//	dumpTable( ostream & dest ) const
//	    Outputs the structure of the table to 'dest'.
//
//	inline
//	ostream &
//	dumpKey( ostream & dest, const Key & key ) const
//	    Override this method in a sub class to output the key value(s)
//	    with the table structure output by dumpTable.
//
//	inline
//	ostream &
//	dumpValue( ostream & dest, const Key & key ) const
//	    Override this method in a sub class to output the
//	    non-key values with the table structure output by dumpTable.
//
//  Protected Interface:
//
//  Private Methods:
//
//  Associated Functions:
//
// Example:
//
//	struct ExRec
//	{
//	  long	key;	    
//	  short	subKey;	    
//	  char	data[16];
//	};
//	
//	struct ExHashFunct
//	{
//	  HashTableBase::HashValue operator () ( const ExRec & rec ) const {
//	    return( rec.key );
//	  };
//	};
//	
//	struct ExLessKey
//	{
//	  bool	    operator () ( const ExRec & one,
//				  const ExRec & two ) const {
//	    return( one.key == two.key ?
//		    one.subKey < two.subKey :
//		    one.key < two.key );
//	  };
//	};
//	
//	typedef HashSet< ExRec, ExHashFunct, ExLessKey >	ExTable;
//
//
// See Also:
//
//	HashTable(3), HashTableBase(3),
//	DashSet(3), DashTable(3), DashTableBase(3),
//	MapFile(3), MultiMemOffset(3), MultiMemOffsetMapDynamic(3),
//	MapMemDynamicDynamic(3), MapMemDynamicFixed(3)
//
//	libMdb/docs/design/HashSet.txt
//	libMdb/docs/design/HashTable.txt
//	libMdb/docs/design/HashTableBase.txt
// 
// Files:
//
//	HashSet.hh, HashSet.ii,
//	HashTable.hh, HashTable.ii,
//	HashTableBase.hh, HashTableBase.ii,
//	MapFile.hh, MapFile.ii,
//	MutliMemOffset.hh, MultiMemOffset.ii
//	libMdb.a
//
// Documented Ver: 2.2
//
// Tested Ver: 2.2
//
// Revision Log:
//
// $Log$
// Revision 4.2  2002/02/28 15:12:51  houghton
// Port(Forte 6.2) Added typename to eliminate warnings.
//
// Revision 4.1  2001/07/27 00:57:43  houghton
// Change Major Version to 4
//
// Revision 2.4  1997/07/25 13:44:49  houghton
// Cleanup.
// Class was renamed to DashSet (was Dash).
//
// Revision 2.3  1997/07/19 10:21:18  houghton
// Cleanup.
//
// Revision 2.2  1997/07/13 11:09:59  houghton
// Cleanup.
// Finished coding.
//
// Revision 2.1  1997/06/05 11:29:10  houghton
// Initial Version.
//
//
#endif // ! def _HashSet_hh_ 

