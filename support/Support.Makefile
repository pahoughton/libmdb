#
# File:         Support.Makefile
# Project:	Mdb %PP%
# Item:   	%PI% (%PF%)
# Desc:
#
#   This makefile retrieves, builds and installs all prerequisite
#   suppport tools and libraries needed by this project. If the
#   correct version of any of these tools is already installed, no
#   action will be take for those tools.
#
# Notes:
#
# Author:	Paul Houghton <Paul.Houghton@wcom.com>
# Created:	07/26/01 using GenProject 6.01.01
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

include $(PROJECT_DIR)/support/.makeconfigs.configvars

# Do NOT pass any var=value pairs on my command line to
# the support tool builds.
MAKEOVERRIDES	=

SUPPORT_ITEMS	=		\
	$(MAKE_CONFIGS_ITEMS)	\
	DOCTOOLS		\
	STLUTILS		\


SUPPORT_MAKEFILES	=						\
	$(foreach var,							\
		$(filter-out $(MAKE_CONFIGS_ITEMS),$(SUPPORT_ITEMS)),	\
		$(PROJECT_DIR)/support/Support.$(var))


$(PROJECT_DIR)/support/Support.Items.Makefile:			\
		$(PROJECT_DIR)/support/Support.Makefile		\
		$(PROJECT_DIR)/support/Support.Items.Header	\
		$(SUPPORT_MAKEFILES)
	$(hide) cat $(PROJECT_DIR)/support/Support.Items.Header > $@;	 \
	        cat $(foreach makefile,$(SUPPORT_MAKEFILES),		 \
			$(makefile)) >> $@;				 \
	    echo							 \
		'include Make/make.cfg.targets.support.$$(make_cfg_ver)' \
		>> $@ ;
	@echo "+ Built $@"

setup: $(PROJECT_DIR)/support/Support.Items.Makefile
	$(hide) $(MAKE) -f $< $@			\
		SUPPORT_ITEMS='$(SUPPORT_ITEMS)'	\
		PROJECT_DIR=$(PROJECT_DIR)		\
		support_vars_file=$(support_vars_file)	\
		config_vars_file=$(config_vars_file)

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
