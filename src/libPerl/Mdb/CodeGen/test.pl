# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

#########################

# change 'tests => 1' to 'tests => last_test_to_print';

use Test;
BEGIN { plan tests => 8 };
use Mdb::CodeGen;
ok(1); # If we made it this far, we're ok.

#########################

# Insert your test code below, the Test module is use()ed here so read
# its man page ( perldoc Test ) for help writing this test script.

$Mdb::CodeGen::TemplateDir = ".";

use DataFile::Record;

my $rec = new DataFile::Record;
{
  use IO::File;
  $rec->parse_record_layout_report( new IO::File( "R5RVCRDC.txt", "r" ),
				    "TRI");
  ok(2);
}

my $gen = new Mdb::CodeGen( PROJECT	    => "Trs",
			    TABLE_NAME	    => "TrsCyclePool",
			    DATASET_TYPE    => "DataSet",
			    RECORD	    => $rec );
ok(3);
$gen->gen_table_class_hh();
ok(4);
$gen->gen_table_class_C();
ok(5);
$gen->gen_table_class_ii();
ok(6);
$gen->gen_mdb_query();
ok(7);
$gen->gen_mdb_loader();
ok(8);
