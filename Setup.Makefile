#
# File:         Setup.Makefile
# Project:	Mdb 
# Desc:
#
#   This is the top level Setup.Makefile used to install/verify all
#   prerequisite components for the project. It's setup target
#   should be the first thing built after installing the Project sources.
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

SHELL	= /bin/ksh
hide	= @

PROJECT_NAME		= Mdb
PROJECT_VER_MAJOR	= 4
PROJECT_VER_MINOR	= 01
PROJECT_VER_RELEASE	= 01

PROJECT_VER_MGMT	= DIM
PROJECT_VER_DIM_DB	= COS
PROJECT_VER_DIM_PRODUCT	= INFR_OBJS
PROJECT_VER_DIM_RELEASE	= \
	$(PROJECT_NAME)							\
	$(PROJECT_VER_MAJOR).$(PROJECT_VER_MINOR).$(PROJECT_VER_RELEASE)

PROJECT_VER_PROD	= $(PROJECT_VER_MAJOR).$(PROJECT_VER_MINOR)
PROJECT_VER_PREV	= 0.00.00

PROJECT_CONTACT_NAME	= Paul Houghton
PROJECT_CONTACT_EMAIL	= Paul.Houghton@mci.com

PROJECT_DESC	=							\
    The Mdb library is a collection of classes that provide high	\
    performance real time memory database functionality. These classes	\
    were developed using ANSI Standard C++ and Unix mapped memory 	\
    system calls.


PROJECT_DIR		=	\
	$(PROJECT_NAME)-$(PROJECT_VER_MAJOR)

PROJECT_EXTRACT_DIR	= src/Build/Libs


WWW_TOP_DIR		= ../../..
WWW_TOOLS_ROOT		= $(WWW_TOP_DIR)/Tools
WWW_OBJECT_REP_ROOT	= $(WWW_TOP_DIR)/ObjectRep
WWW_PROJECTS_ROOT	= $(WWW_TOP_DIR)/Projects

RUN_BASE_DIR		= $(TOOL_DIR)
INSTALL_BASE_DIR	= $(TOOL_DIR)

make_cfg_ver		= 6.01
tools_host		= soundwave.mcilink.com
tools_host_dir		= pub/tools

CONFIG_VARS	=		\
	RUN_BASE_DIR		\
	INSTALL_BASE_DIR	\
	INSTALL_BIN_DIR		\
	INSTALL_LIB_DIR		\
	INSTALL_INC_DIR		\
	INSTALL_JAVA_DIR	\
	INSTALL_DOC_HTML_DIR	\
	INSTALL_DOC_MAN_DIR	\
				\
	tools_host		\
	tools_host_dir

PROJECT_VARS	=		\
	PROJECT_NAME		\
	PROJECT_VER_MAJOR	\
	PROJECT_VER_MINOR	\
	PROJECT_VER_RELEASE	\
	PROJECT_VER_MGMT	\
	PROJECT_VER_DIM_DB	\
	PROJECT_VER_DIM_PRODUCT	\
	PROJECT_VER_DIM_RELEASE	\
	PROJECT_VER_PROD	\
	PROJECT_VER_PREV	\
	PROJECT_CONTACT_NAME	\
	PROJECT_CONTACT_EMAIL	\
	PROJECT_DIR		\
	PROJECT_EXTRACT_DIR	\
				\
	WWW_TOP_DIR		\
	WWW_TOOLS_ROOT		\
	WWW_OBJECT_REP_ROOT	\
	WWW_PROJECTS_ROOT	\
				\
	make_cfg_ver		\
				\
	PROJECT_VARS		\
	CONFIG_VARS

setup_exports	=						\
	PROJECT_NAME='$(PROJECT_NAME)'				\
	PROJECT_VER_MAJOR='$(PROJECT_VER_MAJOR)'		\
	PROJECT_VER_MINOR='$(PROJECT_VER_MINOR)'		\
	PROJECT_VER_RELEASE='$(PROJECT_VER_RELEASE)'		\
	PROJECT_VER_MGMT='$(PROJECT_VER_MGMT)'			\
	PROJECT_VER_DIM_DB='$(PROJECT_VER_DIM_DB)'		\
	PROJECT_VER_DIM_PRODUCT='$(PROJECT_VER_DIM_PRODUCT)'	\
	PROJECT_VER_DIM_RELEASE='$(PROJECT_VER_DIM_RELEASE)'	\
	PROJECT_VER_PROD='$(PROJECT_VER_PROD)'			\
	PROJECT_VER_PREV='$(PROJECT_VER_PREV)'			\
	PROJECT_CONTACT_NAME='$(PROJECT_CONTACT_NAME)'		\
	PROJECT_CONTACT_EMAIL='$(PROJECT_CONTACT_EMAIL)'	\
	PROJECT_DIR='$(PROJECT_DIR)'				\
	PROJECT_EXTRACT_DIR='$(PROJECT_EXTRACT_DIR)'		\
	make_cfg_ver='$(make_cfg_ver)'				\
	tools_host='$(tools_host)'				\
	tools_host_dir='$(tools_host_dir)'			\
	PROJECT_VARS='$(PROJECT_VARS)'				\
	CONFIG_VARS='$(CONFIG_VARS)'				\
	RUN_BASE_DIR='$(RUN_BASE_DIR)'				\
	INSTALL_BASE_DIR='$(INSTALL_BASE_DIR)'			\
	INSTALL_BIN_DIR='$(INSTALL_BIN_DIR)'			\
	INSTALL_LIB_DIR='$(INSTALL_LIB_DIR)'			\
	INSTALL_INC_DIR='$(INSTALL_INC_DIR)'			\
	INSTALL_JAVA_DIR='$(INSTALL_JAVA_DIR)'			\
	INSTALL_DOC_HTML_DIR='$(INSTALL_DOC_HTML_DIR)'		\
	INSTALL_DOC_MAN_DIR='$(INSTALL_DOC_MAN_DIR)'		\
	WWW_TOP_DIR=$(WWW_TOP_DIR)				\
	WWW_TOOLS_ROOT=$(WWW_TOOLS_ROOT)			\
	WWW_OBJECT_REP_ROOT=$(WWW_OBJECT_REP_ROOT)		\
	WWW_PROJECTS_ROOT=$(WWW_PROJECTS_ROOT)			\
	hide='$(hide)'

config_help	=							\
	echo "    RUN_BASE_DIR=$(RUN_BASE_DIR) ";			\
	echo "    INSTALL_BASE_DIR=$(INSTALL_BASE_DIR) ";		\
	echo "    INSTALL_BIN_DIR=$(INSTALL_BIN_DIR) ";			\
	echo "    INSTALL_LIB_DIR=$(INSTALL_LIB_DIR) ";			\
	echo "    INSTALL_INC_DIR=$(INSTALL_INC_DIR) ";			\
	echo "    INSTALL_JAVA_DIR=$(INSTALL_JAVA_DIR) ";		\
	echo "    INSTALL_DOC_HTML_DIR=$(INSTALL_DOC_HTML_DIR) ";	\
	echo "    INSTALL_DOC_MAN_DIR=$(INSTALL_DOC_MAN_DIR) ";		\
	echo "    WWW_TOP_DIR=$(WWW_TOP_DIR) ";				\
	echo "    WWW_TOOLS_ROOT=$(WWW_TOOLS_ROOT) ";			\
	echo "    WWW_OBJECT_REP_ROOT=$(WWW_OBJECT_REP_ROOT) ";		\
	echo "    WWW_PROJECTS_ROOT=$(WWW_PROJECTS_ROOT) ";		\
	echo "    tools_host=$(tools_host) ";				\
	echo "    tools_host_dir=$(tools_host_dir) ";

#
# Targets
#

gen_setup_exports:
	@$(foreach var,$(PROJECT_VARS) $(CONFIG_VARS),		\
		echo "	$(var)='\$$($(var))' \\";)
	@echo "	hide='\$$(hide)' \\";

gen_config_help:
	@$(foreach var,$(subst CONFIG_VARS,,$(CONFIG_VARS)),	      \
		     echo "	echo \"    $(var)=\$$($(var)) \"; \\";)

default: help

setup:
	$(hide) if [ -z "$$TOOL_DIR" ] ||				 \
	      [ -z `echo $$PATH						 \
		 | sed  -n -e "s@.*\($$TOOL_DIR/bin\).*@\1@p"` ]; then \
	  echo ;							 \
	  echo "The \$$TOOL_DIR environment variable is not set" ;	 \
	  echo "or \$$TOOL_DIR/bin is not in the path. Please see" ;	 \
	  echo "$(PROJECT_DIR)/docs/html/User/Install.html for" ;	 \
	  echo "instructions.";						 \
	  echo ;							 \
	  exit 1;							 \
	fi
	$(hide) if [ ! -d $(PROJECT_DIR) ] ; then			      \
	  echo "$(PROJECT_DIR) is not a directory. The setup target must";    \
	  echo "be built from the directory that contains ";		      \
	  echo "$(PROJECT_DIR) with the command:";			      \
	  echo ;							      \
	  echo "    make -f $(PROJECT_DIR)/Makefile.Setup setup";	      \
	  echo								      \
	  exit 1;							      \
	else								      \
	  echo "+ Starting Setup of $(PROJECT_NAME)" `date`;		      \
	  $(MAKE) -f $(PROJECT_DIR)/support/MakeConfigs.Makefile setup	      \
		$(setup_exports)   					      \
		EXTRACT_DIR=`pwd`					      \
		|| exit 1;						      \
	  $(TOOL_DIR)/bin/make -f $(PROJECT_DIR)/support/Setup.Makefile setup \
		$(setup_exports)   					      \
		EXTRACT_DIR=`pwd`					      \
		|| exit 1;						      \
	  echo $(PROJECT_DESC) |					      \
	    perl -ne 'use Text::Wrap; print fill("","", $$_ ),"\n";'	      \
	    > $(PROJECT_DIR)/src/config/.prjdesc.txt ;			      \
	  echo ;							      \
	  echo "+ Setup $(PROJECT_NAME) Complete -" `date`;		      \
	  echo ;							      \
	fi

help:
	@echo "The only valid target is 'setup' and it should be"
	@echo "built from the directory above the one containing"
	@echo "this makefile with the command:"
	@echo
	@echo "    make -f $(PROJECT_DIR)/Setup.Makefile setup";
	@echo
	@echo "To watch the commands that get executed, add `hide='"
	@echo "to the command line above."
	@echo
	@echo "To see a list of configuration variables and their"
	@echo "current values, make the \`help_config' target."

help_config:
	@echo
	@echo " The following configuration variables are available"
	@echo " See the Detailed Documentation at the end of this"
	@echo " makefile for descriptions."
	@echo
	@$(config_help)
	@echo
#
# Detailed Documentation
#
# Targets
#
#   setup:
#
#	The setup target installs and/or verifies all prerequisite
#	components for the project. It needs to be the first target
#	build after the project's sources have been installed. Any
#	configuration settings will be saved in the makefile fragment
#	src/config/Setup.cfg which is used by all other makefiles in
#	the project.
#
#	This target must be build from the parent directory of the
#	project's top level directory with the following command:
#
#	    make -f $(PROJECT_DIR)/Makefile.Setup setup
#
#	You can set any of the configuration variables on the command
#	line and those values will be written to src/config/Setup.cfg
#	and used for all future build targets.
#
#   help:
#
#	The help target describes the available targets.
#
#   help_config:
#
#	This target list the configuration variables and their current
#	settings. 
#
#   gen_setup_exports
#
#	Once GNU Make is installed (i.e. setup complete). This target
#	can be used to to regenerate the values for the setup_exports
#	variable. It just list the correct values, you have to paste
#	them into the makefile.
#
#   gen_config_help
#
#	Once GNU Make is installed (i.e. setup complete). This target
#	can be used to to regenerate the values for the config_help
#	variable. It just list the correct values, you have to paste
#	them into the makefile.
#
# Variables
#
#   CONFIG_VARS
#
#	The list of configuration variables to use for setup and save
#	in the src/config/Setup.cfg file. The value of these variables
#	will be shown with the `help_config' targets in all the
#	project's Makefiles.
#
#   PROJECT_VARS
#
#	This is also a list of variables and values that will be
#	written to the src/config/Setup.cfg file. But, these variables
#	and values will NOT be shown with the `help_config' targets.
#
#   PROJECT_NAME
#
#	The name of the project
#
#   PROJECT_VER_MAJOR
#
#	The major version number of the project (i.e. for version 2.3.5
#	this would be the `2').
#
#   PROJECT_VER_MINOR
#
#	The minor version number of the project (i.e. for version 2.3.5
#	this would be the `3').
#
#   PROJECT_VER_RELEASE
#
#	The release version number of the project (i.e. for version 2.3.5
#	this would be the `5').
#
#   PROJECT_DIR
#
#	The name of the top level directory for the project.
#
#   TOOL_DIR
#
#	This is not a Makefile variable, but it is a very important
#	environment variable. It must be set and $TOOL_DIR/bin
#	must be in your path so that the prerequisite programs that
#	are installed will be found before any other version of these
#	programs that might be on the system.
#
#   INSTALL_BASE_DIR
#
#	The base directory for installing all of the components
#	provided by this project. The default is $TOOL_DIR
#
#   INSTALL_BIN_DIR
#
#	The directory to install executable programs and scripts.
#
#   INSTALL_DOC_HTML_DIR
#
#	The base directory HTML documentation files will be
#	installed. The default is
#	$(INSTALL_BASE_DIR)/html/$(PROGRAM_DIR) 
#
#   INSTALL_DOC_MAN_DIR
#
#	The base directory Unix `man' pages will be installed. The
#	default is $(INSTALL_BASE_DIR)/man
#
#   make_cfg_ver
#
#	The version of MakeConfigs to use. The default is 6.01
#
#   tools_host
#
#	The anonymous ftp host that has all the prerequisite packages.
#
#   tools_host_dir
#
#	The directory on the anonymous ftp host that contains all the
#	prerequisite packages.
#
#   hide
#
#	All commands are prefixed with this variable so the actual
#	commands can be hidden during the build. It defaults to `@'
#	so the commands are hidden, to see the commands, set it to the
#	empty string (i.e. hide= ).
#
#

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
