#
# File:         Setup.Makefile
# Project:	Mdb %PP%
# Item:   	%PI% (%PF%)
# Desc:
#
#   This Makefile is used to setup all the dependances needed
#   by the project and generate the configuration file
#   src/config/Setup.cfg. It should not be directly used, instead, the
#   setup target sould be build from a top level setup makefile with
#   all the configuration variables set.
#
# Notes:
#
# Author:	Paul Houghton - <paul.houghton@wcom.com>
# Created:	03/05/01 05:03
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

include Make/make.cfg.$(make_cfg_ver)

#
# Targets
#

include Make/make.cfg.targets.setup.$(make_cfg_ver)

support:
	$(hide) $(TOOL_DIR)/bin/make				\
		-f $(PROJECT_DIR)/support/Support.Makefile	\
		setup						\
		PROJECT_DIR=$(PROJECT_DIR)			\
		CONFIG_VARS='$(CONFIG_VARS)'			\
		support_vars_file=$(support_vars_file)		\
		config_vars_file=$(config_vars_file)

setup: 	prep $(config_vars_file) support $(config_dir)/Setup.cfg



#
# Revision Log:
#
#
# %PL%
#
#

# Set XEmacs mode
#
# Local Variables:
# mode:makefile
# End:
