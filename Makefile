#
# Title:        Makefile
# Project:	Mdb 
# Desc:
# 
#   This is the Top Level Makefile for Mdb. Before you build
#   any of the targets in this makefile, you must setup the project
#   using the Setup.Makefile in this directory. Please refer to it for
#   more details.
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

#
# The following variables control the behavior of MakeConfigs.
#   make_target	    is the default target to build
#   show_commands   if this is true, the commands  will be output
#   check_install   if this is true, install and install_all will NOT
#		    overwrite an installed version.
#   force	    if this is true, targets will be rebuilt
#		    whether they need to be or not.

make_target	= default
show_commands 	= # true
check_install	= true
force		= # true

PRJ_TOPDIR	= .
config_dir  	= $(PRJ_TOPDIR)/src/config

# #### If you got an error here, see Setup.Makefile ####
include $(config_dir)/00-Makefile.cfg
include Make/make.cfg.$(make_cfg_ver)

DIST_BINARY_TYPE_LIST	= shared debug

BUILD_TYPE_LIST		= all default debug profile test shared

DEPEND_TARGETS		= $(patsubst %,depend_%,$(BUILD_TYPE_LIST))
BUILD_LIB_TARGETS	= $(patsubst %,lib_%,$(BUILD_TYPE_LIST))
BUILD_TARGETS		= $(BUILD_TYPE_LIST)
INSTALL_LIB_TARGETS	= $(patsubst %,install_lib_%,$(BUILD_TYPE_LIST))
INSTALL_TARGETS		= $(patsubst %,install_%,$(BUILD_TYPE_LIST))

SUBDIRS		= src doc test

TARGETS		=		\
	$(DEPEND_TARGETS)	\
	$(BUILD_TARGETS)	\
	$(INSTALL_TARGETS)	\
	$(INSTALL_LIB_TARGETS)	\
	html			\
	man			\
	dist			\
	dist_binary		\
	dist_html		\
	clean			\
	realclean		\
	help			\
	help_config

HELP_TARGETS	= $(TARGETS)

PHONY_TARGETS	= $(HELP_TARGETS)

include Make/make.cfg.targets.common.$(make_cfg_ver)

$(DEPEND_TARGETS):
	$(call make_subdirs,$@,src,			\
		$(exports)				\
		$(depend_exports)			\
		$($(@)_exports)				\
		BUILD_TYPE=$(subst depend_,,$@))

$(BUILD_TARGETS):
	$(call make_subdirs,$@,src,	\
		$(exports)		\
		$($(@)_exports)		\
		BUILD_TYPE=$@)

check:
	$(call make_subdirs,$@,src,	\
		$(exports)		\
		$($(@)_exports))

$(BUILD_LIB_TARGETS):
	$(hide) $(call make_subdirs,$@,src,	\
		$(exports)			\
		$($(@)_exports))

$(INSTALL_LIB_TARGETS):
	$(call make_subdirs,$@,src,			\
		$(exports)				\
		$(install_lib_exports)			\
		$($(@)_exports)				\
		BUILD_TYPE=$(subst install_lib_,,$@))

$(INSTALL_TARGETS):
	$(hide) $(MAKE) -C support -f Install.Makefile $@	\
		INSTALL_BASE_DIR=$(RUN_BASE_DIR)		\
		$(exports)					\
		$(install_exports)				\
		$($(@)_exports)					\
		BUILD_TYPE=$(subst install_,,$@)

install: install_shared

install_support:
	$(hide) $(MAKE) -C support -f Install.Makefile $@	\
		$(install_support_exports)			\
		$(exports)					\
		$($(@)_exports)

install_support_lib_all install_support_lib_shared:
	$(hide) $(MAKE) -C support -f Install.Makefile $@	\
		$(install_support_lib_exports)			\
		$(exports)					\
		$($(@)_exports)

html man:
	$(call make_subdirs,$@,doc,	\
		$(exports)		\
		$(doc_exports)		\
		$($(@)_exports))

install_html install_man:
	$(call make_subdirs,$@,doc,	\
		$(exports)		\
		$(doc_exports)		\
		$(install_doc_exports)	\
		$($(@)_exports))

dist:
	$(call make_dist_from_dim,infr_objs,mcmain,$(PROJECT_DIR))

dist_binary:
	$(hide) $(MAKE) -C support -f Install.Makefile $@	\
		$(exports)					\
		$(install_exports)				\
		$($(@)_exports)

dist_html:
	$(call make_subdirs,$@,docs,$(exports) $($(@)_exports))

realclean::
	rm -rf install

# Detail Documentation
#
# Control Variables
#
#   show_commands   if this is true, the commands executed during the
#		    build will be output. Normally these commands are
#		    hidden and the only thing output is short messages
#		    indicating the items being built
#
#   check_install   if this is true, install and install_all will NOT
#		    overwrite an installed version.
#
#   force	    If this is not empty, force the rebuild of all
#		    targets even if none of the dependencies are out
#		    of date.
#
# Help variables
#
#   HELP_TARGETS	Add any targets you create that should be
#			listed when a user performs a `make help'.
#
# Target Variables
#
#   TARGETS		All the top level targets for this Makefile.
#
#   PHONY_TARGETS	All list of the phony targets (i.e. not real
#			files) that you have added to this makefile
#			which should be appended to the .PHONY:
#			target. For more information, see make(info).
#

#
# Revision Log:
#
#
# %PL%
#
#

# Local Variables:
# mode:makefile
# End:
