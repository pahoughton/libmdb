#!/usr/bin/ksh
#
# Title:        mkdirhier.sh
# Project:	DocTools
# Desc:
# 
#   $XConsortium: mkdirhier.sh,v 1.7 94/03/24 15:46:34 gildea Exp $
#   Courtesy of Paul Eggert
#   
# Notes:
# 
# Author:	Paul Houghton - (paul.houghton@wcom.com)
# Created:	06/11/00 17:05
#
# Revision History: (See end of file for Revision Log)
#
#   Last Mod By:    $Author$
#   Last Mod:	    $Date$
#   Version:	    $Revision$
#
#   $Id$
# 

newline='
'
IFS=$newline

case ${1--} in
  -*) echo >&2 "mkdirhier: usage: mkdirhier directory ..."; exit 1
esac

status=

for directory
do
  case $directory in
    '')
	echo >&2 "mkdirhier: empty directory name"
	status=1
	continue;;
    *"$newline"*)
        echo >&2 "mkdirhier: directory name contains a newline: \`\`$directory''"
	status=1
	continue;;
    ///*) prefix=/;; # See Posix 2.3 "path".
    //*) prefix=//;;
    /*) prefix=/;;
    -*) prefix=./;;
    *) prefix=
  esac

  IFS=/
  set x $directory
  case $2 in
    */*)        # IFS parsing is broken
	 IFS=' '
	 set x `echo $directory | tr / ' '`
	 ;;
  esac
  IFS=$newline
  shift

  for filename
  do
    path=$prefix$filename
    prefix=$path/
    shift

    test -d "$path" || {
      paths=$path
      for filename
      do
	if [ "$filename" != "." ]; then
	  path=$path/$filename
	  paths=$paths$newline$path
	fi
      done

      mkdir $paths || status=$?

      break
    }
  done
done

exit $status

#
# $Log$
# Revision 1.1  2000/07/24 09:59:54  houghton
# Initial Version.
#
# Revision 1.1  2000/06/11 22:59:37  houghton
# Initial Version.
#
#

# Local Variables:
# mode:ksh
# End:
