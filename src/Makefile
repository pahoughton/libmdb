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

# Include standard MakeConfig configuration for GNU make required
include Make/make.cfg.$(make_cfg_ver)

SRC_DIR		= Mdb

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
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi

all default debug:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi

clean realclean:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi

test:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi


install_all install install_default install_debug:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi





#
# $Log$
# Revision 1.1  1998/10/23 13:15:54  houghton
# Initial Version.
#
#

# Local Variables:
# mode:makefile
# End:
