#ifndef _AppParam_hh_
#define _AppParam_hh_
//
// File:        AppParam.hh
// Desc:        
//
//
//  Data Types: - data types defined by this header
//
//  	AppParam	class
//
//  Constructors:
//
//  	AppParam( );
//
//  Destructors:
//
//  Public Interface:
//
//  	virtual const char *
//  	getClassName( void ) const;
//  	    Return the name of this class (i.e. AppParam )
//
//  	virtual Bool
//  	good( void ) const;
//  	    Returns true if there are no detected errors associated
//  	    with this class, otherwise FALSE.
//
//  	virtual const char *
//  	error( void ) const
//  	    Returns as string description of the state of the class.
//
//  Protected Interface:
//
//  Private Methods:
//
//  Other Associated Functions:
//
//  	ostream &
//  	operator <<( ostream & dest, const AppParam & obj );
//
// Locationor:      Paul Houghton x2309 - (houghton@shoe.wiltel.com)
// Created:     02/02/95 14:18
//
// Revision History:
//              David Lin x6048  05/10/95
//              Deleted all info about Ani, Location and Route
//
// $Log$
// Revision 1.1  1997/07/19 12:52:41  houghton
// Initial Version (work in progess).
//
// Revision 1.3  1996/08/20  00:41:35  ichudov
// AppPAram works correctly.
//
// Revision 1.2  1995/09/27  16:43:33  ichudov
// Changed map file names.
//
// Revision 1.1  1995/08/25  22:12:59  ichudov
// New module added.
//
//

#include <Clue.hh>
#include <Str.hh>

#include <Param.hh>
#include <AppParamSybase.hh>

class AppParam : public AppParamSybase
#define AppParam_PARENT AppParamSybase
{

public:

  AppParam( int * argc, char * argv[], char * ver = 0 );

  const char *	mapDir( void ) const { return mapDirV; };

  const char * NpaNxxKeyFile( void ) const { return NpaNxxKeyFileV; }
  const char * NpaNxxDataFile( void ) const { return NpaNxxDataFileV; }


  int           mapChunkSize( void ) const { return mapChunkSizeV; };
  int	    	progress( void ) const { return progressV; };
  Bool          isLoad( void ) const { return isLoadV; }

  const DateTime & effDateOverride( void ) const { return effDateOverrideV; };

  virtual const char *	getClassName( void ) const;
  virtual Bool	    	good( void ) const;
  virtual const char * 	error( void ) const;
    
protected:

private:

  AppParam( const AppParam & copyFrom );
  AppParam & operator=( const AppParam & assignFrom );

  char *    	mapDirV;

  char *	NpaNxxKeyFileV;
  char *	NpaNxxDataFileV;

  int           mapChunkSizeV;
  int	    	progressV;

  Bool          isLoadV;    

  DateTime      effDateOverrideV;
  time_t t;
};


//
// Inline methods
//

inline
AppParam::AppParam( int * argc, char * argv[], char * ver ) :
  AppParamSybase( argc, argv, ver ),
  effDateOverrideV( time( 0 ) )
{

  mapDirV = 0;
  argString( &mapDirV,
	     "Map file directory override.",
	     "maps",
	     "RATING_MAP_DIR" );
  
  NpaNxxKeyFileV = "loc.npa.nxx.key.map";
  argString( &NpaNxxKeyFileV,
             "NpaNxx key map file name.",
             "npanxxkm",
             "NPANXX_KEY_MAP" );

  NpaNxxDataFileV = "loc.npa.nxx.data.map";
  argString( &NpaNxxDataFileV,
             "NpaNxx data map file name.",
             "npanxxdm",
             "NPANXX_DATA_MAP" );

  mapChunkSizeV = 128;
  argInt( &mapChunkSizeV,
          "Map alNpaNxx chunk size.",
          "mc",
          0,
          0, 4096 );

  progressV = 100;
  argInt( &progressV,
	  "Display progress every 'n' records",
	  "p",
	  "PROGRESS" );

  effDateOverrideV.setHHMMSS( "000000" );
  // effDateOverrideV.addDay( -1 );
  t = effDateOverrideV.getTimeT();

  argDateTime( &t,
               "Effective Date Override",
               "ed",
               "DATE_OVERRIDE" );

  effDateOverrideV.setTimeT( t );

  isLoadV = FALSE;
  argFlag( &isLoadV,
           "Load instead of Delta (purging the tree)",
           "LOAD" );

  if( mapDirV )
    {
      NpaNxxKeyFileV = ChangeFileDir(NpaNxxKeyFileV, mapDirV);
      NpaNxxDataFileV = ChangeFileDir(NpaNxxDataFileV, mapDirV);
    }            

};


#endif // ! def _AppParam_hh_ 









