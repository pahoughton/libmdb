#ifndef _MapBatch_hh_
#define _MapBatch_hh_
//
// File:        MapBatch.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Charles B. Reeves - (charles.reeves@wcom.com)
// Created:     07/28/97 19:25
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
#include <MapFile.hh>
#include <DumpInfo.hh>
#include <ClueUtils.hh>
#include <iostream>
#include <Str.hh>



#if defined( MDB_DEBUG )
#define inline
#endif

template <class T>
class MapBatch
{

public:

  typedef T* iterator;
  typedef const T* const_iterator;
  typedef T value_type;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  typedef T & reference;
  typedef const T & const_reference;
  

  // Use this to open an existing file
  MapBatch( const char * filename)
  : map(filename)
    {
      endPos = map.getEnd();
    };

  
  // This one will create a new file
  //  MapBatch( const char * filename, int  s=0);

  iterator append(void)
    {
      if ( endPos + sizeof(value_type) >= map.getEnd() )
	{
	  if (! map.grow( sizeof(value_type) , 0 ))
	    {
	      return(iterator(0));
	    }
	  if ( endPos + sizeof(value_type) >= map.getEnd() )
	    {
	      return(iterator(0));
	    }
	}
      iterator tmp((iterator)endPos);
      endPos += sizeof(value_type);
      return(tmp);
    };
  
  virtual ~MapBatch( void )
    {
      if (  (map.getMode() & ios::out) && endPos)
	{
	  size_type realSize = endPos - map.getBase();
	  map.unmap();
	  truncate(map.getFileName(), realSize);
	}
    }


  const_iterator begin(void) const
    {
      return ( (const_iterator)map.getBase() );
    };
  const_iterator end(void) const
    {
      return ( (iterator)map.getEnd() );
    };
  
  iterator begin(void)
    {
      return ( (iterator) map.getBase() );
    };
  iterator end(void)
    {
      return ( (iterator) map.getEnd() );
    }; 
  
  
 
  virtual bool	    	good( void ) const
    {
      return( true );
    };

  virtual const char * 	error( void ) const
    {
//       static string errStr;
//       errStr = MapBatch<T>::getClassName();
//       if( good() )
// 	{
// 	  errStr += ": ok";
// 	}
//       else
// 	{
//       	  size_t eSize = errStr.size();
	  
//       	  if( eSize == errStr.size() )
//       	    {
// 	      errStr << ": unknown error";
//    	    }
//    	}
//       return( errStr.c_str() );
    };

  virtual const char *	getClassName( void ) const
    {
      return( "MapBatch" );
    };
  
  virtual const char *  getVersion( bool withPrjVer = true ) const
    {
      // return( version.getVer( withPrjVer ) );
    };
  
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const
    {
      if( showVer )
	dest << MapBatch::getClassName() << ":\n"
	     << MapBatch::getVersion() << '\n';

      if( ! MapBatch::good() )
	dest << prefix << "Error: " << MapBatch::error() << '\n';
      else
	dest << prefix << "Good" << '\n';            
      return( dest );
    };

 
 
  static const ClassVersion version;

private:
  
  MapFile map;
  
  MapFile::MapAddr  endPos;

};

#if !defined( inline )
#include <MapBatch.ii>
#else
#undef inline

ostream &
operator << ( ostream & dest, const MapBatch & src );

istream &
operator >> ( istream & src, const MapBatch & dest );


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	MapBatch	class
//
//  Constructors:
//
//  	MapBatch( );
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
//  	    Return the name of this class (i.e. MapBatch )
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
//  	operator <<( ostream & dest, const MapBatch & src );
//
//	istream &
//	operator >> ( istream & src, MapBatch & dest );
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
// Revision 2.1  1997/08/10 19:46:47  houghton
// Initial Version (by Charlie Reeves).
//
//
#endif // ! def _MapBatch_hh_ 

