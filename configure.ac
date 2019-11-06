dnl Process this file with autoconf to produce a configure script.

dnl -----------------------
dnl Autoconf initialization
dnl -----------------------
AC_INIT([coyotetm], [0.8],[igaldino@gmail.com])
AC_LANG_CPLUSPLUS

dnl -----------------------
dnl Automake initialization
dnl -----------------------
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC

dnl ------------
dnl C++ settings
dnl ------------
AC_PROG_CXX
AC_PROG_CXXCPP

dnl ---------------------------------------
dnl Check for Berkeley DB headers/libraries
dnl ---------------------------------------
AC_CHECK_HEADER(db_cxx.h,,AC_MSG_ERROR([Could not find db_cxx.h include file.]))
AC_CHECK_LIB(db_cxx,main,,AC_MSG_ERROR([Could not link with db_cxx library.]),$LDFLAGS $LIBS)

dnl --------------------------------
dnl Check for FLTK headers/libraries
dnl --------------------------------
AC_PATH_PROG(FLTKCONFIG,fltk-config)
if test "x$FLTKCONFIG" = x
then
   AC_CHECK_HEADER(FL/Fl.H,,AC_MSG_ERROR([Could not find FLTK include files.]))
   AC_CHECK_LIB(fltk,main,,AC_MSG_ERROR([Could not link with FLTK library.]),$LDFLAGS $LIBS)
else
   CFLAGS="`$FLTKCONFIG --cflags` $CFLAGS"
   CXXFLAGS="`$FLTKCONFIG --cflags` $CXXFLAGS"
   LIBS="`$FLTKCONFIG --ldflags` $LIBS"
fi

dnl -------------------------------------------------------------------
dnl Enable maintainer mode, if configured with --enable-maintainer-mode
dnl -------------------------------------------------------------------
AM_MAINTAINER_MODE

dnl ------------------
dnl Write output files
dnl ------------------
AC_OUTPUT(Makefile)

