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
hide		= @

PROJECT		= libMdb-2

PRJ_TOPDIR	= .
CFG_DIR		= $(PRJ_TOPDIR)/src/config

INSTALL_INC_DIR = $(TOOL_DIR)/include/prod
INSTALL_LIB_DIR = $(TOOL_DIR)/lib/prod
INSTALL_BIN_DIR	= $(TOOL_DIR)/bin
INSTALL_MAN_DIR = $(TOOL_DIR)/man

stlutils_hh	= $(INSTALL_INC_DIR)/StlUtilsConfig.hh

setup_exports	    = 				\
	INSTALL_INC_DIR=$(INSTALL_INC_DIR)	\
	INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)	\
	INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)	\
	show_commands=$(show_commands)		\
	check_install=$(check_install)		\

exports			=			\
	show_commands=$(show_commands)		\
	check_install=$(check_install)		\

no_target: help

setup:
	$(MAKE) -f $(PROJECT)/support/Setup.Makefile $(setup_exports) setup
	$(TOOL_DIR)/bin/make -C $(PROJECT) realclean depend_all
	$(hide) echo 
	$(hide) echo "+ $(PROJECT) setup complete."
	$(hide) echo 


verify_setup:
	$(hide) if [ -z "$$TOOL_DIR" ] ; then				      \
	  echo "TOOL_DIR env var not set.";				      \
	  echo "  Please see $(PROJECT)/docs/devel/Dependencies.txt";	      \
	  echo "  for details.";					      \
	  exit 1;							      \
	fi
	$(hide) if [ ! -f "$(stlutils_hh)" ] ; then			      \
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
install_lib_all								      \
install_default								      \
install_debug								      \
install									      \
install_all: verify_setup
	$(hide) $(TOOL_DIR)/bin/make -C $(PRJ_TOPDIR)/src $@ $(exports)
	$(hide) echo + $(PROJECT) $@ complete

help targets:
	$(hide) echo " + The following targets are available:"
	$(hide) echo 
	$(hide) echo "    setup"
	$(hide) echo 
	$(hide) echo "    depend_all"
	$(hide) echo "    depend_debug"
	$(hide) echo "    depend_default"
	$(hide) echo "    debug"
	$(hide) echo "    default"
	$(hide) echo "    test (testing version)"
	$(hide) echo "    shared"
	$(hide) echo "    all"
	$(hide) echo "    check (run tests)"
	$(hide) echo "    clean"
	$(hide) echo "    realclean"
	$(hide) echo "    install_docs"
	$(hide) echo "    install_lib_all"
	$(hide) echo "    install_default"
	$(hide) echo "    install_debug"
	$(hide) echo "    install"
	$(hide) echo "    install_all"
	$(hide) echo
	$(hide) echo " + Use the help_config target to see the available"
	$(hide) echo "   configuration overides."
	$(hide) echo

help_config:
	$(hide) if [ -f $(CFG_DIR)/Setup.cfg ] ; then			      \
	  $(MAKE) -f $(CFG_DIR)/Setup.cfg help_config ;			      \
	else								      \
	  if [ -f $(PROJECT)/$(CFG_DIR) ] ; then			      \
	    $(MAKE) -f $(PROJECT)/$(CFG_DIR)/Setup.cfg help_config ;	      \
	  else								      \
	    echo ;							      \
	    echo "+ The following configuration variables are available:" ;   \
	    echo ;							      \
	    echo "    INSTALL_INC_DIR=$(INSTALL_INC_DIR)" ;		      \
	    echo "    INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)" ;		      \
	    echo "    INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)" ;		      \
	    echo "    show_commands=$(show_commands)" ;			      \
	    echo "    check_install=$(check_install)" ;			      \
	    echo ;							      \
	  fi ;								      \
	fi 





#
# $Log$
# Revision 1.4  1999/11/09 10:53:50  houghton
# Changed setup to generate Setup.cfg.
#
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
