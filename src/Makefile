#
# File:         Makefile
# Project:	Mdb 
# Desc:
#
#   This Makefile builds the targets in all the source subdirectories.
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

PRJ_TOPDIR	= ..
config_dir	= $(PRJ_TOPDIR)/src/config

include $(config_dir)/00-Makefile.cfg
include Make/make.cfg.$(make_cfg_ver)

BUILD_TYPE_LIST		= all default debug profile test shared

DEPEND_TARGETS		= $(patsubst %,depend_%,$(BUILD_TYPE_LIST))
BUILD_LIB_TARGETS	= $(patsubst %,lib_%,$(BUILD_TYPE_LIST))
BUILD_TARGETS		= $(BUILD_TYPE_LIST)
INSTALL_LIB_TARGETS	= $(patsubst %,install_lib_%,$(BUILD_TYPE_LIST))
INSTALL_TARGETS		= $(patsubst %,install_%,$(BUILD_TYPE_LIST))

LIB_SUBDIRS	=	\
	Mdb

INSTALL_SUBDIRS	=	\

SUBDIRS		=		\
	$(LIB_SUBDIRS)		\
	$(BIN_SUBDIRS)		\
	$(INSTALL_SUBDIRS)

TARGETS		= $(DEPEND_TARGETS)		\
		  $(BUILD_TARGETS)		\
		  check				\
		  $(INSTALL_TARGETS)		\
		  $(INSTALL_LIB_TARGETS)	\
		  clean				\
		  realclean			\
		  help				\
		  help_config

HELP_TARGETS	= $(TARGETS)
PHONY_TARGETS	= $(TARGETS)


include Make/make.cfg.targets.common.$(make_cfg_ver)

$(DEPEND_TARGETS):
	$(call make_subdirs,$@,$(LIB_SUBDIRS) $(BIN_SUBDIRS),	\
		$(exports)					\
		$(depend_exports)				\
		$($(@)_exports))

$(BUILD_LIB_TARGETS):
	$(hide) $(call make_subdirs,$(subst lib_,,$@),$(LIB_SUBDIRS), \
		$(exports)					      \
		$($(@)_exports))

$(BUILD_TARGETS):
	$(call make_subdirs,$@,$(LIB_SUBDIRS) $(BIN_SUBDIRS),	\
		$(exports)					\
		$($(@)_exports))

check: test
	$(call make_subdirs,$@,$(PRJ_TOPDIR)/test,	\
		$(exports)				\
		$(test_exports)				\
		$($(@)_exports))

$(INSTALL_LIB_TARGETS):
	$(call make_subdirs,$(patsubst install_lib_%,install_%,$@),	\
		$(LIB_SUBDIRS),						\
		$(exports)						\
		$(install_lib_exports)					\
		$($(@)_exports))

$(INSTALL_TARGETS):
	$(call make_subdirs,install_shared,	\
		$(LIB_SUBDIRS),			\
		$(exports)			\
		$(install_lib_exports)		\
		$($(@)_exports))
	$(call make_subdirs,$@,		\
		$(BIN_SUBDIRS),		\
		$(exports)		\
		$(install_exports)	\
		$($(@)_exports))
	$(call make_subdirs,install,	\
		$(INSTALL_SUBDIRS),	\
		$(exports)		\
		$(install_exports)	\
		$($(@)_exports))

install: install_shared

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
