#
# File:         Makefile
# Project:	Mdb %PP%
# Item:   	%PI% (%PF%)
# Desc:
#
#   This Makefile builds the targets in all the source subdirectories.
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

show_commands 	= # true
check_install	= true

PRJ_TOPDIR	= ..
config_dir	= $(PRJ_TOPDIR)/src/config

include $(config_dir)/00-Makefile.cfg
include Make/make.cfg.$(make_cfg_ver)

LIB_SUBDIRS	=	\
	Mdb

SUBDIRS		=	\
	$(LIB_SUBDIRS)

TARGETS		= $(standard_targets)	\
	debug				\
	test				\
	depend				\
	depend_all			\
	depend_default			\
	depend_debug depend_test

HELP_TARGETS	= $(TARGETS)
PHONY_TARGETS	= $(TARGETS)

include Make/make.cfg.targets.common.$(make_cfg_ver)

$(TARGETS)::
	$(call make_subdirs,$@,$(SUBDIRS),$($(@)_exports) $(exports))

install_lib_all:
	$(call make_subdirs,install_all,$(LIB_SUBDIRS),$($(@)_exports) $(exports))

install install_all::

check:: test
	$(call make_subdirs,$@,$(PRJ_TOPDIR)/test,$($(@)_exports) $(exports))


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
