# -*- makefile -*- for Mdb
#
# Created: 5/8/97 using MakeConfigs 5.01.00.a.09
#
# $Id$
#
# (see end of file for Log info)

#
# Do not modifiy 'makefile_type'
makefile_type   = lib
# This is the make.cfg version that will be used
make_cfg_ver	= 5.01
# Default target type
make_target	= default
# Uncomment 'show_compile_command' to output compile command.
# Comment check_install (or set to any value but 'true') to install over
#     the same version.
# Comment out proto_dep (or set to any value but true) to prevent
#     regen of proto header
show_commands 	= # true
check_install   = true
proto_dep       = true

# Include standard MakeConfig configuration for GNU make required
include Make/make.cfg.$(make_cfg_ver)

# 
# =USE_CURRENT_MAKEFILE=
# When you re-gen a make file, every thing between 
# these tags will be kept in the new makefile
# if you want it re-gen'd, just delete the tags
#

NAME		= Mdb

INSTALL_LIBDIR	= $(beta_libdir)
INSTALL_INCDIR	= $(beta_incdir)

VER_MAJOR	= 1
VER_MINOR	= 00
VER_RELEASE	= 00.a.01
VERSION		= $(VER_MAJOR).$(VER_MINOR).$(VER_RELEASE)

LIBRARY_DEFAULT	= lib$(NAME).a
LIBRARY_DEBUG	= lib$(NAME)_g.a
LIBRARY_PROFILE	= lib$(NAME)_pg.a
LIBRARY_SHARED	= lib$(NAME)$(shlib_ext)


PRJ_TOPDIR	= ../..

# include any arch specific makefile component
-include $(PRJ_TOPDIR)/src/arch/$(arch)/Makefile.$(NAME)

VPATH		= $(ARCH_VPATH):.

###### D E F I N E S  #####
# 
# Config defines avalable
#
#


SRC_DEFINES	= 
PRJ_DEFINES	= -DVERSION=\"$(VERSION)\" $(ARCH_DEFINES)

DEFAULT_DEFINES = -DMDB_DEFAULT=1
DEBUG_DEFINES	= -DMDB_DEBUG=1 -DCLUE_DEBUG=1
PROFILE_DEFINES	= -DMDB_PROFILE=1
SHARED_DEFINES	= -DMDB_SHARED=1

######  I N C L U D E S #####
#
# Config includes available (need to use -I)
#
# $(alpha_incdir) 	$(beta_incdir) 	$(local_incdir)
#


SRC_INC		= $(ARCH_INC) -I. -I$(PRJ_TOPDIR)
PRJ_INC		= -I$(beta_incdir)


# Any other cpp flags to use on for all targets

CPP_FLAGS		= $(ARCH_CPP_FLAGS)

###### C O M P I L E / C P P #####
#
# Config flags available
#
# $(cc_optimize) $(cxx_optimize)
# $(cc_debug)	 $(cxx_debug)
# $(cc_profile)	 $(cxx_profile)
# $(cc_warn)	 $(cxx_warn)
#


CC_FLAGS		= $(cc_warn) $(ARCH_CC_FLAGS)

DEFAULT_CC_FLAGS	= $(cc_optimize)
DEBUG_CC_FLAGS		= $(cc_debug)
PROFILE_CC_FLAGS	= $(cc_profile)
SHARED_CC_FLAGS		= $(cc_shared) $(cc_optimize)

CXX_FLAGS		= $(cxx_warn) $(ARCH_CXX_FLAGS)

DEFAULT_CXX_FLAGS	= $(cxx_optimize)
DEBUG_CXX_FLAGS		= $(cxx_debug)
PROFILE_CXX_FLAGS	= $(cxx_profile)
SHARED_CXX_FLAGS	= $(cxx_shared) $(cxx_optimize)

# **** S H A R E D   D E P   L I B S *****

SHDEP_LIBS		= -lc

# **** I N S T A L L ****

INSTALL_LIB_FLAGS	= -m 664
INSTALL_INC_FLAGS	= -m 664

#
# =USE_CURRENT_MAKEFILE=
#

# **** S O U R C E   F I L E S ****

CXX_C_SOURCES	=			\
	MapFile.C			\
	MapMem.C			\
	MapMemDynamic.C			\
	MapMemDynamicDynamic.C		\
	ChunkMgr.C			\
	HashTableBase.C			\
	DashTableBase.C			\
	MultiMem.C			\
	MultiMemOffset.C		\
	MultiMemOffsetMapFixed.C	\
	MapMemFixedDynamic.C		\

hold = \
	MapMemDynamicDynamic.C		\
	$(ARCH_CXX_C_SOURCES)           \


USER_H_INCS	=			\
	$(ARCH_USER_H_INCS)             \

USER_HH_INCS	=			\
	ChunkMgr.hh                     \
	MdbConfig.hh                    \
	MdbVersion.hh                   \
	MapFile.hh			\
	MapMem.hh			\
	MapMemDynamic.hh		\
	MapMemDynamicDynamic.hh		\
	MapMemFixedDynamic.hh		\
	HashTableBase.hh		\
	DashTableBase.hh		\
	DashTable.hh			\
	Dash.hh				\
	DavlTreeOffset.hh		\
	AvlTreeOffset.hh		\
	DavlTreeOffsetBase.hh		\
	AvlTreeOffsetBase.hh		\
	AvlTreeBase.hh			\
	MultiMem.hh			\
	MultiMemOffset.hh		\
	MultiMemOffsetMapFixed.hh	\
	$(ARCH_USER_HH_INCS)            \

USER_II_INCS	=			\
	ChunkMgr.ii                     \
	MapFile.ii			\
	MapMem.ii			\
	MapMemDynamic.ii		\
	MapMemDynamicDynamic.ii		\
	HashTableBase.ii		\
	DashTableBase.ii		\
	DashTable.ii			\
	Dash.ii				\
	$(ARCH_USER_II_INCS)            \

USER_INCLUDES	=		\
	$(USER_H_INCS)		\
	$(USER_HH_INCS)		\
	$(USER_II_INCS)		\

C_HEADERS	=			\
	tree.h                          \
	$(ARCH_C_HEADERS)               \

CXX_HEADERS	=			\
	ChunkMgr.hh                     \
	MdbConfig.hh                    \
	MdbVersion.hh                   \
	RBTreeBase.hh                   \
	$(ARCH_CXX_HEADERS)             \

CXX_INLINES	=			\
	RBTreeBase.ii                   \
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


default debug profile shared:: proto

default:: $(LIBRARY_$($(make_target)))

debug profile shared:: 
	$(hide) $(MAKE) make_target=$@ $(LIBRARY_$($@))

all::	default debug profile shared


install:: install_lib_check install_$(make_target)
	$(hide) echo Installation Complete!

install_all:: install_lib_check

install_all:: install_default install_debug install_profile install_shared
	$(hide) echo Installation Complete!

install_default:: default $(INSTALL_LIBDIR)/$(LIBRARY_DEFAULT)
install_debug::   debug   $(INSTALL_LIBDIR)/$(LIBRARY_DEBUG)
install_profile:: profile $(INSTALL_LIBDIR)/$(LIBRARY_PROFILE)
install_shared::  shared  $(INSTALL_LIBDIR)/$(LIBRARY_SHARED)
	$(hide) chmod +x $(INSTALL_LIBDIR)/$(LIBRARY_SHARED)


test::	default

FORCE: 

#
# include dependancies generated by 'make depend'
#
-include .depend_$(make_target)


#
# =MAKEFILE_LOG=
#
# $Log$
# Revision 2.3  1997/06/18 14:14:21  houghton
# Added MultiMem classes and MapMemFixedDynamic.
#
# Revision 2.2  1997/06/05 13:44:02  houghton
# Fixed up install values.
#
# Revision 2.1  1997/06/05 11:29:11  houghton
# Initial Version.
#
#
# =MAKEFILE_LOG=
#




