# 2001-06-04 (cc) <paul4hough@gmail.com>
#

lib_LTLIBRARIES = libmdb.la
libmdb_la_CPPFLAGS = -I..

include_HEADERS = \
MapFile.hpp \
MapBatch.hpp \
MapMem.hpp \
MapMemDynamic.hpp \
MapMemDynamicDynamic.hpp \
MapMemDynamicFixed.hpp \
MultiMem.hpp \
MultiMemOffset.hpp \
MultiMemOffsetMapDynamic.hpp \
MultiMemOffsetMapFixed.hpp \
RBTreeBase.hpp \
RBTree.hpp \
RBSet.hpp \
DRBTree.hpp \
DRBSet.hpp \
HashTableBase.hpp \
DashTableBase.hpp \
HashTable.hpp \
HashSet.hpp \
DashTable.hpp \
DashSet.hpp \
ReconTableBase.hpp \
ReconTable.hpp

libmdb_la_SOURCES = \
MapFile.cpp \
MapMem.cpp \
MapMemDynamic.cpp \
MapMemDynamicDynamic.cpp \
MapMemDynamicFixed.cpp \
MultiMem.cpp \
MultiMemOffset.cpp \
MultiMemOffsetMapDynamic.cpp \
MultiMemOffsetMapFixed.cpp \
RBTreeBase.cpp \
HashTableBase.cpp \
DashTableBase.cpp \
ReconTableBase.cpp


test: all
	$(MAKE) -C ../tests/unit test
