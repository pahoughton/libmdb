#
#
# Title:        Makefile
# Project:	
# Desc:
# 
#   Makefile for Perf
# 
# Notes:
#
#   This Makefile requres MakeConfig and GNU's make version 3.75
#
#   Try 'make help' for a list of standard targets.
# 
# Author:	Paul Houghton 719-527-7834 (paul.houghton@wcom.com)
# Created:	5/27/100 useing MakeConfigs 5.06.01
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
#   proto_dep	    if this is true, the proto header will be rebuild when
#		    one of it's dependancies changes (ie a .c source)
#   check_install   if this is true, install and install_all will NOT
#		    overwrite an installed version.
#

makefile_type   = lib
make_cfg_ver	= 5.06
make_target	= default
show_commands 	= # true
proto_dep	= true
check_install	= true

# Include standard MakeConfig configuration for GNU make required
include Make/make.cfg.$(make_cfg_ver)

# 
# =USE_CURRENT_MAKEFILE=
# When you re-gen a make file, every thing between 
# these tags will be kept in the new makefile
# if you want it re-gen'd, just delete the tags
#

NAME		= Perf

PRJ_TOPDIR	= ../../..
CFG_DIR			= $(PRJ_TOPDIR)/src/config


INSTALL_BASE_DIR	= $(TOOL_DIR)
INSTALL_LIB_DIR		= $(beta_libdir)
INSTALL_INC_DIR		= $(beta_incdir)

VER_MAJOR	= 1
VER_MINOR	= 00
VER_RELEASE	= 00.a.01
VERSION		= $(VER_MAJOR).$(VER_MINOR).$(VER_RELEASE)

LIBRARY_DEFAULT	= lib$(NAME).a
LIBRARY_DEBUG	= lib$(NAME)_g.a
LIBRARY_TEST	= lib$(NAME)_tg.a
LIBRARY_PROFILE	= lib$(NAME)_pg.a
LIBRARY_SHARED	= lib$(NAME)_s.$(shlib_ext)


# include any arch specific makefile component
-include $(PRJ_TOPDIR)/src/arch/$(arch)/Makefile.$(NAME)

include $(CFG_DIR)/00-Makefile.cfg

VPATH		= $(ARCH_VPATH):.

###### D E F I N E S  #####
# 
# Config defines avalable
#
#


SRC_DEFINES	= 
PRJ_DEFINES	= -DVERSION=\"$(VERSION)\" $(ARCH_DEFINES)

DEFAULT_DEFINES = -DPERF_DEFAULT=1
DEBUG_DEFINES	= -DPERF_DEBUG=1 -DMDB_DEBUG=1 -DRDB_TEST=1 -DSTLUTILS_DEBUG=1
TEST_DEFINES	= -DPERF_TEST=1 $(DEBUG_DEFINES)
PROFILE_DEFINES	= -DPERF_PROFILE=1
SHARED_DEFINES	= -DPERF_SHARED=1

######  I N C L U D E S #####
#
# Config includes available (need to use -I)
#
# $(alpha_incdir) 	$(beta_incdir) 	$(local_incdir)
#


SRC_INC		= $(ARCH_INC) -I. $(MDB_SRC_INC)
PRJ_INC		= $(PROJECT_INC_DIRS)


# Any other cpp flags to use on for all targets

CPP_FLAGS		= $(ARCH_CPP_FLAGS)

FLEX_FLAGS		=
BISON_FLAGS		=

###### C O M P I L E / C P P #####
#
# Config flags available
#
# $(cc_optimize_flags) $(cxx_optimize_flags)
# $(cc_debug_flags)    $(cxx_debug_flags)
# $(cc_profile_flags)  $(cxx_profile_fllags)
# $(cc_warn)           $(cxx_warn)
#


CC_FLAGS		= $(cc_warn) $(ARCH_CC_FLAGS) $(CC_OTHER)

DEFAULT_CC_FLAGS	= $(cc_optimize_flags)
DEBUG_CC_FLAGS		= $(cc_debug_flags)
TEST_CC_FLAGS		= $(cc_debug_flags)
PROFILE_CC_FLAGS	= $(cc_profile_flags)
SHARED_CC_FLAGS		= $(cc_shared_flags) $(cc_optimize_flags)

CXX_FLAGS		= $(cxx_warn) $(ARCH_CXX_FLAGS) $(CXX_OTHER)

DEFAULT_CXX_FLAGS	= $(cxx_optimize_flags)
DEBUG_CXX_FLAGS		= $(cxx_debug_flags)
TEST_CXX_FLAGS		= $(cxx_debug_flags)
PROFILE_CXX_FLAGS	= $(cxx_profile_flags)
SHARED_CXX_FLAGS	= $(cxx_shared_flags) $(cxx_optimize_flags)

# **** S H A R E D   D E P   L I B S *****

SHDEP_LIBS		= -lc

# **** I N S T A L L ****

INSTALL_LIB_PERM	= 444
INSTALL_INC_PERM	= 444

INSTALL_LIB_FLAGS	= -m $(INSTALL_LIB_PERM)
INSTALL_INC_FLAGS	= -m $(INSTALL_INC_PERM)

#
# =USE_CURRENT_MAKEFILE=
#

# **** S O U R C E   F I L E S ****

GENERATED_SOURCES	=			

FLEX_C_SOURCES	=			
BISON_C_SOURCES	=			
FLEX_CXX_C_SOURCES	=			
BISON_CXX_C_SOURCES	=			
C_SOURCES	=			
CXX_C_SOURCES	=			\
	LogPerfData.C                   \
	$(ARCH_CXX_C_SOURCES)           \

CXX_CC_SOURCES	=			
CXX_CPP_SOURCES	=			

USER_HH_INCS	=			\
	Perf.hh                         \
	$(ARCH_USER_HH_INCS)            \

USER_INCLUDES	=		\
	$(USER_H_INCS)		\
	$(USER_HH_INCS)		\
	$(USER_II_INCS)		\

CXX_HEADERS	=			\
	Perf.hh                         \
	$(ARCH_CXX_HEADERS)             \



###### T A R G E T S #####
#
# Targets defined in make.cfg.targets.
#
# default: 	all
# depend:	create .depend_default
# profile:	MAKE CCDEBUG=CC_PROFILE all
# proto:	create $(PROTO_H)
# clean:  	rm -f LIB_OBJECTS LIBRARY
# realclean:	rm -f LIB_OBJECTS LIBRARY .depend.lib PROTO_H TAGS
# tags:		create TAGS using etags
#


include Make/make.cfg.targets.$(makefile_type).$(make_cfg_ver)


default debug test profile shared:: $(generated_files)

default:: $(LIBRARY_$($(make_target)))

debug test profile shared:: 
	$(hide) $(MAKE) make_target=$@ $(LIBRARY_$($@))

all::	default debug test profile shared


install:: install_lib_check install_$(make_target)
	$(hide) echo + Installation Complete!

install_all:: install_lib_check

install_all:: install_default install_debug install_profile install_shared
	$(hide) echo + Installation Complete!

install_default:: default $(INSTALL_LIB_DIR)/$(LIBRARY_DEFAULT)
install_debug::   debug   $(INSTALL_LIB_DIR)/$(LIBRARY_DEBUG)
install_test::    test    $(INSTALL_LIB_DIR)/$(LIBRARY_TEST)
install_profile:: profile $(INSTALL_LIB_DIR)/$(LIBRARY_PROFILE)
install_shared::  shared  $(INSTALL_LIB_DIR)/$(LIBRARY_SHARED)
	$(hide) chmod +x $(INSTALL_LIB_DIR)/$(LIBRARY_SHARED)

FORCE: 

#
# include dependancies generated by 'make depend'
#
-include .depend_$(make_target)


#
# =MAKEFILE_LOG=
#
# $Log$
# Revision 2.2  2000/05/27 14:32:04  houghton
# Port: Sun CC 5.0.
#
# Revision 2.1  1997/09/21 19:22:41  houghton
# Changed version to 2
#
# Revision 1.2  1997/09/17 16:56:23  houghton
# Changed for new library rename to StlUtils
#
# Revision 1.1  1997/07/21 10:27:21  houghton
# Initial version.
#
#
# =MAKEFILE_LOG=
#

# Local Variables:
# mode:makefile
# End:

