#
# Title:        Makefile
# Project:	Mdb
# Desc:
# 
#   
# 
# Notes:
# 
# Author:	Paul Houghton 719-527-7834 - (paul.houghton@wcom.com)
# Created:	04/11/03 08:58
#
# Revision History: (See end of file for Revision Log)
#
#   Last Mod By:    $Author$
#   Last Mod:	    $Date$
#   Version:	    $Revision$
#
#   $Id$
# 


#
# The following variables control the behavior of MakeConfigs.
#   makefile_type   is the type of object being build (do NOT change)
#   make_cfg_ver    is the version of MakeConfigs to use
#   make_target	    is the default target to build
#   show_commands   if this is true, the commands  will be output
#   check_install   if this is true, install and install_all will NOT
#		    overwrite an installed version.
#

makefile_type   = doc
make_target	= html
make_cfg_ver	= 6.01
show_commands 	= # true
check_install	= true
force		= # true

PRJ_TOPDIR  	= ../..
config_dir  	= $(PRJ_TOPDIR)/src/config

PROJECT		= Mdb

include $(config_dir)/00-Makefile.cfg
include Make/make.cfg.$(make_cfg_ver)



include Make/make.cfg.targets.common.$(make_cfg_ver)

$(INSTALL_BIN_DIR)/GenMdbTableSources.pl: GenMdbTableSources.pl
	$(install_cmd) -m 555 $< $@

install: $(INSTALL_BIN_DIR)/GenMdbTableSources.pl

#
# $Log$
# Revision 1.1  2003/06/08 18:03:52  houghton
# Initial Version
#
#

# Local Variables:
# mode:makefile
# End:
