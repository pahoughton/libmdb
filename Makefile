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

make_cfg_ver	= 5.06
show_commands 	= # true
check_install	= true

# Include standard MakeConfig configuration for GNU make required
include Make/make.cfg.$(make_cfg_ver)


INSTALL_INC_DIR = $(local_incdir)
INSTALL_LIB_DIR = $(local_libdir)
INSTALL_BIN_DIR	= $(local_bindir)
INSTALL_MAN_DIR = $(local_mandir)

SRC_DIR		= src
DOC_DIR		= docs
TEST_DIR	= test

exports	    = 					\
	INSTALL_BIN_DIR=$(INSTALL_BIN_DIR)	\
	INSTALL_INC_DIR=$(INSTALL_INC_DIR)	\
	INSTALL_LIB_DIR=$(INSTALL_LIB_DIR)	\
	INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)	\
	show_commands=$(show_commands)		\
	check_install=$(check_install)		\

beta_exports	=				\
	INSTALL_BIN_DIR=$(INSTALL_BIN_DIR)	\
	INSTALL_INC_DIR=$(beta_incdir)		\
	INSTALL_LIB_DIR=$(beta_libdir)		\
	INSTALL_MAN_DIR=$(INSTALL_MAN_DIR)	\
	show_commands=$(show_commands)		\
	check_install=$(check_install)		\

.PHONY: test

depend depend_all depend_default depend_debug:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi

depend_test:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi
	$(hide) if ! $(MAKE) -C $(TEST_DIR) $@ $(exports); then exit; fi

all default debug:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi

clean realclean:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi
	$(hide) if ! $(MAKE) -C $(DOC_DIR) $@ $(exports); then exit; fi
	$(hide) if ! $(MAKE) -C $(TEST_DIR) $@ $(exports); then exit; fi

test:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi
	$(hide) if ! $(MAKE) -C $(TEST_DIR) $@ $(exports); then exit; fi

install_doc:
	$(hide) if ! $(MAKE) -C $(DOC_DIR) $@ $(exports); then exit; fi

install_all install: install_doc
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi

install_beta:
	$(hide) if ! $(MAKE) -C $(SRC_DIR) install_all $(beta_exports); then exit; fi

install_default install_debug: 
	$(hide) if ! $(MAKE) -C $(SRC_DIR) $@ $(exports); then exit; fi





#
# $Log$
# Revision 1.1  1998/10/23 13:15:45  houghton
# Initial Version.
#
#

# Local Variables:
# mode:makefile
# End:
