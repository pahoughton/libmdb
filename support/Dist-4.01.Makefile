#
# File:         Dist-4.01.Makefile
# Project:	Mdb ()
# Desc:
#
#   
#
# Notes:
#
# Author:	Paul Houghton - <paul.houghton@mci.com>
# Created:	08/05/01 05:50
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

INSTALL_VER		= PROD
INSTALL_VER_PROD	= $(PROJECT_VER_PROD)
INSTALL_VER_PREV	= $(PROJECT_VER_PREV)

INSTALL_VERSION		= $(INSTALL_VER_$(INSTALL_VER))

INSTALL_TARGETS		= $(patsubst %,install_%,$(BUILD_TYPE_LIST))

DIST_BINARY_TYPE_LIST	= shared debug

DIST_BASE_DIR			= $(shell cd $(PRJ_TOPDIR) && pwd)/install

override INSTALL_BASE_DIR	=			\
	$(DIST_BASE_DIR)$(strip				\
	    $(if $(RUN_BASE_DIR),/$(RUN_BASE_DIR)))

DIST_INSTALL_DIR_EXPORTS	=				\
	$(foreach var,$(RUN_DIR_VAR_LIST),			\
  INSTALL_$(var)_DIR=$(strip $(subst $(RUN_BASE_DIR),		\
				     $(INSTALL_BASE_DIR),	\
				     $(RUN_$(var)_DIR))))


SUBDIRS		=

TARGETS		= dist_binary			\
		  clean				\
		  realclean

HELP_TARGETS	= $(TARGETS)

PHONY_TARGETS	= $(HELP_TARGETS)


include Make/make.cfg.targets.install.$(make_cfg_ver)

dist_install:
	$(hide) $(MAKE) -f Install.Makefile				\
		$(patsubst %,install_%,$(DIST_BINARY_TYPE_LIST))	\
		$(exports)						\
		$(install_exports)					\
		$($(@)_exports)						\
		$(DIST_INSTALL_DIR_EXPORTS)


create_dist_cpio:
	$(hide) MakeInstallCpio						\
		--project=$(PROJECT_NAME)				\
		--prj-version=$(FULL_VERSION)				\
		--dist-base-dir=$(DIST_BASE_DIR)			\
		$(if $(INSTALL_FS_BASE_DIR),				\
		     --install-fs-base-dir=$(INSTALL_FS_BASE_DIR),)	\
		--install-base-dir=$(INSTALL_BASE_DIR)			\
		$(if $(FS_BASE_DIR),					\
		     --fs-base-dir=$(FS_BASE_DIR),)			\
		--run-base-dir=$(RUN_BASE_DIR)				\
		--install-script=$(INSTALL_SCRIPT)

dist_binary: dist_install create_dist_cpio


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
