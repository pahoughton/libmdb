#
# Title:        Makefile
# Project:	Mdb
# Desc:
# 
#   
# 
# Notes:
# 
# Author:	Paul Houghton - (paul.houghton@wcom.com)
# Created:	10/20/98 06:52
#
# Revision History: (See end of file for Revision Log)
#
#   Last Mod By:    $Author$
#   Last Mod:	    $Date$
#   Version:	    $Revision$
#
#   $Id$
# 

show_commands	= # true
check_install	= true

PROJECT		= libMdb-2

PRJ_TOPDIR	= .
CFG_DIR		= $(PRJ_TOPDIR)/src/config

INSTALL_INC_DIR = $(TOOL_DIR)/include/prod
INSTALL_LIB_DIR = $(TOOL_DIR)/lib/prod
INSTALL_BIN_DIR	= $(TOOL_DIR)/bin
INSTALL_MAN_DIR = $(TOOL_DIR)/man

stlutils_hh	= $(INSTALL_INC_DIR)/StlUtilsConfig.hh

exports	    = 					\
	INSTALL_BIN_DIR=$(INSTALL_BIN_DIR)	\
	INSTALL_INC_DIR=$(INSTALL_INC_DIR)	\
	INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)	\
	INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)	\
	show_commands=$(show_commands)		\
	check_install=$(check_install)

no_target: help

setup:
	$(MAKE) -f $(PROJECT)/support/Setup.Makefile $(exports) setup

verify_setup:
	@ if [ -z "$$TOOL_DIR" ] ; then					      \
	  echo "TOOL_DIR env var not set.";				      \
	  echo "  Please see $(PROJECT)/docs/devel/Dependencies.txt";	      \
	  echo "  for details.";					      \
	  exit 1;							      \
	fi
	@ if [ ! -f "$(stlutils_hh)" ] ; then				      \
	  echo " ";							      \
	  echo "+ $(stlutils_hh) NOT FOUND!";				      \
	  echo " ";							      \
	  echo "    To install all the dependencies, please perform";	      \
	  echo "    the following:";				      \
	  echo " ";							      \
	  echo "      cd \$$TOOL_DIR/src/Build/Libs";			      \
	  echo "      make -f $(PROJECT)/Makefile setup";		      \
	  echo " ";							      \
	  echo "    Please see $(PROJECT)/docs/devel/Dependencies.txt";	      \
	  echo "    for details.";					      \
	  echo " ";							      \
	  exit 1;							      \
	fi


depend_all								      \
depend_debug								      \
depend_default								      \
debug									      \
default									      \
test									      \
shared									      \
all									      \
check									      \
clean									      \
realclean								      \
install_docs								      \
install_default								      \
install_debug								      \
install									      \
install_all: verify_setup
	@ $(TOOL_DIR)/bin/make -C $(PRJ_TOPDIR)/src $@ $(exports)
	@ echo + $(PROJECT) $@ complete

help targets:
	@ echo " + The following targets are available:"
	@ echo 
	@ echo "    setup"
	@ echo 
	@ echo "    depend_all"
	@ echo "    depend_debug"
	@ echo "    depend_default"
	@ echo "    debug"
	@ echo "    default"
	@ echo "    test (testing version)"
	@ echo "    shared"
	@ echo "    all"
	@ echo "    check (run tests)"
	@ echo "    clean"
	@ echo "    realclean"
	@ echo "    install_docs"
	@ echo "    install_default"
	@ echo "    install_debug"
	@ echo "    install"
	@ echo "    install_all"
	@ echo
	@ echo " + Use the help_config target to see the available"
	@ echo "   configuration overides."
	@ echo

help_config:
	@ echo " + The following configuration variables are available:"
	@ echo
	@ echo "    INSTALL_BIN_DIR=$(INSTALL_BIN_DIR)"
	@ echo "    INSTALL_INC_DIR=$(INSTALL_INC_DIR)"
	@ echo "    INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)"
	@ echo "    INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)"
	@ echo "    show_commands=$(show_commands)"
	@ echo "    check_install=$(check_install)"
	@ echo





#
# $Log$
# Revision 1.3  1999/10/30 11:50:23  houghton
# Changed (complete rework) to support setup and other new targets.
#
# Revision 1.2  1999/07/02 09:26:01  houghton
# Changed if $(MAKE) ... to $(MAKE) ...
#
# Revision 1.1  1998/10/23 13:15:45  houghton
# Initial Version.
#
#

# Local Variables:
# mode:makefile
# End:
