#
# Title:        make.cfg.template.prog
# Project:	Test
# Desc:
# 
#   Makefile for Test.
# 
# Notes:
# 
#   This Makefile requres MakeConfig and GNU's make version 3.75
#
#   Try 'make help' for a list of standard targets.
# 
# Author:	Paul Houghton (paul.houghton@wcom.com)
# Created:	10/20/98 using MakeConfigs 5.05.00.b.05
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

makefile_type   = prog
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
# these tags will be kept int the new makefile
# if you want it re-gen'd, just delete the tags
#

NAME		= Test

PRJ_TOPDIR		= ../../..
CFG_DIR			= $(PRJ_TOPDIR)/src/config

PRJ_BINDIR		= $(PRJ_TOPDIR)/bin

INSTALL_BASE_DIR	= $(PRJ_TOPDIR)
INSTALL_BIN_DIR		= $(INSTALL_BASE_DIR)/bin

VER_MAJOR	= 1
VER_MINOR	= 00
VER_RELEASE	= 00.a.01
VERSION		= $(VER_MAJOR).$(VER_MINOR).$(VER_RELEASE)

PROGRAM_DEFAULT	= $(NAME)
PROGRAM_DEBUG	= $(NAME)_g
PROGRAM_TEST	= $(NAME)_tg
PROGRAM_PROFILE	= $(NAME)_pg
PROGRAM_STATIC	= $(NAME)_static
PROGRAM_SHARED	= $(NAME)_s


# include any arch specific makefile component
-include $(PRJ_TOPDIR)/src/arch/$(arch)/Makefile.$(NAME)

include $(CFG_DIR)/00-Makefile.cfg

VPATH		= $(ARCH_VPATH):.

###### D E F I N E S  #####
# 
# Config defines avalable
#
#


SRC_DEFINES	= -DVERSION=\"$(VERSION)\" $(ARCH_DEFINES)
PRJ_DEFINES	=

DEFAULT_DEFINES = -DTEST_DEFAULT=1
DEBUG_DEFINES	= -DTEST_DEBUG=1 -DMDB_DEBUG=1 -DRDB_TEST=1 -DSTLUTILS_DEBUG=1
TEST_DEFINES	= -DTEST_TEST=1 $(DEBUG_DEFINES)
PROFILE_DEFINES	= -DTEST_PROFILE=1
STATIC_DEFINES	= -DTEST_STATIC=1
SHARED_DEFINES	= -DTEST_SHARED=1

######  I N C L U D E S #####
#
# Config includes available (need to use -I)
#
# $(alpha_incdir) 	$(beta_incdir) 	$(local_incdir)
#


SRC_INC		= $(ARCH_INC) -I. $($($(make_target))_PRJ_INC)
PRJ_INC		= $(PROJECT_INC_DIRS)

DEFAULT_PRJ_INC	= 
DEBUG_PRJ_INC	= $(MDB_SRC_INC)
TEST_PRJ_INC	= $(MDB_SRC_INC)
PROFILE_PRJ_INC = $(MDB_SRC_INC)
STATIC_PRJ_INC	= 

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


CC_FLAGS		= $(ARCH_CC_FLAGS) $(CC_OTHER)

DEFAULT_CC_FLAGS	= $(cc_optimize_flags)
DEBUG_CC_FLAGS		= $(cc_debug_flags)
TEST_CC_FLAGS		= $(cc_debug_flags)
PROFILE_CC_FLAGS	= $(cc_profile_flags)
STATIC_CC_FLAGS		= $(cc_optimize_flags)
SHARED_CC_FLAGS		= $(cc_shared_flags) $(cc_optimize_flags)

CXX_FLAGS		= $(cxx_warn) $(ARCH_CXX_FLAGS) $(CXX_OTHER)

DEFAULT_CXX_FLAGS	= $(cxx_optimize_flags)
DEBUG_CXX_FLAGS		= $(cxx_debug_flags)
TEST_CXX_FLAGS		= $(cxx_debug_flags)
PROFILE_CXX_FLAGS	= $(cxx_profile_flags)
STATIC_CXX_FLAGS	= $(cxx_optimize_flags)
SHARED_CXX_FLAGS	= $(cxx_shared_flags) $(cxx_optimize_flags)

# ##### I N S T A L L #####

INSTALL_BIN_PERM	= 555
INSTALL_BIN_FLAGS	= -m $(INSTALL_BIN_PERM)

# ##### L I N K  /  L I B R A R I E S #####
#
# Config flags available
#
# $(ld_optimize_flags)
# $(ld_debug_flags)
# $(ld_profile_flags)
# $(ld_static_flags)
#
# Config library directories available (need to use -L)
#
# $(alpha_libdir)	$(beta_libdir)	$(local_libdir)
#


LD_FLAGS		= $(ARCH_LD_FLAGS) $(LD_OTHER)

DEFAULT_LD_FLAGS	= $(ld_optimize_flags)
DEBUG_LD_FLAGS		= $(ld_debug_flags)
TEST_LD_FLAGS		= $(ld_debug_flags)
PROFILE_LD_FLAGS	= $(ld_profile_flags)
STATIC_LD_FLAGS		= $(ld_static_flags) $(ld_optimize_flags)
SHARED_LD_FLAGS		= $(ld_shared_flags) $(ld_optimize_flags)

DEFAULT_LD_LIBDIRS	= $(PROJECT_LIB_DIRS)
DEBUG_LD_LIBDIRS	= -L$(PRJ_TOPDIR)/src/Mdb $(PROJECT_LIB_DIRS)
TEST_LD_LIBDIRS		= $(DEBUG_LD_LIBDIRS)
PROFILE_LD_LIBDIRS	= $(DEBUG_LD_LIBDIRS)
STATIC_LD_LIBDIRS	= $(DEBUG_LD_LIBDIRS)
SHARED_LD_LIBDIRS	= $(DEFAULT_LD_LIBDIRS)

DEFAULT_LD_LIBS		= -lStlUtils -lCommon -lGnuRegex
DEBUG_LD_LIBS		= -lMdb_g -lStlUtils_g 	\
			  -lCommon -lGnuRegex	\
			  -lm $(ARCH_DEBUG_LD_LIBS)
TEST_LD_LIBS		= -lMdb_tg -lStlUtils_g \
			  -lCommon -lGnuRegex	\
			  -lm $(ARCH_DEBUG_LD_LIBS)
PROFILE_LD_LIBS		= $(DEFAULT_LD_LIBS)
STATIC_LD_LIBS		= $(DEFAULT_LD_LIBS)
SHARED_LD_LIBS		= -lMdb_s -lStlUtils_s		\
			  -lCommon_s -lGnuRegex_s	\
			  -lm $(ARCH_DEBUG_LD_LIBS)

# DEP_LIBS - rebuild target if DEP_LIBS newer than target

DEFAULT_DEP_LIBS	= 
DEBUG_DEP_LIBS		= $(PRJ_TOPDIR)/src/Mdb/libMdb_g.a
TEST_DEP_LIBS		= $(PRJ_TOPDIR)/src/Mdb/libMdb_tg.a
PROFILE_DEP_LIBS	= 
STATIC_DEP_LIBS		= 
SHARED_DEP_LIBS		= 

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
	main.C                          \
	tDRBTree.C                      \
	tDRBTree01.C                    \
	tDRBTree02.C                    \
	tDRBTree03.C                    \
	tDRBTree04.C                    \
	tDashSet.C                      \
	tDashSet01.C                    \
	tHashSet.C                      \
	tHashSet01.C                    \
	tHashTable.C                    \
	tHashTable01.C                  \
	tMapFile.C                      \
	tMapFile01.C                    \
	tMapFile02.C                    \
	tMapFile03.C                    \
	tMapMemDlist.C                  \
	tMapMemDynamicDynamic.C         \
	tMapMemDynamicDynamic01.C       \
	tMapMemDynamicDynamic02.C       \
	tMapMemDynamicFixed.C           \
	tMapMemDynamicFixed01.C         \
	tRBSet.C                        \
	tRBTree.C                       \
	tRBTree01.C                     \
	tRBTree02.C                     \
	tRBTree03.C                     \
	$(ARCH_CXX_C_SOURCES)           \

CXX_CC_SOURCES	=			
CXX_CPP_SOURCES	=			

USER_HH_INCS	=			\
	TestAvlTreeOffset.hh            \
	TestFunctProto.hh               \
	$(ARCH_USER_HH_INCS)            \

USER_II_INCS	=			\
	TestAvlTreeOffset.ii            \
	$(ARCH_USER_II_INCS)            \

USER_INCLUDES	=		\
	$(USER_H_INCS)		\
	$(USER_HH_INCS)		\
	$(USER_II_INCS)		\

CXX_HEADERS	=			\
	TestAvlTreeOffset.hh            \
	TestFunctProto.hh               \
	$(ARCH_CXX_HEADERS)             \

CXX_INLINES	=			\
	TestAvlTreeOffset.ii            \
	$(ARCH_CXX_INLINES)             \



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

default debug test profile static shared:: $(generated_files)

default:: $(PROGRAM_DEFAULT)

debug test profile static shared::
	$(hide) $(MAKE) make_target=$@ $(PROGRAM_$($@))

all::	default debug test profile shared

install:: install_$(make_target)
	$(hide) echo + Installation Complete!

install_all:: 

install_all:: install_default install_debug install_profile install_static
	$(hide) echo + Installation Complete!

install_default::
	$(hide) $(MAKE) make_target=default			\
			$(INSTALL_BIN_DIR)/$(PROGRAM_DEFAULT)
install_debug::   
	$(hide) $(MAKE) make_target=debug			\
			$(INSTALL_BIN_DIR)/$(PROGRAM_DEBUG)
install_test::    
	$(hide) $(MAKE) make_target=test			\
			$(INSTALL_BIN_DIR)/$(PROGRAM_TEST)
install_profile:: 
	$(hide) $(MAKE) make_target=profile			\
			$(INSTALL_BIN_DIR)/$(PROGRAM_PROFILE)
install_static::  
	$(hide) $(MAKE) make_target=static			\
			$(INSTALL_BIN_DIR)/$(PROGRAM_STATIC)
install_shared::  
	$(hide) $(MAKE) make_target=shared			\
			$(INSTALL_BIN_DIR)/$(PROGRAM_SHARED)

check: test
	$(PROGRAM_TEST) $(TEST)

#
# include dependancies generated by 'make depend'
#
-include .depend_$(make_target)

#
# =MAKEFILE_LOG=
#
# $Log$
# Revision 2.5  2000/02/28 11:57:58  houghton
# OS Update: with upgrade to 5.6 AVL test are giving me trouble. skiping
#     for now.
#
# Revision 2.4  1999/11/10 12:47:28  houghton
# Changed to use 00-Makefile.cfg.
#
# Revision 2.3  1998/10/23 13:22:09  houghton
# Added MapMemDList test.
#
#
# =MAKEFILE_LOG=
#

# Local Variables:
# mode:makefile
# End:
