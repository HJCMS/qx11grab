prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=$prefix
libdir=$prefix/lib@LIB_SUFFIX@
datadir=$prefix/share/qx11grab
includedir=$prefix/include

Name: qx11grab
Description: The QX11Grab Plugin Interface Library
Url: http://qx11grab.hjcms.de
Version: @QX11GRAB_VERSION_MAJOR@.@QX11GRAB_VERSION_MINOR@.@QX11GRAB_VERSION_RELEASE@
Requires: QtGui >= 4.7.3 QtCore >= 4.7.3
Libs: -L$libdir -l@INCLUDE_NAME_DIR@
Libs.private: 
Cflags: -I$includedir/@INCLUDE_NAME_DIR@
