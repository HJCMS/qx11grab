dnl configure.in
dnl -----------------------------------------------------------------------
dnl Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2010         dnl
dnl                                                                      dnl
dnl Copyright (C) 2004 by Juergen Heinemann                              dnl
dnl                                     nospam __AT__ hjcms (DOT) de     dnl
dnl                                                                      dnl
dnl This program is free software; you can redistribute it and/or modify dnl
dnl it under the terms of the GNU General Public License as published by dnl
dnl the Free Software Foundation; either version 2 of the License, or    dnl
dnl (at your option) any later version.                                  dnl
dnl                                                                      dnl
dnl This program is distributed in the hope that it will be useful,      dnl
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of       dnl
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        dnl
dnl GNU General Public License for more details.                         dnl
dnl                                                                      dnl
dnl You should have received a copy of the GNU General Public License    dnl
dnl along with this program; if not, write to the                        dnl
dnl Free Software Foundation, Inc.,                                      dnl
dnl 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.            dnl
dnl ------------------------------------------------------------------------
AC_PREREQ(2.62)

dnl -----------------------------------------------
dnl Release Versioning
dnl -----------------------------------------------
m4_define(PKG_NAME, [qx11grab])
m4_define(PKG_MAJOR, [0])
m4_define(PKG_MINOR, [2])
m4_define(PKG_MICRO, [1])

dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------
AC_INIT(PKG_NAME, PKG_MAJOR.PKG_MINOR.PKG_MICRO, nospam (AT) hjcms (DOT) de)

dnl -----------------------------------------------
dnl Set options and macros
dnl -----------------------------------------------
AC_CONFIG_AUX_DIR([auxdir])
AC_REQUIRE_AUX_FILE([missing])
AC_CONFIG_MACRO_DIR([m4])

AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE([enable])
dnl Enable support for silent build rules
AM_SILENT_RULES([yes])
dnl Enable support for strip symbols
AM_PROG_INSTALL_STRIP
dnl Enable regular expression library
AM_WITH_REGEX

dnl -----------------------------------------------
dnl Check for required applications
dnl -----------------------------------------------
AC_LANG_CPLUSPLUS
AC_PROG_CPP
AC_PROG_CXX([g++ mpic++ mpicxx])
AC_PROG_CXX_C_O
AC_PROG_CC_C99
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_SED
AC_PROG_GREP
AC_GNU_SOURCE
AC_PATH_X

dnl -----------------------------------------------
dnl QT4 >= 4.6.0 : http://www.trolltech.com
dnl AC_CHECK_QT4([min version],[Required Libraries])
dnl -----------------------------------------------
AC_CHECK_QT4(["4.6.0"],["QtCore QtGui QtDBus"])

dnl -----------------------------------------------
dnl Check for required headers
dnl -----------------------------------------------
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdio.h time.h stdarg.h])

dnl -----------------------------------------------
dnl Check for required ffmpeg Development Files
dnl -----------------------------------------------
dnl libavformat,libavutil,libavcodec
AC_ARG_VAR([FF_SUFFIX],[optional you can set an suffix for ffmpeg libraries])
AC_SUBST([FF_SUFFIX])

dnl PKG_CHECK_EXISTS([libavformat >= 52.41.0 libavutil >= 50.6.0 libavcodec >= 52.43.0],[
dnl   AC_DEFINE(HAVE_FFMPEG,[1],[libavformat, libavutil an libavcodec exists])
dnl   AC_SUBST([FFMPEG_LIBS],"-lavformat$FF_SUFFIX -lavutil$FF_SUFFIX -lavcodec$FF_SUFFIX")
dnl   ],[
dnl   AC_MSG_ERROR([This Application need:
dnl libavformat >= 52.41.0, libavutil >= 50.6.0, libavcodec >= 52.43.0 from http://www.ffmpeg.org 
dnl One of this Required Version Libraries not found!])
dnl ])

dnl -----------------------------------------------
dnl get Default Install Prefix from pkg-config
dnl -----------------------------------------------
AC_PREFIX_PROGRAM([pkg-config])

dnl -----------------------------------------------
dnl Result
dnl -----------------------------------------------
AC_MSG_RESULT([Installation Directory is \$(DESTDIR)$prefix])

dnl -----------------------------------------------
dnl Generate configuration files and scripts
dnl -----------------------------------------------
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([auxdir/Makefile])
AC_CONFIG_FILES([m4/Makefile])
AC_CONFIG_FILES([tr/Makefile])
AC_CONFIG_FILES([src/images/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([extras/Makefile])
AC_CONFIG_FILES([src/version.h])
AC_CONFIG_FILES([qx11grab.spec])
AC_OUTPUT

echo
echo -n " Now type 'make', followed by 'make install' as root "
echo "or 'make DESTDIR=/tmp install' for test Installation. "
echo

dnl -----------------------------------------------
dnl eof
