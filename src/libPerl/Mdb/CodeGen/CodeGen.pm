package Mdb::CodeGen;

use 5.006;
use strict;
use warnings;

require Exporter;
use AutoLoader qw(AUTOLOAD);

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use Mdb::CodeGen ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	
);
our $VERSION = '0.01';


# Preloaded methods go here.
use Carp;
use Config;
use IO::File;
use DataFile::Record;
use Time::Readable;

our $TemplateDir = $Config{ sitelibexp }."/Mdb/CodeGen";

sub new {
  my ( $this, %params ) = (@_);
  my $class = ref($this) || $this;
  my $self  = {}; # $class->SUPER::new( %params );
  bless( $self, $class );

  $$self{ classname } = __PACKAGE__;

  print "$TemplateDir\n";

  my $now;
  {
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)
      = localtime(time);
    $now = sprintf( "%02d/%02d/%02d %02d:%02d",
		    $mon + 1,
		    $mday,
		    $year - 100,
		    $hour,
		    $min );
  }

  $$self{ author } = $ENV{ 'REPLYTO' };
  $$self{ created } = $now;

  {
    my $unknown = 0;
    my $p;
    foreach $p (keys(%params)) {
      if( $p eq "PROJECT" ) {
	$$self{ prj } = $params{ $p };
	$$self{ prj_upcase } = $$self{ prj };
	$$self{ prj_upcase } =~ tr/a-z/A-Z/;
      } elsif( $p eq "TABLE_NAME" ) {
	$$self{ table } = $params{ $p };
      } elsif( $p eq "DATASET_TYPE" ) {
	$$self{ dataset_type } = $params{ $p };
      } elsif( $p eq "RECORD" ) {
	$$self{ rec } = $params{ $p };
      } elsif( $p eq "TABLE_DIR" ) {
	$$self{ table_dir } = $params{ $p };
      } elsif( $p eq "QUERY_DIR" ) {
	$$self{ query_dir } = $params{ $p };
      } elsif( $p eq "LOADER_DIR" ) {
	$$self{ loader_dir } = $params{ $p };
      } else {
	print "Unknown arg '$p' passed to ", $$self{ classname }, "\n";
	$unknown = 1;
      }
    }
    if( $unknown ) {
      croak( "Aborted for unknown ", $$self{ classname }, " parameter." );
    }
  }

  if( ! defined( $$self{ table_dir } ) ) {
    $$self{ table_dir } = "lib".$$self{ prj }."Tables";
  }
  if( ! defined( $$self{ query_dir } ) ) {
    $$self{ query_dir } = $$self{ table } . "Query";
  }
  if( ! defined( $$self{ loader_dir } ) ) {
    $$self{ loader_dir } = "lib".$$self{ prj }."Loader";
  }

  return( $self );
}

sub gen_table_class () {
  my ($self) = (@_);

  if( ! $self->gen_table_class_hh()
      || ! $self->gen_table_class_ii()
      || ! $self->gen_table_class_C() ) {
    return( 0 );
  } else {
    return( 1 );
  }
}

sub gen_mdb_query () {
  my ($self) = (@_);

  if( ! $self->gen_mdb_qry_app_param_hh()
      || ! $self->gen_mdb_qry_app_param_C()
      || ! $self->gen_mdb_qry_main_C()
      || ! $self->gen_mdb_qry_makefile() ) {
    return( 0 );
  } else {
    return( 1 );
  }
}

sub gen_mdb_loader() {
  my ($self) = (@_);

  if( ! $self->gen_mdb_loader_hh()
      || ! $self->gen_mdb_loader_C()
      || ! $self->gen_mdb_loader_ii()) {
    return( 0 );
  } else {
    return( 1 );
  }
}

sub gen_prep_in_out {
  my ($self, $tmplSrcFn, $outDir, $outFn) = (@_);

  if( ! $$self{ table } ) {
    croak( "no table." );
  }

  if( defined( $outDir ) ) {
    if( ! -d $outDir ) {
      mkdir( $outDir ) || croak( "mkdir '$outDir' - $!" );
    }
  }

  my $out = new IO::File( "> $outDir/$outFn" );
  defined( $out ) || croak( "open '$outDir/$outFn' - $!" );

  print "+ Generating $outFn\n";

  my $inFn = $TemplateDir . "/" . $tmplSrcFn;

  my $in = new IO::File( "< $inFn" );
  defined( $in ) || croak( "open '$inFn - $!" );

  return( $in, $out );
}

sub replace_common_vars {
  my ($self, $tmplSrcFn) = (@_);

  s/%TABLE%/$$self{ table }/g;
  s/%PRJ%/$$self{ prj }/g;
  s/%PRJ_UPCASE%/$$self{ prj_upcase }/g;
  s/%PERL_PKG%/$$self{ classname }/g;
  s/%TMPL_SRC%/$tmplSrcFn/g;
  s/%AUTHOR%/$$self{ author }/g;
  s/%CREATED%/$$self{ created }/g;

}

sub rec_has_ebcdic {
  my ($self) = (@_);

  my $rec = $$self{ rec };
  my $hasEbcdic = 0;
  foreach my $f ($rec->field_list() ) {
    if( $rec->field_info( $f )->ebcdic()
	|| $rec->field_info( $f )->packed() ) {
      $hasEbcdic = 1;
      last;
    }
  }
  return( $hasEbcdic );
}


sub gen_table_class_hh () {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_Table_hh.tmpl";
  my $outFn	= $$self{ table } . "Table.hh";
  my $rec	= $$self{ rec };
  my $hasEbcdic = $self->rec_has_ebcdic();

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ table_dir },
					   $outFn );

  while( <$in> ) {
    if( /%FIELD_TYPES%/ ) {
      $out->print( $rec->gen_struct_fields( "    " ) );
    } elsif( /%FIND_KEY_FIELDS_DELC%/ ) {
      my $fld;
      my $findFields = "";

      $out->print( "  inline const_iterator find( " );

      foreach my $fld ($rec->field_list()) {

	my $argType = "const ".$rec->field_info( $fld )->ctype();
	if( $rec->field_info( $fld )->len() ) {
	  $argType .= " *";
	}
	
	$findFields .= sprintf( "%-14s %s,\n",
				$argType,
				$fld );
	$findFields .= "                              ";
      }
      $findFields =~ s/,\n                              $/\) const;/;
      $out->print( "$findFields\n" );
    } elsif( /%FIND_ASCII_DELC%/ ) {
      if( $hasEbcdic ) {
	my $fld;
	my $findFields = "";

	$out->print( "  inline const_iterator find_ascii( " );

	foreach my $fld ($rec->field_list()) {

	  my $argType = "const ".$rec->field_info( $fld )->ctype();
	  if( $rec->field_info( $fld )->len() ) {
	    $argType .= " *";
	  }
	
	  $findFields .= sprintf( "%-14s %s,\n",
				  $argType,
				  $fld );
	  $findFields .= "                                    ";
	}
	$findFields =~ s/,\n\s+$/\) const;/;
	$out->print( "$findFields\n" );
      }
    } elsif( /%GET_DATASET_ID_DECL%/ ) {
      if( $$self{ dataset_type } ) {
	$out->print(
	    "  DataSetId		getDataSetId( void ) const;\n" );
      }
    } elsif( /%COPY_TABLE_DECL%/ ) {
      if( $$self{ dataset_type } ) {
	$out->print( "
  static bool	    Copy( const FilePath & srcDir,
			  const FilePath & destDir,
			  DataSetId	   dataSetId = ".$$self{ dataset_type }."::anyDataSet() );
" );
      } else {
	$out->print( "
  static bool	    Copy( const FilePath & srcDir,
			  const FilePath & destDir );
");
      }
    } elsif( /%ERROR_NUM_ENUM%/ ) {
      if( $$self{ dataset_type } ) {
	$out->print( "  enum ErrorNum
  {
    E_OK,
    E_DATASET_KEY,
    E_DATASET,
    E_UNDEFINED
  };
");
      }
    } else {
      if( $hasEbcdic ) {
	s/%FIELD_UTILS%/EbcdicUtils/g;
	
      } else {
	s/%FIELD_UTILS%/FieldUtils/g;
      }
      if( $$self{ dataset_type } ) {
	s/%DATASET_HH%/\#include <$$self{ dataset_type }.hh>/g;
	s/%DATASET_ID_TYPE%/typedef $$self{ dataset_type }::DataSetId        DataSetId;/g;
	s/%CNSTR_DECL_DATASET_ID_ARG%/DataSetId       dataSetId = DataSet::anyDataSet(),/g;
	s/%CNSTR_DATASET_ID_ARG%/DataSetId       dataSetId,/g;
	s/%DATASET_ID_DECL%/DataSetId		    dataSet;/g;
	s/%ERROR_NUM_DECL%/ErrorNum		    errorNum;/g;
      } else {
	s/%DATASET_HH%//g;
	s/%DATASET_ID_TYPE%//g;
	s/%CNSTR_DATASET_ID_ARG%//g;
	s/%DATASET_ID_DECL%//g;
	s/%ERROR_NUM_DECL%//g;
      }
      $self->replace_common_vars( $tmplSrcFn );
      $out->print($_);
    }
  }

  $in->close();
  $out->close();

  return( 1 );
}

sub gen_table_class_C {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_Table_C.tmpl";
  my $outFn	= $$self{ table } . "Table.C";
  my $rec	= $$self{ rec };
  my $hasEbcdic = $self->rec_has_ebcdic();

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ table_dir },
					   $outFn );

  while( <$in> ) {

    if( /%CNSTR_DATASET_LOGIC%/ ) {
      if( $$self{ dataset_type } ) {
	$out->print( "
  if( create )
    {
      if( ! dataMgr.setNewKey( ".$$self{ dataset_type }."::key(), dataSetId ) )
	errorNum = E_DATASET_KEY;
      else
	errorNum = E_OK;

    }
  else
    {
      if( dataSetId != ".$$self{ dataset_type }."::anyDataSet()
	  && dataMgr.getKey( ".$$self{ dataset_type }."::key() ) != dataSetId )
	errorNum = E_DATASET;
      else
	errorNum = E_OK;
    }
");
      }
    } elsif( /%GET_DATASET_ID_IMPL%/ ) {
      if( $$self{ dataset_type } ) {
	my $class = $$self{ table }."Table";
	$out->print( "${class}::DataSetId
${class}::getDataSetId( void ) const
{
  return( dataMgr.getKey( ".$$self{ dataset_type }."::key() ) );
}
");
      }
    } elsif( /%ERROR_NUM_MSG_IMPL%/ ) {
      if( $$self{ dataset_type } ) {
	$out->print( "      switch( errorNum )
	{
	case E_DATASET_KEY :
	  errStr << \": data set key '\" << ".$$self{ dataset_type }."::key()
		 << \"' already in use.\";
	  break;

	case E_DATASET :
	  errStr << \": table from wrong data set '\"
		 << dataMgr.getKey( ".$$self{ dataset_type }."::key() )
		 << \"' need '\" << dataSet
	    ;
	  break;

	default:
	  break;
	}

" );
      }
    } elsif( /%TABLE_COPY_IMPL%/ ) {
       if( $$self{ dataset_type } ) {
	 $out->print( "bool
".$$self{ table }."Table::Copy( 
  const FilePath &	srcDir,
  const FilePath &	destDir,
  DataSetId		dataSetId
  )
{

  FileOp	    fileOp;

  // copy data file
  FilePath	dataSrcFn( srcDir, RecFileName );

  if( ! fileOp.copy( dataSrcFn, destDir, false ) ) {
    LLgError << fileOp.error() << endl;
    return( false );
  }

  // update data set id
  if( dataSetId != ".$$self{ dataset_type }."::anyDataSet() ) {
      ".$$self{ table }."Table    table( destDir,
			       ".$$self{ dataset_type }."::anyDataSet(),
			       (ios::open_mode)(ios::in | ios::out) );

      if( ! table.dataMgr.setKey( ".$$self{ dataset_type }."::key(), dataSetId ) )
	{
	  LLgError << \"setting dataSetId - \" << table.error() << endl;
	  return( false );
	}
    }

  return( true );
}" );
       } else {
	 $out->print( "bool
".$$self{ table }."Table::Copy( 
  const FilePath &	srcDir,
  const FilePath &	destDir
  )
{
  
  FileOp	    fileOp;

  // copy data file
  FilePath	dataSrcFn( srcDir, RecFileName );

  if( ! fileOp.copy( dataSrcFn, destDir, false ) )
    {
      LLgError << fileOp.error() << endl;
      return( false );
    }

  return( true );
}
");
       }
    } elsif( /%DUMP_FIELDS%/ ) {
      $out->print( $rec->gen_dump_fields() );
    } else {
      if( $$self{ dataset_type } ) {
	s/%DATASET_HH%/\#include <$$self{ dataset_type }.hh>/g;
	s/%DATASET_ID_TYPE%/typedef $$self{ dataset_type }::DataSetId        DataSetId;/g;
	s/%CNSTR_DATASET_ID_ARG%/DataSetId       dataSetId,/g;
	s/%DATASET_ID_DECL%/DataSetId		    dataSet;/g;
	s/%ERROR_NUM_DECL%/ErrorNum		    errorNum;/g;
	s/%CHECK_ERROR_NUM%/\&\& errorNum == E_OK /g;
      } else {
	s/%DATASET_HH%//g;
	s/%DATASET_ID_TYPE%//g;
	s/%CNSTR_DATASET_ID_ARG%//g;
	s/%DATASET_ID_DECL%//g;
	s/%ERROR_NUM_DECL%//g;
        s/%CHECK_ERROR_NUM%//g;
      }
      s/%TABLE%/$$self{ table }/g;
      s/%PRJ%/$$self{ prj }/g;
      s/%PRJ_UPCASE%/$$self{ prj_upcase }/g;
      s/%PERL_PKG%/$$self{ classname }/g;
      s/%TMPL_SRC%/$tmplSrcFn/g;
      s/%AUTHOR%/$$self{ author }/g;
      s/%CREATED%/$$self{ created }/g;
      $out->print($_);
    }
  }

  $in->close();
  $out->close();

  return( 1 );
}

sub gen_table_class_ii {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_Table_ii.tmpl";
  my $outFn	= $$self{ table } . "Table.ii";
  my $rec	= $$self{ rec };
  my $hasEbcdic = $self->rec_has_ebcdic();

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ table_dir },
					   $outFn );

  while( <$in> ) {

    if( /%KEY_COMPARE%/ ) {
      $out->print( "  int     diff;\n\n " );

      foreach my $fld ( $rec->field_list() ) {

	if( $rec->field_info( $fld )->len() ) {
	  $out->print(
"  if( (diff = memcmp( a.$fld,
		      b.$fld,
		      sizeof( a.$fld ) ))
      != 0 )
    return( diff < 0 );

" );
	} else {
	  $out->print(
"  if( (diff = compare( a.$fld, b.$fld )) != 0 )
    return( diff < 0 );

" );
	}
      }
    } elsif ( /%FIND_KEY_FIELDS_ARGS%/ ) {

      my $findArgs = "";
      foreach my $fld ( $rec->field_list() ) {
	if( $rec->field_info( $fld )->len() ) {
	  $findArgs .= sprintf( "  %-14s  %s,\n",
				"const "
				.$rec->field_info( $fld )->ctype()
				." *",
				$fld );
	} else {
	  $findArgs .= sprintf( "  %-14s  %s,\n",
				$rec->field_info( $fld )->ctype(),
				$fld );
	}
      }
      chop $findArgs;
      chop $findArgs;
      $out->print( "$findArgs\n" );
    } elsif ( /%FIND_KEY_FIELDS_METH%/ ) {

      foreach my $fld ( $rec->field_list() ) {
	if( $rec->field_info( $fld )->len() ) {
	  $out->print(
"  FixedFieldCopy( k.$fld, sizeof( k.$fld ),
                  $fld, NPOS, ' ' );

" );
	} else {
	  $out->print( "  k.$fld = $fld;\n\n" );
	}
      }
    } elsif( /%FIND_ASCII_IMPL%/ ) {
      if( $hasEbcdic ) {
	$out->print( "inline
".$$self{ table }."Table::const_iterator
".$$self{ table }."Table::find_ascii(
");
	my $findArgs = "";
	foreach my $fld ( $rec->field_list() ) {
	  if( $rec->field_info( $fld )->len() ) {
	    $findArgs .= sprintf( "  %-14s  %s,\n",
				  "const "
				  .$rec->field_info( $fld )->ctype()
				  ." *",
				  $fld );
	  } else {
	    $findArgs .= sprintf( "  %-14s  %s,\n",
				  $rec->field_info( $fld )->ctype(),
				  $fld );
	  }
	}
	chop $findArgs;
	chop $findArgs;
	$out->print( "$findArgs\n  ) const\n{\n  Rec k;\n" );
	
	foreach my $fld ( $rec->field_list() ) {
	  if( $rec->field_info( $fld )->len() ) {
	    $out->print(
"  FixedFieldCopy( k.$fld, sizeof( k.$fld ),
                  $fld, NPOS, ' ' );
  AsciiToEbcdic( k.$fld, sizeof( k.$fld ) );

" );
	  } else {
	    $out->print( "  k.$fld = $fld;\n\n" );
	  }
	}
	$out->print( "  return( find( k ) );\n}" );
      }
    } elsif( /%COMPARE_DATA%/ ) {
      foreach my $fld ( $rec->field_list() ) {

	if( $rec->field_info( $fld )->len() ) {
	  $out->print(
"  if( (diff = memcmp( a.$fld,
		      b.$fld,
		      sizeof( a.$fld ) ))
      != 0 )
    return( diff );

" );
	} else {
	  $out->print(
"  if( (diff = compare( a.$fld, b.$fld )) != 0 )
    return( diff );

" );
	}
      }
    } else {
      if( $$self{ dataset_type } ) {
	s/%DATASET_HH%/\#include <$$self{ dataset_type }.hh>/g;
	s/%DATASET_ID_TYPE%/typedef $$self{ dataset_type }::DataSetId        DataSetId;/g;
	s/%CNSTR_DATASET_ID_ARG%/DataSetId       dataSetId,/g;
	s/%DATASET_ID_DECL%/DataSetId		    dataSet;/g;
	s/%ERROR_NUM_DECL%/ErrorNum		    errorNum;/g;
	s/%CHECK_ERROR_NUM%/\&\& errorNum == E_OK /g;
      } else {
	s/%DATASET_HH%//g;
	s/%DATASET_ID_TYPE%//g;
	s/%CNSTR_DATASET_ID_ARG%//g;
	s/%DATASET_ID_DECL%//g;
	s/%ERROR_NUM_DECL%//g;
        s/%CHECK_ERROR_NUM%//g;
      }
      s/%TABLE%/$$self{ table }/g;
      s/%PRJ%/$$self{ prj }/g;
      s/%PRJ_UPCASE%/$$self{ prj_upcase }/g;
      s/%PERL_PKG%/$$self{ classname }/g;
      s/%TMPL_SRC%/$tmplSrcFn/g;
      s/%AUTHOR%/$$self{ author }/g;
      s/%CREATED%/$$self{ created }/g;
      $out->print($_);
    }
  }

  $in->close();
  $out->close();

  return( 1 );
}

sub gen_mdb_qry_app_param_hh {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_QUERY_AppParam_hh.tmpl";
  my $outFn	= "AppParam.hh";
  my $rec	= $$self{ rec };

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ query_dir },
					   $outFn );

  while( <$in> ) {
    if( /%KEY_VALUE_METHODS%/ ) {
      foreach my $fld ($rec->field_list()) {
	if( $rec->field_info( $fld )->ctype() =~ /char/ ) {
	  $out->printf(
		  "  const Str &   %s( void ) const { return( %sV ); };\n",
		  $fld,
		  $fld );
	} else {
	  $out->printf(
		  "  %-16s    %s( void ) const { return( %sV ); };\n",
		  $rec->field_info( $fld )->ctype(),
		  $fld,
		  $fld );
	}
      }
    } elsif( /%KEY_VALUE_VARS%/ ) {
      foreach my $fld ($rec->field_list()) {
	if( $rec->field_info( $fld )->ctype() =~ /char/ ) {
	  $out->printf( "  Str             %sV;\n", $fld );
	} else {
	  $out->printf( "  %-16s    %sV;\n",
			$rec->field_info( $fld )->ctype(),
			$fld );
	}
      }
    } else {
      $self->replace_common_vars( $tmplSrcFn );
      $out->print($_);
    }
  }

  $in->close();
  $out->close();
  return( 1 );
}

our %AppParamArgTypes = ( "char"	    => "argStr",
			  "unsigned char"   => "argStr",
			  "long"	    => "argLong" );


sub gen_mdb_qry_app_param_C {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_QUERY_AppParam_C.tmpl";
  my $outFn	= "AppParam.C";
  my $rec	= $$self{ rec };

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ query_dir },
					   $outFn );

  while( <$in> ) {
    if( /%KEY_VALUE_ARGS%/ ) {
      foreach my $fld ($rec->field_list()) {
	my $fldArg;
	$fldArg = $fld;
	$fldArg =~ s/([A-Z])/-$1/g;
	$fldArg =~ tr/A-Z/a-z/;
	$out->printf( "  status &= %s( %sV,
		    \"VAL\",
		    \"%s key value\",
		    0,
		    false,
		    \"%s\" );\n\n",
		      $AppParamArgTypes{ $rec->field_info( $fld )->ctype() },
		      $fld,
		      $fld,
		      $fldArg );
      }
    } elsif( /%TABLE_DIR_INIT%/ ) {
      if( $$self{ dataset_type } ) {
	$out->printf( "    tableDirV( %s::getDataSetDir( RUN_DATA_TABLES_DIR,
				       %s::anyDataSet() ) ),\n",
		      $$self{ dataset_type },
		      $$self{ dataset_type } );
      } else {
	$out->print( "    tableDirV( RUN_DATA_TABLES_DIR ),\n" );
      }
    } else {
      if( $$self{ dataset_type } ) {
	s/%DATASET_HH%/\#include <$$self{ dataset_type }.hh>/g;
      } else {
	s/%DATASET_HH%//g;
      }
      $self->replace_common_vars( $tmplSrcFn );
      $out->print($_);
    }
  }

  $in->close();
  $out->close();
  return( 1 );
}

sub gen_mdb_qry_main_C {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_QUERY_main_C.tmpl";
  my $outFn	= "main.C";
  my $rec	= $$self{ rec };

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ query_dir },
					   $outFn );

  while( <$in> ) {
   if( /%KEY_APP_ARGS%/ ) {
     my $findArgs;
      foreach my $fld ($rec->field_list() ) {
	$findArgs .= "	    (*App).$fld(),\n";
      }
      chop $findArgs;
      chop $findArgs;
      $out->print( "$findArgs\n" );
    } elsif ( /(\s+)%KEY_FIELDS_OUT%/ ) {
      my $indent = $1;
      foreach my $fld ($rec->field_list()) {
	$out->print( "$indent<< setw( 15 ) << \"$fld: '\" ",
		     "<< (*App).$fld() << \"'\\n\"\n" );
      }
    } else {
      if( $$self{ dataset_type } ) {
	s/%DATASET_HH%/\#include <$$self{ dataset_type }.hh>/g;
      } else {
	s/%DATASET_HH%//g;
      }
      $self->replace_common_vars( $tmplSrcFn );
      $out->print($_);
    }
  }

  $in->close();
  $out->close();
  return( 1 );
}

sub gen_mdb_qry_makefile {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_QUERY_Makefile.tmpl";
  my $outFn	= "Makefile";
  my $rec	= $$self{ rec };

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ query_dir },
					   $outFn );

  while( <$in> ) {
    s/%LIB_TABLES_DIR%/$$self{ table_dir }/g;
    $self->replace_common_vars( $tmplSrcFn );
    $out->print($_);
  }

  $in->close();
  $out->close();
  return( 1 );
}

sub gen_mdb_loader_hh {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_TABLE_Loader_hh.tmpl";
  my $outFn	= $$self{ table }."Loader.hh";
  my $rec	= $$self{ rec };
  my $hasEbcdic = $self->rec_has_ebcdic();

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ loader_dir },
					   $outFn );

  while( <$in> ) {
    if( /%FIELD_TYPES%/ ) {
      $out->print( $rec->gen_struct_fields( "    " ) );
    } else {
      if( $$self{ dataset_type } ) {
	s/%DATASET_ID_TYPE%/typedef $$self{ dataset_type }::DataSetId        DataSetId;/g;
	s/%CNSTR_DATASET_ID_ARG%/DataSetId       dataSetId,/g;
      } else {
	s/%DATASET_ID_TYPE%//g;
	s/%CNSTR_DATASET_ID_ARG%//g;
      }
      $self->replace_common_vars( $tmplSrcFn );
      $out->print($_);
    }
  }

  $in->close();
  $out->close();
  return( 1 );
}

sub gen_mdb_loader_C {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_TABLE_Loader_C.tmpl";
  my $outFn	= $$self{ table }."Loader.C";
  my $rec	= $$self{ rec };
  my $hasEbcdic = $self->rec_has_ebcdic();

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ loader_dir },
					   $outFn );

  while( <$in> ) {
    if( /%COPY_FIELDS%/ ) {
      foreach my $fld ( $rec->field_list() ) {
	my $spc = ($hasEbcdic ? "E_SPACE" : " " );
	if( $rec->field_info( $fld )->len() ) {
	  $out->print(
"      FixedFieldCopy( rec.$fld, sizeof( rec.$fld ),
                      (*them).$fld, sizeof((*them).$fld, $spc ) );

" );
	} else {
	  $out->print( "      rec.$fld = (*them).$fld;\n\n" );
	}
      }
    } elsif( /%DUMP_FIELDS%/ ) {
      $out->print( $rec->gen_dump_fields() );
    } else {
      if( $hasEbcdic ) {
	s/%FIELD_UTILS%/EbcdicUtils/g;
	
      } else {
	s/%FIELD_UTILS%/FieldUtils/g;
      }
      if( $$self{ dataset_type } ) {
	s/%DATASET_ID_TYPE%/typedef $$self{ dataset_type }::DataSetId        DataSetId;/g;
	s/%CNSTR_DATASET_ID_ARG%/DataSetId       dsId,/g;
      } else {
	s/%DATASET_ID_TYPE%//g;
	s/%CNSTR_DATASET_ID_ARG%//g;
      }
      $self->replace_common_vars( $tmplSrcFn );
      $out->print($_);
    }
  }

  $in->close();
  $out->close();
  return( 1 );
}

sub gen_mdb_loader_ii {
  my ($self) = (@_);

  my $tmplSrcFn = "MDB_TABLE_Loader_ii.tmpl";
  my $outFn	= $$self{ table }."Loader.ii";
  my $rec	= $$self{ rec };
  my $hasEbcdic = $self->rec_has_ebcdic();

  my ($in, $out) = $self->gen_prep_in_out( $tmplSrcFn,
					   $$self{ loader_dir },
					   $outFn );

  while( <$in> ) {
    $self->replace_common_vars( $tmplSrcFn );
    $out->print($_);
  }

  $in->close();
  $out->close();
  return( 1 );
}


# Autoload methods go after =cut, and are processed by the autosplit program.

1;
__END__
# Below is stub documentation for your module. You better edit it!

=head1 NAME

Mdb::CodeGen - Generate code for Mdb Table

=head1 SYNOPSIS

  use Mdb::CodeGen;


=head1 DESCRIPTION


=head2 EXPORT

None by default.


=head1 AUTHOR

Paul Houghton <lt>paul.houghton@wcom.com<gt>


=head1 SEE ALSO

L<perl>.

=cut

