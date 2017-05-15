# 2017-04-19 (cc) <paul4hough@gmail.com>
#

check_PROGRAMS = unit
unit_LDADD = $(top_srcdir)/mdb/libmdb.la -lclue
unit_CPPFLAGS =	-I$(top_srcdir)

unit_SOURCES =					\
	main.cpp				\
	v_MapFile01.cpp				\
	v_MapFile02.cpp				\
	v_MapFile03.cpp				\
	v_MapBatch01.cpp			\
	v_MapMemDynamicDynamic01.cpp		\
	v_MapMemDynamicDynamic02.cpp		\
	v_MapMemDynamicFixed01.cpp		\
	v_RBTree01.cpp				\
	v_RBTree02.cpp				\
	v_RBTree03.cpp				\
	v_RBSet.cpp				\
	v_DRBTree01.cpp				\
	v_DRBTree02.cpp				\
	v_DRBTree03.cpp				\
	v_DRBTree04.cpp				\
	v_DashSet01.cpp				\
	v_HashSet01.cpp


.PHONY: validate
test: unit
	LD_LIBRARY_PATH=$(prefix)/lib ./unit

dbg: unit
	echo LD_LIBRARY_PATH=$(prefix)/lib libtool --mode=execute gdb unit
