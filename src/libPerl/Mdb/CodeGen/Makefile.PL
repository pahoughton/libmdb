use ExtUtils::MakeMaker;
# See lib/ExtUtils/MakeMaker.pm for details of how to influence
# the contents of the Makefile that is written.
WriteMakefile(
    'NAME'		=> 'Mdb::CodeGen',
    'VERSION_FROM'	=> 'CodeGen.pm', # finds $VERSION
    'PREREQ_PM'		=> { Time::Readable => 0.01,
			     DataFile::Record => 1.01,
			     DataFile::Field => 1.01 },
    ($] >= 5.005 ?    ## Add these new keywords supported since 5.005
      (ABSTRACT_FROM => 'CodeGen.pm', # retrieve abstract from module
       AUTHOR     => 'Paul Houghton <paul.houghton@wcom.com>') : ()),
);
