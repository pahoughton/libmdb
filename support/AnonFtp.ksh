#!/usr/bin/ksh
#
# Title:        AnonFtp.ksh
# Project:	MakeConfigs
# Desc:
# 
#   This is a simple shell script to fetch the tools needed
#   to build and install NcmLookups. It can be used to retreive
#   any binary file from any host via anonymous ftp.
#
#   AnonFtp.ksh host remotefile localdir
# 
# Notes:
# 
# Author:	Paul Houghton - (paul.houghton@wcom.com)
# Created:	10/28/99 15:29
#
# Revision History: (See end of file for Revision Log)
#
#   Last Mod By:    $Author$
#   Last Mod:	    $Date$
#   Version:	    $Revision$
#
#   $Id$
# 

cmdfile=/tmp/AnonFtp.cmd.$$

host=$1
remote_file=$2
local_dir=$3

if [ -z "$host" \
 -o -z "$remote_file" \
 -o -z "$local_dir" \
 -o ! -d "$local_dir" ] ; then
  echo "usage: $0 host remote_file local_dir"
  exit 1
fi

remote_dir=`dirname $remote_file`
remote_fn=`basename $remote_file`

cat << EOF > $cmdfile
open $host
user anonymous $USER@wcom.com
binary
cd $remote_dir
get $remote_fn
close
quit
EOF

cd $local_dir
if ftp -vn < $cmdfile ; then
  exit_code=0
else
  echo "ftp FAILED."
  cat $cmdfile
  exit_code=1
fi

if [ -f $local_dir/$remote_fn -a $exit_code = 0 ] ; then
  echo "retrieved $local_dir/$remote_fn"
else
  echo "retrieval FAILED."
  cat $cmdfile
  exit_code=1
fi  

rm $cmdfile

exit $exit_code

#
# $Log$
# Revision 1.1  2000/07/24 09:59:54  houghton
# Initial Version.
#
# Revision 1.1  1999/10/29 21:43:53  houghton
# Initial Version.
#
#

# Local Variables:
# mode:ksh
# End:
