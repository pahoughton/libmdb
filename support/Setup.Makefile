#
# Title:        Setup.Makefile
# Project:	Mdb-2
# Desc:
# 
#   
# 
# Notes:
# 
# Author:	Paul Houghton - (paul.houghton@wcom.com)
# Created:	10/30/99 06:44
#
# Revision History: (See end of file for Revision Log)
#
#   Last Mod By:    $Author$
#   Last Mod:	    $Date$
#   Version:	    $Revision$
#
#   $Id$
# 

SHELL		= /bin/ksh

PROJECT		= libMdb-3
CFG_DIR		= $(PROJECT)/src/config

stlutils_hh	= $(INSTALL_INC_DIR)/StlUtilsConfig.hh 


tools_build_dir		= $(TOOL_DIR)/src/Build/Tools
libs_build_dir		= $(TOOL_DIR)/src/Build/Libs

exports	    = 					\
	INSTALL_BIN_DIR=$(INSTALL_BIN_DIR)	\
	INSTALL_INC_DIR=$(INSTALL_INC_DIR)	\
	INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)	\
	INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)	\
	show_commands=$(show_commands)		\
	check_install=$(check_install)

no_target:

$(tools_archive_dir)/$(tools_cvs):
	cd $(libs_build_dir)						      \
	&& $(PROJECT)/support/AnonFtp.ksh				      \
	      $(tools_host)						      \
	      pub/tools/$(tools_cvs)					      \
	      $(tools_archive_dir)

$(TOOL_DIR)/bin/cvs: $(tools_archive_dir)/$(tools_cvs)			      \
		$(TOOL_DIR)/bin/gzip
	cd $(tools_build_dir)						      \
	&& zcat $(tools_archive_dir)/$(tools_cvs) | tar xf -
	cd $(tools_build_dir)/cvs*					      \
	&& ./configure --prefix=$(TOOL_DIR)				      \
	&& make								      \
	&& make install

cvs: $(TOOL_DIR)/bin/cvs

check_cvs:
	@if type cvs ; then						      \
	  echo " + cvs found.";						      \
	else								      \
	  echo " + cvs not found please install using the cvs target.";       \
	  echo "   (i.e. make -f $(PROJECT)/support/Setup.Makefile cvs";      \
	  exit 1;							      \
	fi

$(libs_build_dir)/$(LIB_STLUTILS):
	cd $(libs_build_dir)						      \
	&& cvs $(tools_cvsroot) co $(LIB_STLUTILS)


$(stlutils_hh): $(libs_build_dir)/$(LIB_STLUTILS)
	cd $(libs_build_dir)						      \
	&& $(MAKE) -f $(LIB_STLUTILS)/Makefile setup $(exports)
	$(TOOL_DIR)/bin/make -C $(libs_build_dir)/$(LIB_STLUTILS)	      \
	    install_all $(exports) 

gen_setup_cfg:
	rm -f $(CFG_DIR)/Setup.cfg
	sed								      \
		-e 's!%INSTALL_INC_DIR%!$(INSTALL_INC_DIR)!'		      \
		-e 's!%INSTALL_LIB_DIR%!$(INSTALL_LIB_DIR)!'		      \
		-e 's!%INSTALL_MAN_DIR%!$(INSTALL_MAN_DIR)!'		      \
	  < $(CFG_DIR)/Setup.cfg.src					      \
	  > $(CFG_DIR)/Setup.cfg
	chmod 444 $(CFG_DIR)/Setup.cfg

setup: check_cvs $(stlutils_hh) gen_setup_cfg





#
# $Log$
# Revision 1.5  2000/05/30 13:01:46  houghton
# Changed to use LIB_STLUTILS variable (vs libStlUtils-4).
#
# Revision 1.4  2000/05/26 11:44:59  houghton
# Changed Project to Version 3.
#
# Revision 1.3  2000/01/03 14:18:49  houghton
# Bug-Fix: forgot to use exports.
#
# Revision 1.2  1999/11/09 10:54:36  houghton
# Added gen_setup_cfg target.
#
# Revision 1.1  1999/10/30 11:51:07  houghton
# Initial Version.
#
#

# Local Variables:
# mode:makefile
# End:
