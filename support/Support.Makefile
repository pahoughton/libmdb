#
# File:         Support.Makefile
# Project:	Mdb 
# Desc:
#
#   This makefile retrieves, builds and installs all prerequisite
#   suppport tools and libraries needed by this project. If the
#   correct version of any of these tools is already installed, no
#   action will be take for those tools.
#
# Notes:
#
# Author:	Paul Houghton <Paul.Houghton@mci.com>
# Created:	07/26/01 using GenProject 6.01.01
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

include Make/make.cfg.$(make_cfg_ver)

include $(PROJECT_DIR)/support/.makeconfigs.configvars

SETUP_CONFIG_DIRS	= INC LIB JAVA BIN DOC DOC_HTML DOC_MAN

RUN_INC_DIR		= $(RUN_BASE_DIR)/include
RUN_LIB_DIR		= $(RUN_BASE_DIR)/lib
RUN_JAVA_DIR		= $(RUN_BASE_DIR)/share/java
RUN_BIN_DIR		= $(RUN_BASE_DIR)/bin
RUN_DOC_DIR		= $(RUN_BASE_DIR)/docs
RUN_DOC_HTML_DIR	= $(RUN_BASE_DIR)/html
RUN_DOC_MAN_DIR		= $(RUN_BASE_DIR)/man


NO_SETUP_CFG	= true
include  $(PROJECT_DIR)/src/config/00-Makefile.cfg

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

SETUP_CONFIG_INSTALL_DIRS	=				\
	$(foreach var,$(SETUP_CONFIG_DIRS)			\
		      $(filter-out $(CONFIG_DIRS),		\
				   $(RUN_DIR_VAR_LIST)),	\
		    INSTALL_$(var)_DIR='$(strip			\
			$(if $(INSTALL_$(var)_DIR),		\
			     $(INSTALL_$(var)_DIR),		\
			     $(subst $(RUN_BASE_DIR),		\
				     $(INSTALL_BASE_DIR),	\
				     $(RUN_$(var)_DIR))))')



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
		$(SETUP_CONFIG_INSTALL_DIRS)		\
		support_vars_file=$(support_vars_file)	\
		config_vars_file=$(config_vars_file)	\
		hide=$(hide)


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
