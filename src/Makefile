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
# Created:	10/20/98 06:53
#
# Revision History: (See end of file for Revision Log)
#
#   Last Mod By:    $Author$
#   Last Mod:	    $Date$
#   Version:	    $Revision$
#
#   $Id$
# 

make_cfg_ver	= 5.06
show_commands 	= # true
check_install	= true

# Include standard MakeConfig configuration for GNU make required
include Make/make.cfg.$(make_cfg_ver)

PRJ_TOPDIR	= ..
CFG_DIR		= $(PRJ_TOPDIR)/src/config

include $(CFG_DIR)/00-Makefile.cfg

INSTALL_BIN_DIR	= $(local_bindir)
INSTALL_INC_DIR = $(local_incdir)
INSTALL_LIB_DIR = $(local_libdir)
INSTALL_MAN_DIR = $(local_mandir)

exports	    = 					\
	INSTALL_BIN_DIR=$(INSTALL_BIN_DIR)	\
	INSTALL_INC_DIR=$(INSTALL_INC_DIR)	\
	INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)	\
	INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)	\
	show_commands=$(show_commands)		\
	check_install=$(check_install)		\

depend depend_all depend_test depend_default depend_debug:
	$(hide) $(MAKE) -C Mdb $@ $(exports)

all default debug:
	$(hide) $(MAKE) -C Mdb $@ $(exports)

clean realclean:
	$(hide) $(MAKE) -C Mdb $@ $(exports)
	$(hide) $(MAKE) -C $(DOC_DIR) $@ $(exports)
	$(hide) $(MAKE) -C $(TEST_DIR) $@ $(exports)

test:
	$(hide) $(MAKE) -C Mdb $@ $(exports)

check: test
	$(hide) $(MAKE) -C $(TEST_DIR) $@ $(exports)

install_docs:
	$(hide) $(MAKE) -C $(DOC_DIR) $@ $(exports)

install_all install install_default install_debug: install_docs
	$(hide) $(MAKE) -C Mdb $@ $(exports)






#
# $Log$
# Revision 1.2  1999/10/30 11:50:28  houghton
# Changed (complete rework) to support setup and other new targets.
#
# Revision 1.1  1998/10/23 13:15:54  houghton
# Initial Version.
#
#

# Local Variables:
# mode:makefile
# End:
