#
#
# Title:        Makefile
# Project:	Mdb %PP%
# Item:   	%PI% (%PF%)
# Desc:
# 
#   Makefile for User Documentation
# 
# Notes:
#
#   This Makefile requres MakeConfig and GNU's make version 3.75
#
#   See Detail Documentation the end of the file for more information
#
#   Try 'make help' for a list of standard targets.
# 
# Author:	Paul Houghton 719-527-7834 (paul.houghton@wcom.com)
# Created:	2/14/101 useing MakeConfigs 6.01.01
#
# Revision History: (See end of file for Revision Log)
#
#   Last Mod By:    %PO%
#   Last Mod:	    %PRT%
#   Version:	    %PIV%
#   Status:	    %PS%
#
#   %PID%
# 

#
# The following variables control the behavior of MakeConfigs.
#   makefile_type   is the type of object being build (do NOT change)
#   make_cfg_ver    is the version of MakeConfigs to use
#   make_target	    is the default target to build
#   show_commands   if this is true, the commands  will be output
#   check_install   if this is true, install and install_all will NOT
#		    overwrite an installed version.
#

makefile_type   = doc
make_target	= html
make_cfg_ver	= 6.01
show_commands 	= # true
check_install	= true
force		= # true

PRJ_TOPDIR  	= ../../..
config_dir  	= $(PRJ_TOPDIR)/src/config

PROJECT		= Mdb

include $(config_dir)/00-Makefile.cfg
include Make/make.cfg.$(make_cfg_ver)

# 
# =USE_CURRENT_MAKEFILE=
# When you re-gen a make file, every thing between 
# these tags will be kept in the new makefile
# if you want it re-gen'd, just delete the tags
#

# DOC_SRC_SUBDIR	= docs/src
# DOC_HTML_SUBDIR	= docs/html
# DOC_MAN_SUBDIR	= docs/man


# XALAN_DEBUG	= true

# JAVA_SRC_BASE_DIR	= $(PRJ_TOPDIR)/src/java
# JAVADOC_API_DIR	= JavaApi


#
# =USE_CURRENT_MAKEFILE=
#

# **** S U B D I R  T A R G E T S ****

SUBDIR_TARGETS		= default all html man

# ****H E L P   V A R I A B L E S ****

HELP_CONFIG_VARS	=	\
	XALAN_DEBUG

HELP_TARGETS		= $(SUBDIR_TARGETS)

# **** P H O N Y   T A R G E T S

PHONY_TARGETS		= $(HELP_TARGETS)

#
# =USE_CURRENT_MAKEFILE=
#

# *** DOC FILES ***

SUBDIRS		=

#
# Uncomment this to generate the SupportTools.xml document
# from the Setup.cfg file
#
# SUPPORT_TOOLS_XML	= SupportTools.xml

SUPPORT_MAKEFILE_TXT	= Support.Makefile.txt

XML_MAN_1_SOURCES	=

XML_MAN_3_SOURCES	=

XML_MAN_5_SOURCES	=

XML_HTML_SOURCES	=	\
	$(XML_MAN_1_SOURCES)	\
	$(XML_MAN_3_SOURCES)	\
	$(XML_MAN_5_SOURCES)	\
	00-Readme.xml		\
	Install.xml		\
	Prerequisites.xml	\
	StandaloneInstall.xml	\
	SupportInstall.xml

POD_MAN_1_SOURCES	=

POD_MAN_3_SOURCES	=

POD_MAN_5_SOURCES	=

POD_HTML_SOURCES	=	\
	$(POD_MAN_1_SOURCES)	\
	$(POD_MAN_3_SOURCES)	\
	$(POD_MAN_5_SOURCES)

HTML_SOURCES	=

OTHER_SOURCES	=		\
	Support.Makefile

DOCS_OTHER	=  $(OTHER_SOURCES:%=$(doc_html_dir)/%)

# **** T A R G E T S * * * *

include Make/make.cfg.targets.doc.$(make_cfg_ver)

$(doc_html_dir)/%.Makefile: %.Makefile $(force_target)
	$(hide) $(install_cmd) $< $@

$(SUBDIR_TARGETS)::
	$(call make_subdirs,$@,$(SUBDIRS),$($(@)_exports) $(exports))

html:: $(docs_html_targets)

man:: $(docs_man_targets)

default:: html

all:: html man

clean::
	rm -f $(docs_all_html) $(docs_all_man)
	rm -f core

realclean:: clean
	rm -f *~


# Detail Documentation
#
# General Information
#
#	Do not be daunted by the number of variables and targets. The
#	MakeConfigs system has a tremendous amount of functionality
#	built into it. All you really need to do is list your
#	document sources in the appropriate section and build the
#	targets (default or html). But, if you have to perform actions
#	that are beyond the default targets, these targets and
#	variables are available for your use.
#
#	This Makefile is designed to work within a directory structure
#	that separates the documentation sources from the generated
#	output. For example, let say you place all of your
#	documentation under the directory `$(PRJ_TOPDIR)/docs'. The
#	first thing you should do is create a subdirectory for your
#	document sources, the default is `$(PRJ_TOPDIR)/docs/src', but
#	you can name it whatever you like, you'll just have to set the
#	`DOC_SRC_SUBDIR' variable to the correct value.
#
#	Once you have a documentation source subdirectory, you start
#	creating your documents in that directory and any
#	subdirectories below it. When you build the HTML (i.e. make
#	html), the generated output will be placed in
#	`$(PRJ_TOPDIR)/docs/html' (note this is a default value you
#	can change with the DOC_HTML_SUBDIR variable.
#
#	The makefile targets create the output directories as
#	needed. If you have a subdirectory under your
#	$(DOC_SRC_SUBDIR) that contains documentation files, a
#	subdirectory under `$(DOCS_HTML_SUBDIR) will be created and
#	the generated output will be placed in it.
#
#	To illustrate: (Note index.html is generated from 00-Readme.xml)
#
#	    $(PRJ_TOPTDIR)/docs/src
#		    00-Readme.xml
#		    ReleaseNotes.xml
#
#	    $(PRJ_TOPTDIR)/docs/src/User
#		    00-Readme.xml
#		    Install.xml
#		    MyApp.xml
#
#	    $(PRJ_TOPDIR)/docs/src/User/Headers
#		    00-Readme.xml
#		    MyClassHeader.xml
#
#	Will Generate the following HTML files:
#
#	    $(PRJ_TOPTDIR)/docs/html
#		    index.html
#		    ReleaseNotes.xml
#
#	    $(PRJ_TOPTDIR)/docs/html/User
#		    index.html
#		    Install.xml
#		    MyApp.xml
#
#	    $(PRJ_TOPDIR)/docs/html/User/Headers
#		    index.html
#		    MyClassHeader.xml
#
#	The same idea is true for Unix Man pages, only the destination
#	directory is `$(PRJ_TOPDIR)/docs/man' by default and can be
#	changed with the `DOC_MAN_SUBDIR' variable.
#
#	NOTE: as of this time (3/2/01) Unix Man Page support has not
#	been completed. Attempts to build the `man' target will
#	echo a message stating it is not yet available, but it will
#	not fail. If you place your documentation sources under the
#	correct variables, when `man' support becomes available, you
#	should not have to change your Makefile in any way.
#
# Control Variables
#
#   show_commands   if this is true, the commands executed during the
#		    build will be output. Normally these commands are
#		    hidden and the only thing output is short messages
#		    indicating the items being built
#
#   make_target	    is the default target to build
#
#   check_install   if this is true, install and install_all will NOT
#		    overwrite an installed version.
#
#   force	    If this is not empty, force the rebuild of all
#		    targets even if none of the dependencies are out
#		    of date.
#
#
# Target Variables
#
#   PRJ_TOPDIR	    This is the project's top directory (i.e. all
#		    the projects files are within subdirectories
#		    under this directory). The Generated Makefile
#		    contains a PRJ_TOPDIR variable the points to
#		    this directory by determining how many
#		    directory levels up the current directory is
#		    from the `topdir' and using that number of
#		    `..' sets. Fore example, if the `topdir' is
#		    /prj/libCool and the current directory is
#		    /prj/libCool/src/Cool, the PRJ_TOPDIR would be
#		    set to `../..' (i.e this directory is 2
#		    levels down from the `topdir'.
#
#   SUBDIR_TARGETS  These are the targets that will be built in the
#		    subdirectories listed in SUBDIR.
#
#   standard_targets
#		    This is a list of the standard targets. It is set in
#		    the make.cfg file to include the following targets:
#
#			all
#			default
#			check
#			install
#			install_all
#			clean
#			realclean
#
#
# XSLT Variables
#
#	These variables control the conversion of XML to HTML
#
#   XALAN_DEBUG	    If this is not empty all xalan debug flags will be
#                   passed to the converter.
#
#   XSL_DIR	    If this is not empty, it will be used as the
#		    xalan xsl directory. If it is empty, the default
#		    is: $(TOOL_DIR)/share/xsl
#
#   XSL_HTML_FILE   If this is not empty, it will be used as the
#		    default XSLT conversion stylesheet. If it is
#		    empty, the default
#		    $(TOOL_DIR)/share/xsl/wcom/html/doctools.xsl
#		    will be used.
#
# Source Variables
#
#   SUBDIRS		This should contain the list of subdirectorys that
#			should be build.
#
#   XML_MAN_1_SOURCES	Should contain the XML document source files that
#			document programs.
#
#   XML_MAN_3_SOURCES	Should contain the XML document source files
#			that document classes and functions.
#
#   XML_MAN_5_SOURCES	Should contain the XML document source files
#			that document file formats.
#
#   XML_HTML_SOURCES	Should contain all the XML document source to
#			be used to generate HTML documentation.
#
#   POD_MAN_1_SOURCES	Should contain the POD document source files that
#			document programs.
#
#   POD_MAN_3_SOURCES	Should contain the POD document source files
#			that document classes and functions.
#
#   POD_MAN_5_SOURCES	Should contain the POD document source files
#			that document file formats.
#
#   POD_HTML_SOURCES	Should contain all the POD document source to
#			be used to generate HTML documentation.
#
#   HTML_SOURCES	Should contain any HTML documents to be copied
#			into the generated html documentation directory.
#
# Destination Variables
#
#	See General Information above more additional details about
#	these variables.
#
#   DOC_SRC_SUBDIR	The top level subdirectory under $(PRJ_TOPDIR)
#			that contains documentation source files (the
#			default is `docs/src').
#
#   DOC_HTML_SUBDIR	The top level subdirectory under $(PRJ_TOPDIR)
#			that generated HTML files should be written to
#			(the default is `docs/src').
#
#   DOC_MAN_SUBDIR	The top level subdirectory under $(PRJ_TOPDIR)
#			that generated man pages should be written to
#			(the default is `docs/src').
#
# Help variables
#
#   HELP_TARGETS	Add any targets you create that should be
#			listed when a user performs a `make help'.
#
#   HELP_CONFIG_VARS	If you are using a standard MakeConfigs
#			Setup.cfg file, any variables listed here will
#			be shown when a `make help_config' is
#			performed.
#
# Target Variables
#
#   PHONY_TARGETS	All list of the phony targets (i.e. not real
#			files) that you have added to this makefile
#			which should be appended to the .PHONY:
#			target. For more information, see make(info).
#	
# Targets
#
#   html:	    Build all the HTML files from the
#		    XML_HTML_SOURCES, POD_HTML_SOURCES and
#		    HTML_SOURCES variables. It also perform a `make
#		    html' in all the subdirectories listed in the
#		    SUBDIRS variable.
#
#   man:	    Build all the man pages from the
#		    XML_MAN_1_SOURCES, XML_MAN_3_SOURCES,
#		    XML_MAN_5_SOURCES, POD_MAN_1_SOURCES,
#		    POD_MAN_3_SOURCES and POD_MAN_5_SOURCES
#		    variables. It also perform a `make html' in all
#		    the subdirectories listed in the SUBDIRS variable.
#
#   default:	    build html target
#
#   all:	    build both the html and man targets.
#
# User Defined Functions
#
#   $(call xml2html,src,dest[,xslfile])
#
#	Use xalan to convert the xml `src' file to an HTML `dest' file
#	using the default xsl file or `xslfile' if specified. 
#
#

#
#
# %PL%
#
#

# Local Variables:
# mode:makefile
# End:
