dnl configure.in
dnl -----------------------------------------------------------------------
dnl Author: Juergen Heinemann http://www.hjcms.de, (C) 2007-2009         dnl
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
m4_define(PKG_MINOR, [1])
m4_define(PKG_MICRO, [8])

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
dnl Checks for required programs.
dnl -----------------------------------------------
AC_LANG_CPLUSPLUS
AC_PROG_CPP
AC_PROG_CXX([g++ mpicxx])
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
dnl locale's files and scripts
dnl -----------------------------------------------
dnl AM_GNU_GETTEXT([external])
dnl AM_GNU_GETTEXT_VERSION(0.16)
dnl AC_LIB_LINKFLAGS([asprintf])
dnl AC_SUBST([LOCALEDIR],"$localedir") dnl

dnl -----------------------------------------------
dnl Checks for required headers
dnl -----------------------------------------------
AC_HEADER_STDC
AC_CHECK_HEADERS([stddef.h stdio.h time.h stdarg.h])

dnl -----------------------------------------------
dnl Checks for required ffmpeg Development Files
dnl -----------------------------------------------
dnl libavformat
PKG_CHECK_MODULES([AVFORMAT],[libavformat >= 52.41.0],
  AC_DEFINE(HAVE_AVFORMAT,[1],[libavformat exists]),
  AC_MSG_ERROR([libavformat >= 52.41.0 from http://www.ffmpeg.org not found!])
)

dnl libavutil
PKG_CHECK_MODULES([AVUTIL],[libavutil >= 50.6.0],
  AC_DEFINE(HAVE_AVUTIL,[1],[libavutil exists]),
  AC_MSG_ERROR([libavutil >= 50.6.0 from http://www.ffmpeg.org not found!])
)

dnl libavcodec
PKG_CHECK_MODULES([AVCODEC],[libavcodec >= 52.43.0],
  AC_DEFINE(HAVE_AVCODEC,[1],[libavcodec exists]),
  AC_MSG_ERROR([libavcodec >= 52.43.0 from http://www.ffmpeg.org not found!])
)

dnl -----------------------------------------------
dnl get Default Install Prefix from pkg-config
dnl -----------------------------------------------
AC_PREFIX_PROGRAM([pkg-config])

dnl -----------------------------------------------
dnl NOTICE
dnl -----------------------------------------------
AC_MSG_RESULT([Installation Directory is \$(DESTDIR)$prefix])

dnl -----------------------------------------------
dnl Generates configuration files and scripts
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
echo "or 'make DESTDIR=/tmp install' for Installation Tests. "
echo

dnl -----------------------------------------------
dnl eof
