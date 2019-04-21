## libMdb

[![Test Build Status](https://travis-ci.org/pahoughton/libMdb.png)](https://travis-ci.org/pahoughton/libmdb)

The Mdb library is a collection of classes that provide high
performance real time memory database functionality. These classes
were developed using (draft) ANSI Standard C++ and Unix's mapped
memory system calls.

Just to 'wet your appetite', during one of the performance test
we were able to perform over 250,000 index searches per second on
a 1,000,000 record table.

## Features

NONE

## Install

pre-reqs: g++ libtool automake
[pahoughton/libclue](https://github.com/pahoughton/libclue)

```
git clone https://github.com/pahoughton/libmdb
cd libmdb
autoreconf && configure && make install
```

## Usage

All of the container (i.e. table) classes utilize the exact same
interface as the Standard C++ STL container classes. This should
significantly reduce the learning curve for our C++
developers. The only significant difference between the STL and
libMdb's container classes is that these classes use Mapped memory
for data storage and access.

Using mapped memory provides both persistence and multiple
application access. Once a table is mapped by one application, all
other applications accessing the table will use the same real
memory.

There is one limitation libMdb's container classes has as compared
to the STL. This is that other classes can not be stored in mapped
memory. Only primary data types and structures (i.e. long, short,
char[50], struct Record { ... }) can be stored in the
container. This is because classes contain function addresses that
would not be valid across applications.


As a minimum, all the container classes includes the following
standard methods:

	pair_iterator_bool  insert( const Record & rec)
	bool		        erase( const Record & key )
	iterator	        find( const Record & key )
	const_iterator	    find( const Record & key ) const

	size_type	    size( void )
	bool		    empty( void )

    Most of the classes also have the following methods:

	iterator	        begin( void )
	iterator	        end( void )
	const_iterator	    begin( void ) const
	const_iterator	    end( void ) const
	iterator	        rbegin( void )
	iterator	        rend( void )
	const_iterator	    rbegin( void ) const
	const_iterator	    rend( void ) const

	bool		    erase( iterator pos )
	bool		    erase( iterator first, iterator last )

C++'s template capability is used to specialize the container
classes for a specific data type. For example, the following code
segment shows how a Trunk Group table could be created and used.


    //
	// define the structure of the trunk record
	//
	struct TrunkRec
	{
	  short	    nbrSwt;
	  short	    nbrTnkGrpExt;
	  short	    idnSwt;
	  short	    npaHom;
	  short	    nxxHom;
	  char	    abbOwnCom[8 + 1];
	};

	//
	// I'm going to store this in a hash table, so a hash function
	// is needed
	//
	class HashTrunk
	{
	public:

	   HashTableBase::HashValue
	   operator () ( const TrunkRec & rec ) const {
	     return((rec.nbrSwt * 10000) + rec.nbrTnkGrpExt );
	   };
	};

	//
	// Define how to determine one trunk record is less that
	// another trunk record.
	//
	inline bool
	operator () ( const TrunkRec & a, const TrunkRec & b )
	{
	  return( a.nbrSwt < b.nbrSwt ? true :
	          a.nbrTnkGrpExt < b.nbrTnkGrpExt );
	}

	//
	// Define the trunk table that uses a hash table index.
	//

	typedef HashSet< TrunkRec, HashTrunk, less< TrunkRecc > > TrunkTable

	//
	// Here's another example that defines a trunk table using
	// an Red Black Binary tree for indexing.
	//

	typedef RBSet< TrunkRec, less< TrunkRec > > TrunkTable

	//
	// Instantiate the mapped memory manager for the Trunk Table.
	//
	MultiMemOffsetMapDynamic mmo( new MapMemDynamicDynamic(
					"TrunkGroup.rbs",
					(ios::open_mode)(ios::in|ios::out) ) );

	//
	// Instantiate the trunk table.
	//
	TrunkTable  trunks( &mmo );

	//
	// Now that everything is defined, you could perform any of
	// the following operations.
	//

	TrunkRec rec;

	if( ! trunks.insert( rec ).second )
	  {
	     // failed
	  }

	TrunkTable::iterator it = trunks.find( rec );

	if( it != trunks.end() )
  	  {
	    cout << (*it).abbOwnCom << endl;
	  }
	else
  	  {
	     // didn't find it
	  }

	if( ! trunks.erase( rec ) )
	  {
	     // failed.
	  }

	// do something with all the records.

	for( TrunkTable::iterator them = trunks.begin();
	     them != trunks.end();
	     ++ them )
	  {
	     cout << (*them).nbrSwt << " "
	          << (*them).nbrTnkGrpExt << endl;
	  }


## Classes

### Container Classes

#### RBSet< class Rec, class LessRec >

The RBSet<> template class is a Red Black Binary Tree that can
use mapped memory for data storage and access. It's interface
mimics the Standard C++ STL set<> template class. The only
significant difference is the constructor which requires a
MultiMemOffset object for memory management.

#### DRBSet< class Rec, class LessRec >

#### HashSet< class Rec, class HashRec, class LessRec >

#### DashSet< class Rec, class HashRec, class LessRec >

### Mapped Memory Classes

There are many mapped memory classes in libMdb. This allows for a
greater degree of flexibility and reuse.

When deciding exactly which combination of classes to use for a
specific Memory Database Table, there are a two primary factors to
consider. The most significant of these is where to use fixed
or dynamic addressing.

Fixed addressing is faster that dynamic, but forces an application
to pre-determine the maximum size of a table. It also creates an
operating system dependency, because each OS uses different
addresses for memory mapped files. For now, the fixed addressing
capability has been designed for, but the classes have not yet
been developed.

The item to decide, is whether to use fixed or dynamic sized
record chunks. Each has specific advantages and disadvantages.

The advantages of using dynamic size chucks are that each chunk
allocated can be any size, and the average release performance is
slightly faster. The disadvantage is that each allocated chunk
requires 8 additional bytes of space, where fixed size chunks have
no per chunk overhead.

There are three basic categories of mapped memory classes in
libMdb. These are the Interface Classes used by the container
classes, the Chunk Management Classes used by the Interface Classes,
and the Mapped Memory class used by the Chunk Management Classes.

### Memory Interface Classes

The Memory Interface classes define the interface for
allocating and releasing chunks of memory. This also provides an
offset to address translation and access to 16 specialized
'key' values.

All of these classes have the following methods.

    allocate( chunkSize );
    release( chunk );

    long    getKey( unsigned short key );
    long    setKey( unsigned short key, long value );

#### MultiMemOffset

This is base class for offset based (vs address based)
memory management. It defines the interface that is used
by the offset based container classes. It has pure virtual
methods, so it can not be directly instantiated.

#### MultiMemOffsetMapDynamic

This is a MultiMemOffset that uses a MapMemDynamic for
chunk mangement.

### Chunk Management Classes

These classes manage the allocation and release of memory
chunks from a MapFile.

#### MapMem

#### MapMemDynamic

#### MapMemDynamicDynamic

#### MapMemDynamicFixed


### Mapped Memory Class

#### MapFile

MapFile uses the operating system's mapped memory
functions (mmap(2) & munmap(2)) to map a file to memory. A
file can be mapped to a specific memory address or the
operating system can select the address to use.

## Contribute

[Github pahoughton/libMdb](https://github.com/pahoughton/libmdb)

## Licenses

1995-02-13 (cc) Paul Houghton <paul4hough@gmail.com>

[![LICENSE](http://i.creativecommons.org/l/by/4.0/88x31.png)](http://creativecommons.org/licenses/by/4.0/)

[![endorse](https://api.coderwall.com/pahoughton/endorsecount.png)](https://coderwall.com/pahoughton)
