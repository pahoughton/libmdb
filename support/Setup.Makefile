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

PROJECT		= libMdb-2

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

$(libs_build_dir)/libStlUtils-4:
	cd $(libs_build_dir)						      \
	&& cvs $(tools_cvsroot) co libStlUtils-4


$(stlutils_hh): $(libs_build_dir)/libStlUtils-4
	cd $(libs_build_dir)						      \
	&& $(MAKE) -f libStlUtils-4/Makefile setup
	$(TOOL_DIR)/bin/make -C $(libs_build_dir)/libStlUtils-4		      \
	    install_all $(exports) 

setup: check_cvs $(stlutils_hh)





#
# $Log$
# Revision 1.1  1999/10/30 11:51:07  houghton
# Initial Version.
#
#

# Local Variables:
# mode:makefile
# End:
