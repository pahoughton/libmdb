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

LIB_STLUTILS	= libStlUtils-5

tools_archive_dir	= $(TOOL_DIR)/src/Tools
tools_build_dir		= $(TOOL_DIR)/src/Build/Tools
libs_build_dir		= $(TOOL_DIR)/src/Build/Libs

anon_ftp		= $(tools_bin_dir)/AnonFtp.ksh

cvs_ver			= 1.10
tools_cvs_pkg		= cvs-$(cvs_ver).tar.gz
tools_gzip_pkg		= gzip-1.2.4.tar
tools_host		= sideswipe.wcom.com

exports	    = 					\
	INSTALL_BIN_DIR=$(INSTALL_BIN_DIR)	\
	INSTALL_INC_DIR=$(INSTALL_INC_DIR)	\
	INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)	\
	INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)	\
	show_commands=$(show_commands)		\
	check_install=$(check_install)

no_default:
	$(hide) echo "++ ERROR: no default target available"
	$(hide) exit 1

$(tools_bin_dir) $(tools_build_dir) $(tools_archive_dir):
	$(hide) $(SHELL) $(PROJECT)/support/mkdirhier.sh $@

$(anon_ftp):
	$(hide) cp $(PROJECT)/support/AnonFtp.ksh $@
	$(hide) chmod 775 $@

$(TOOL_DIR)/bin/gzip:
	$(hide)								      \
	pkg=$(tools_gzip_pkg);						      \
	[ -f $(tools_archive_dir)/$$pkg ]				      \
	  || $(anon_ftp) $(tools_host)					      \
	         pub/tools/$$pkg					      \
	         $(tools_archive_dir) || exit 1;			      \
	$(hide) cd $(tools_build_dir)					      \
	&& tar xf $(tools_archive_dir)/$$pkg				      \
	&& cd gzip*							      \
	&& ./configure --prefix=$(TOOL_DIR)				      \
	&& make								      \
	&& make install

$(TOOL_DIR)/bin/cvs:
	$(hide)								      \
	pkg=$(tools_cvs_pkg);						      \
	[ -f $(tools_archive_dir)/$$pkg ]				      \
	  || $(anon_ftp) $(tools_host)					      \
	         pub/tools/$$pkg					      \
	         $(tools_archive_dir) || exit 1; 			      \
	$(hide) cd $(tools_build_dir)					      \
	&& $(TOOL_DIR)/bin/zcat $(tools_archive_dir)/$$pkg | tar xf -	      \
	&& cd cvs-$(cvs_ver)						      \
	&& ./configure --prefix=$(TOOL_DIR)				      \
	&& make								      \
	&& make install


cvs: 		$(tools_archive_dir)					      \
		$(tools_bin_dir)					      \
		$(tools_build_dir)					      \
		$(anon_ftp)						      \
		$(TOOL_DIR)/bin/gzip					      \
		$(TOOL_DIR)/bin/cvs
	$(hide) echo; echo " + CVS installation complete."; echo

check_cvs:
	@if type cvs ; then						      \
	  echo " + cvs found.";						      \
	else								      \
	  echo " + cvs not found please install using the cvs target.";       \
	  echo "   (i.e. make -f $(PROJECT)/support/Setup.Makefile cvs";      \
	  exit 1;							      \
	fi


$(stlutils_hh):
	cd $(libs_build_dir)						      \
	&& ( [ -d $(LIB_STLUTILS) ] || cvs co $(LIB_STLUTILS) )		      \
	&& $(TOOL_DIR)/bin/make -f $(LIB_STLUTILS)/Makefile setup $(exports)  \
	&& $(TOOL_DIR)/bin/make -C $(LIB_STLUTILS) install_lib_all


gen_setup_cfg:
	rm -f $(CFG_DIR)/Setup.cfg
	sed								      \
		-e 's!%INSTALL_INC_DIR%!$(INSTALL_INC_DIR)!'		      \
		-e 's!%INSTALL_LIB_DIR%!$(INSTALL_LIB_DIR)!'		      \
		-e 's!%INSTALL_MAN_DIR%!$(INSTALL_MAN_DIR)!'		      \
	  < $(CFG_DIR)/Setup.cfg.src					      \
	  > $(CFG_DIR)/Setup.cfg
	chmod 444 $(CFG_DIR)/Setup.cfg

setup: check_cvs $(libs_build_dir) $(stlutils_hh) gen_setup_cfg





#
# $Log$
# Revision 1.7  2000/07/24 09:41:50  houghton
# Major rework to improve existing target detection (i.e. if it's there
#      dont rebuild it).
#
# Revision 1.6  2000/05/30 15:27:25  houghton
# Bug-Fix: forgot to define LIB_STLUTILS variable.
#
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
