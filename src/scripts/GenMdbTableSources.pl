eval 'exec perl $0 ${1+"$@"}'
    if 0 ;
use warnings;
use strict;
package GenMakefile;

our %Doc;
$Doc{ File }	=   "GenMdbTableSources.pl";
$Doc{ Project } =   ["Mdb",""];
$Doc{ Item }   	=   "";
$Doc{ desc } = "Generate Mdb Table, Query, and Loader sources..";

$Doc{ Description } = "

      GenMdbTableSources will create C++ sources for a Memory database
      table sources from a copybook (-cb) or record layout
      report (-rlr). The table sources will be written to
      libPRJTables, the QuerySources will be written to TABLEQuery and
      the loader sources will be written to libPRJLoader.

      You will need to edit the sources to meet the specific needs of
      the table. I have included '// FIXME' comment at MOST of the
      location you will most likely edit.
";
$Doc{ Notes } = "

   For the rest of this programs documentation, either run it
   with -man or see `Detailed Documentation' below.
";
$Doc{ Author } =  [["Paul Houghton","<paul.houghton\@wcom.com>"]];
$Doc{ Created } = "04/10/03";

$Doc{ Last_Mod_By } = '$Author$ ';
$Doc{ Last_Mod }    = '$Date$ ';
$Doc{ Version }	    = '$Revision$ ';
$Doc{ Status }	    = '$Status:$ ';

$Doc{ VersionId }
  = '$Id$ ';

$Doc{ VERSION } = "+FULLVERSION+";

#
# Revision History: (See end of file for Revision Log)
#

#
# This is the Options array. It contains the command line
# option information used for both documentation and
# the processing of command line options. For more information,
# see the App::Options package.
#
our @Options =
  (
   [ "help+",		undef,
			"",	    "opt",
			"Output usage information.",
			"The amount of information increases each"
			." time it appears on the command line."
			." The first instance just outputs the available"
			." command line arguments. Successive instances"
			." (i.e. -help -help -help) provide additional"
			." information up to 4 which output the entire"
			." program documentation." ],

   [ "man",		undef,
			"",	    "opt",
			"Output the entire program documentation.",
			"This is just a short cut for using -help 4 times" ],

   [ "nopager",		undef,
			"",	    "opt",
			"Don NOT use pager for help output",
			"When ever the help (or man) text has more lines"
			." that your terminal window, the default pager"
			." is used to keep the text from scrolling off"
			." the screen. To keep the pager from being used,"
			." put this option on the command line." ],

   [ "show-opts",	undef,
			"",	    "opt",
			"Show option and env values then exit.",
			"Output the command line options and their respective"
			." current values. Also output any relevant"
			." environment variables and their respective"
			." values." ],

   [ "debug+",		undef,
			"",	    "opt",
			"Output debug information.",
			"Increments the amount of debug information"
			." each time it is found in the arguments. So,"
			." no --debug sets debug level to 0 (none),"
			." one -debug sets it to 1 and -debug -debug"
			." sets the debug level to 2. The higher the"
			." level, the more debug information is"
			." output." ],
   [ "version",		undef,
			"",	    "opt",
			"Show version and exit.",
			"Output the program's version information"
			." then exit" ],
   [ "cb=s",		undef,
			"FILE",	    "opt",
			"copybook file name",
			"The copybook to generate the table's record"
			." layout from." ],
   [ "rlr=s",		undef,
			"FILE",	    "opt",
			"record layout report file",
			"The record layout report to generate the table's"
			." record structure from."],
   [ "prj=s",		undef,
			"PROJECT",  "req",
			"project name",
			"The name of the project this table is for."],
   [ "name=s",		undef,
			"NAME",	    "req",
			"table name.",
			"The name to use for the Table. This should be mixed"
			." case such as 'TrunkGroup'."],
   [ "prefix=s",		undef,
			"STRING", "opt",
			"field name prefix to strip",
			"Any prefix to strip off of all field names"
			." (i.e. -prefix TRI will name TRI-EFF-DATE as"
			." effDate)."],
   [ "ds=s",		undef,
			"DATASET",
			"data set class name",
			"If you project uses data sets, This sould be the"
			." name of the the class that has the anyDataSet()"
			." and key() static methods (noramlly DataSet)."]
  );

# our @Environment =
#   (
#   );

#
# Global Constants
#


use Doc::Self;
use App::Options;
use App::Debug;
use Cwd;
use DataFile::Record;
use Mdb::CodeGen;
# use IO;


sub main {

  # $App::Options::debug = 1;

  my $doc   = new Doc::Self( DOC => \%Doc,
			     OPTIONS => \@Options );
  my $opts  = new App::Options( DOCSELF => $doc,
				OPTIONS => \@Options );


  my $rec = new DataFile::Record;

  if( $opts->opt( "cb" ) ) {
    $rec->parse_copy_book( new IO::File( $opts->opt( "cb" ), "r" ),
			   $opts->opt( "prefix" ) );
  } elsif( $opts->opt( "rlr" ) ) {
    if( ! -f $opts->opt( "rlr" ) ) {
      die( $opts->opt("rlr"), " - not found." );
    }
    $rec->parse_record_layout_report( new IO::File( $opts->opt( "rlr" ),
						    "r" ),
				      $opts->opt( "prefix" ) );
  }

  my $gen = new Mdb::CodeGen( PROJECT	    => $opts->opt( "prj" ),
			      TABLE_NAME    => $opts->opt( "name" ),
			      DATASET_TYPE  => $opts->opt( "ds" ),
			      RECORD	    => $rec );

  $gen->gen_table_class();
  $gen->gen_mdb_query();
  $gen->gen_mdb_loader();

}
$Doc{ "EXTRA_SECTIONS" } = [""];

$Doc{ "See Also" } = [["perl","1"],
		      ["DataFile::Record","3"]];

$Doc{ Files } = [];

if( ! defined( $main::DontRun ) || $main::DontRun == 0 ) {
  main();
} else {
  1;
}


#
# Revision Log:
#
# $Log$
# Revision 1.1  2003/06/08 18:03:52  houghton
# Initial Version
#
#

# Set XEmacs mode
#
# Local Variables:
# mode:perl
# End:
