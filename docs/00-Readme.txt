#
#
# Title:        00-Readme.txt
# Project:	Traffic
# Desc:
# 
#   
# 
# Notes:
# 
# Author:	Paul A. Houghton - (paul.houghton@wcom.com)
# Created:	05/12/97 09:32
#
# Revision History: (See end of file for Revision Log)
#
#   Last Mod By:    $Author$
#   Last Mod:	    $Date$
#   Version:	    $Revision$
#
#   $Id$
# 


Howdy,

Hopefully, this collection of headers and expamples will give you a
general feel for how the Mapped Memory classes I've developed can be
used. The classes are very flexable and not well documented. They are
all based on using Unix Mapped Memory for data storage and retieval.

The two examples I included should help you understand some of the
posibilities available.

Here is a short explination of the files included:

AvlTreeBase.hh

    This is a base class for balanced binary AVL trees.
    
AvlTreeOffsetBase.hh:

    AvlTreeOffsetBase : public AvlTreeBase

    This is the base class for AvlTreeOffset.

AvlTreeOffset.hh:

    AvlTreeOffset : public AvlTreeOffsetBase
    
    This template class provides an Avl Tree that
    does not use any memory addresses. All links are accomplished with
    offsets.

MapFile.hh:

    This class provides a basic interaface to Unix's mapped memory
    functions. It uses the mmap and munmap system calls.

MapMem.hh

    MapMem : public MapFile.

    This is the base class for both the MapMemFixedDynamic and
    MapMemDynamciDynamic classes.

MapMemDynamciDynamci.hh

    MapMemDynamciDynamc : public MapMem

    This class provides a managed mapped memory region that dyanmic
    sized chunks can be allocated and deallocated from.

MapMemFixedDynamic.hh

    MapMemFixedDynamic : public MapMem

    This class provides a managed mapped memory region that fixed
    sized chunks can be allocated and deallocated from.

MultiMem.hh
MultiMemOffset.hh
MultiMemOffsetMapFixed.hh

    These classes are used by the AvlTreeOffset class to access the
    MapMemFixedDynamic class.


MultiKeyData.C

    This is an incomplete example that uses AvlTreeOffset and
    MapMemDynamicDynamic to create a multi key'd data set.

StaticFixed.C

    This incomplete example shows how MapFile can be used directly to
    create a fixed data set that can be accessed as an array.

src/

    This directory contains the primary sources for the Mapped Memory
    classes.

examples/

    This direcory contains examples of AvlTreeOffset & MapMem usage from other
    projects. 
    
    
    
#
# $Log$
# Revision 1.1  1997/06/05 11:28:50  houghton
# Initial Version.
#
#

# Local Variables:
# mode:indented-text
# End:
