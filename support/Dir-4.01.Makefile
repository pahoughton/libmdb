#
# File:         Dir-4.01.Makefile
# Project:	Mdb ()
# Desc:
#
#   
#
# Notes:
#
# Author:	Paul Houghton - <paul.houghton@mci.com>
# Created:	08/05/01 05:48
#
# Revision History: (See end of file for Revision Log)
#
#   $Author$ 
#   $Date$ 
#   $Name$ 
#   $Revision$ 
#   $State$ 
#
#   $Id$ 
#

#
# The following variables control the behavior of MakeConfigs.
#   make_cfg_ver    is the version of MakeConfigs to use
#   make_target	    is the default target to build
#   show_commands   if this is true, the commands  will be output
#   check_install   if this is true, install and install_all will NOT
#		    overwrite an installed version.
#   force	    if this is true, targets will be rebuilt
#		    whether they need to be or not.

make_target	= help
show_commands 	= # true
check_install	= true
force		= # true

PRJ_TOPDIR	= ..
config_dir  	= $(PRJ_TOPDIR)/src/config

include $(config_dir)/00-Makefile.cfg
include Make/make.cfg.$(make_cfg_ver)

include Make/make.cfg.targets.dir.$(make_cfg_ver)

install_dirs:  $(INSTALL_BASE_DIR) $(INSTALL_FS_BASE_DIR)
	$(hide) $(call install_dir_command,$(RUN_DIR_VAR_LIST))
	@echo "+ Install dirs created."




#
# Revision Log:
#
# %PL%
#

# Set XEmacs mode
#
# Local Variables:
# mode:makefile
# End:
