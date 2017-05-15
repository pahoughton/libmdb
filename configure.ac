dnl configure.ac - 2017-05-11
dnl
dnl (cc) <paul4hough@gmail.com>

AC_PREREQ(2.69)
AC_INIT( libmdb,
	 1.01.01,
	 Paul Houghton <paul4hough@gmail.com> )
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
LT_INIT

AC_PROG_CXX
AC_LANG([C++])
dnl AC_CONFIG_HEADERS([config.h])
AC_CHECK_HEADERS(valid/verify.hpp,[],[echo ERROR valid.hpp required; exit 1;])
AC_CHECK_HEADERS(clue/Clue.hpp,[],[echo ERROR Clue.hpp required; exit 1;])
AC_CHECK_HEADERS([iostream])

AC_CONFIG_FILES([Makefile tests/unit/Makefile mdb/Makefile])
AC_OUTPUT
