#
# File:         Install.Makefile
# Project:	Mdb (%PP%)
# Item:   	%PI% (%PF%)
# Desc:
#
#   
#
# Notes:
#
# Author:	Paul Houghton - <paul.houghton@wcom.com>
# Created:	08/05/01 05:52
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

#
# The following variables control the behavior of MakeConfigs.
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

BUILD_TYPE_LIST		= all default debug profile test shared

INSTALL_VER		= PROD
INSTALL_VER_PROD	= $(PROJECT_VER_PROD)
INSTALL_VER_PREV	= $(PROJECT_VER_PREV)

INSTALL_VERSION		= $(INSTALL_VER_$(INSTALL_VER))

INSTALL_TARGETS		= $(patsubst %,install_%,$(BUILD_TYPE_LIST))

INSTALL_SUPPORT_LIB_TARGETS	=				\
	$(patsubst %,install_support_lib_%,$(BUILD_TYPE_LIST))

INSTALL_SUPPORT_TARGETS	=					\
	$(patsubst %,install_support_%,$(BUILD_TYPE_LIST))

TARGETS		= help					\
		  install_dirs				\
		  $(INSTALL_SUPPORT_LIB_TARGETS)	\
		  $(INSTALL_SUPPORT_TARGETS)		\
		  $(INSTALL_TARGETS)			\
		  dist_binary				\
		  clean					\
		  realclean

HELP_TARGETS	= $(TARGETS)

PHONY_TARGETS	= $(HELP_TARGETS)


include Make/make.cfg.targets.install.$(make_cfg_ver)

install_dirs:
	$(hide) $(MAKE) -f Dir-$(INSTALL_VERSION).Makefile install_dirs	\
		$(exports)						\
		$(install_dir_exports)

$(INSTALL_SUPPORT_LIB_TARGETS):
	$(call make_subdirs,$@,						\
	    $(foreach var,$(SUPPORT_ITEMS),				\
		$(if $($(var)_LIBS)$($(var)_DEP_LIBS),			\
		    $(if $($(var)_BUILD_DIR),				\
			$($(var)_EXTRACT_DIR)/$($(var)_BUILD_DIR)))),	\
	    $(exports)							\
	    $(install_lib_exports)					\
	    $($(@)_exports)						\
	    INSTALL_INC_DIR=$(RUN_NO_DIR))
	$(call make_subdirs,$(subst _support,,$@),$(PRJ_TOPDIR),	\
		$(exports)						\
		$(install_lib_exports)					\
		$($(@)_exports)						\
		INSTALL_INC_DIR=$(RUN_NO_DIR))

$(INSTALL_SUPPORT_TARGETS):
	$(call make_subdirs,$(subst _support,,$@),			\
	    $(foreach var,$(SUPPORT_INSTALL_ITEMS),			\
		$(if $($(var)_BUILD_DIR),				\
		    $($(var)_EXTRACT_DIR)/$($(var)_BUILD_DIR))),	\
	    $(exports)							\
	    $(install_exports)						\
	    $($(@)_exports))


$(INSTALL_TARGETS): 	install_dirs			\
			install_support_lib_shared	\
			install_support_shared		\
			install_support_debug
	$(hide) $(MAKE) -C $(PRJ_TOPDIR)/src $@	\
		$(exports)			\
		$(install_exports)		\
		$($(@)_exports)


dist_binary:
	$(MAKE) -f Dist-$(INSTALL_VERSION).Makefile $@	\
		$(exports)				\
		$(install_exports)			\
		$($(@)_exports)


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
