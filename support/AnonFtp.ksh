#!/usr/bin/ksh
#
# Title:        AnonFtp.ksh
# Project:	Mdb %PP%
# Item:   	%PI% (%PF%)
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
#   Last Mod By:    %PO%
#   Last Mod:	    %PRT%
#   Version:	    %PIV%
#   Status:	    %PS%
#
#   %PID%
# 

# set -x

cmdfile=/tmp/AnonFtp.cmd.$$

host=$1
remote_file=$2
local_dir=$3
unknown=$4

if [ -z "$host"		\
 -o -z "$remote_file"	\
 -o -z "$local_dir"	\
 -o ! -d "$local_dir"	\
 -o -n "$unknown" ] ; then
  echo "usage: $0 host remote_file local_dir"
  exit 1
fi

remote_dir=`dirname $remote_file`
remote_fn=`basename $remote_file`

if [ -n "$USER" ] ; then
  ftp_user=$USER
else
  if [ -n "$LOGIN" ] ; then
    ftp_user=$LOGIN
  else
    if [ -n "$LOGNAME" ] ; then
      ftp_user=$LOGNAME
    else
      ftp_user=`who am i | cut -d ' ' -f 1`
    fi
  fi
fi


cat << EOF > $cmdfile
open $host
user anonymous $ftp_user@wcom.com
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
# 
#
# %PL%
#
# 
# $Log$
# Revision 4.1  2001/07/27 00:57:44  houghton
# Change Major Version to 4
#
# Revision 1.2  2001/07/27 00:47:31  houghton
# *** empty log message ***
#
# Revision 6.2  2001/06/21 11:24:42  houghton
# Bug-fix: was trying to use /bin/ksh - changed to /usr/bin/ksh.
#
# Revision 6.1  2001/06/20 20:45:47  houghton
# Changed Major version to 6.
#
# Revision 1.1  2001/06/20 20:34:54  houghton
# Initial Version.
#
# Revision 5.5  1999/12/28 17:47:01  houghton
# Comment out debug 'set -x'.
#
# Revision 5.4  1999/12/28 17:45:24  houghton
# Bug-Fix: get an name for anon user.
#
# Revision 5.3  1999/12/28 17:42:53  houghton
# Turn on debug output for testing.
#
# Revision 5.2  1999/12/28 17:40:15  houghton
# Bug-Fix: fixed user id for anon ftp.
#
# Revision 5.1  1999/10/29 21:48:40  houghton
# Initial Version.
#
#

# Local Variables:
# mode:ksh
# End:
