#ifndef _RBTreeBase_hh_
#define _RBTreeBase_hh_
//
// File:        RBTreeBase.hh
// Project:	Mdb
// Desc:        
//
//
//
// Quick Start: - short example of class usage
//
// Author:      Paul A. Houghton - (paul.houghton@wcom.com)
// Created:     05/07/97 19:28
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
#include <MultiMemOffset.hh>
#include <Str.hh>
#include <iostream>

#if defined( MDB_DEBUG )
#define inline
#endif


class RBTreeBase
{

public:

  typedef MultiMemOffset::Loc		Loc;
  typedef MultiMemOffset::size_type	size_type;
  
  enum Color
  {
    Red,
    Black
  };

  struct RBNodeBase
  {
    Color   color;
    Loc	    parent;
    Loc	    left;
    Loc	    right;
  };

  class ConstNodeBase
  {
  public:

    inline ConstNodeBase( const MultiMemOffset & memMgr, Loc loc );
    
    inline Loc		loc( void ) const;
    inline Loc		parent( void ) const;
    inline Loc		left( void ) const;
    inline Loc		right( void ) const;
    inline Color	color( void ) const;

    inline void	    next( void );
    inline void	    prev( void );
    
    inline ConstNodeBase &  operator ++ ( void );
    inline ConstNodeBase &  operator -- ( void );

    inline ConstNodeBase &  operator = ( Loc rhs );
      
  protected:

    inline const RBNodeBase &	rbNode( void ) const;
    
    Loc			nodeLoc;

  private:
    
    const MultiMemOffset &	mgr;

  };
  
  class NodeBase : public ConstNodeBase
  {
  public:

    inline NodeBase( MultiMemOffset & memMgr, Loc nodeLoc );

    inline Loc &	parent( void );
    inline Loc &	left( void );
    inline Loc &	right( void );
    inline Color &	color( void );
    
    inline void		set( Loc p, Loc l, Loc r, Color c );
    
    inline NodeBase &	operator =  ( Loc rhs );
    
    inline NodeBase &  operator ++ ( void );
    inline NodeBase &  operator -- ( void );
    
  protected:
    
    inline RBNodeBase &	rbNode( void );
    
    MultiMemOffset &		    mgr;
    
  private:

  };
  
  struct RBTreeHeader : RBNodeBase
  {
    unsigned long   version;
    unsigned long   count;
  };
  
  RBTreeBase( MultiMemOffset *	memMgr,
	      unsigned short	treeKey,
	      bool		create );

  virtual ~RBTreeBase( void );

  inline size_type	size( void ) const;
  inline bool		empty( void ) const;
  
  virtual bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
  virtual const char *	getClassName( void ) const;
  virtual const char *  getVersion( bool withPrjVer = true ) const;
  virtual ostream &     dumpInfo( ostream &	dest = cerr,
				  const char *  prefix = "    ",
                                  bool          showVer = true ) const;

  
  static const ClassVersion version;


  class DumpMethods
  {
  public:

    virtual ostream &	dumpNode( ostream & dest,
				  const Loc STLUTILS_UNUSED( node ) ) const {
      return( dest );
    };
  };
  
  ostream &	dumpRBTree( ostream & dest, const DumpMethods & meth ) const;
  bool		testNode( ostream & dest, const ConstNodeBase & node ) const;
  bool		testTree( ostream & dest ) const;
  bool		allTested( void ) const;
  void		resetTested( void ) const;
    
protected:

  virtual bool	lessKey( Loc one, Loc two ) const = 0;

  Loc	    insertNode( const NodeBase & c, NodeBase & p, NodeBase & n );
  Loc	    insert( Loc nodeLoc );

  bool	    erase( Loc nodeLoc );
  
  inline RBTreeHeader &		header( void );
  inline const RBTreeHeader &   header( void ) const;

  inline Loc &		    root( void );
  inline Loc		    root( void ) const;
  
  inline Loc &		    first( void );
  inline Loc		    first( void ) const;
  
  inline Loc &		    last( void );
  inline Loc		    last( void ) const;

  inline Loc		    next( Loc & node ) const;
  inline Loc		    prev( Loc & node ) const;
  
  inline NodeBase	    parent( const ConstNodeBase & node );
  inline NodeBase	    parent( Loc loc );
  inline ConstNodeBase	    parent( const ConstNodeBase & node ) const;
  inline ConstNodeBase	    parent( Loc loc ) const;
  
  inline NodeBase	    left( const ConstNodeBase & node );
  inline NodeBase	    left( Loc loc );
  inline ConstNodeBase	    left( const ConstNodeBase & node ) const;
  inline ConstNodeBase	    left( Loc loc ) const;
  
  inline NodeBase	    right( const ConstNodeBase & node );
  inline NodeBase	    right( Loc loc );
  inline ConstNodeBase	    right( const ConstNodeBase & node ) const;
  inline ConstNodeBase	    right( Loc loc ) const;

  inline bool		    setNode( Loc loc, Loc p, Loc l, Loc r, Color c );

  inline NodeBase	    minimum( const NodeBase & node );
  inline NodeBase	    maximum( const NodeBase & node );
  
  void		rotate_left( Loc loc );
  void		rotate_right( Loc loc );
  
  enum ErrorNum
  {
    E_OK,
    E_VERSION,
    E_BADTREEKEY,
    E_UNDEFINED
  };

  bool	setError( ErrorNum err );
  
  MultiMemOffset *  mgr;
  Loc		    headerLoc;
  ErrorNum	    errorNum;
  
  static Str errStr;
  
private:

  RBTreeBase( const RBTreeBase & from );
  RBTreeBase & operator =( const RBTreeBase & from );

  static const unsigned long rbTreeVersion;
  
};

#if !defined( inline )
#include <RBTreeBase.ii>
#else
#undef inline


#endif


//
// Detail Documentation
//
//  Data Types: - data types defined by this header
//
//  	RBTreeBase	class
//
//  Constructors:
//
//  	RBTreeBase( );
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
//  	    Return the name of this class (i.e. RBTreeBase )
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
//  	operator <<( ostream & dest, const RBTreeBase & src );
//
//	istream &
//	operator >> ( istream & src, RBTreeBase & dest );
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
// Revision 2.6  1997/10/01 14:03:33  houghton
// Chaged to reserve 'keys' set.
// Changed to use portable multi platform types.
//
// Revision 2.5  1997/09/17 16:56:10  houghton
// Changed for new library rename to StlUtils
//
// Revision 2.4  1997/08/18 10:23:57  houghton
// Port(Sun5): had to add static errStr to be used by template sub
//     classes (DBTree). The sun compiler gets a dup symbol error.
//
// Revision 2.3  1997/07/25 13:48:48  houghton
// Cleanup.
//
// Revision 2.2  1997/07/13 11:33:15  houghton
// Cleanup.
// Changed to use MultiMemOffset.
// Added size() & empty().
// Added testing & debuging methods().
//
// Revision 2.1  1997/06/05 11:29:14  houghton
// Initial Version.
//
//
#endif // ! def _RBTreeBase_hh_ 

