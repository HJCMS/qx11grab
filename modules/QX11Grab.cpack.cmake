##############################################################
## CPack
##############################################################
SET (_package_architecture "i386")
FIND_PROGRAM (UNAME_EXECUTABLE NAMES uname)
IF (UNAME_EXECUTABLE)
  EXECUTE_PROCESS (COMMAND ${UNAME_EXECUTABLE} -m
    OUTPUT_VARIABLE _package_architecture
    OUTPUT_STRIP_TRAILING_WHITESPACE)
ENDIF (UNAME_EXECUTABLE)

##############################################################
## CPack Defaults
##############################################################
SET (CPACK_PACKAGE_NAME "qx11grab")
SET (CPACK_PACKAGE_VENDOR "HJCMS")
SET (CPACK_PACKAGE_VERSION_MAJOR ${QX11GRAB_VERSION_MAJOR})
SET (CPACK_PACKAGE_VERSION_MINOR ${QX11GRAB_VERSION_MINOR})
SET (CPACK_PACKAGE_VERSION_PATCH ${QX11GRAB_VERSION_RELEASE})
SET (CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET (CPACK_SET_DESTDIR ON)
SET (CPACK_PACKAGE_RELOCATABLE OFF)
SET (CPACK_COMPONENTS_ALL "core;data;translation;plugins;ffpreset")
SET (CPACK_PACKAGE_EXECUTABLES "qx11grab")
SET (CPACK_STRIP_FILES "${CPACK_PACKAGE_EXECUTABLES}")
SET (CPACK_TOPLEVEL_TAG "cpack.tmp")
SET (CPACK_PACKAGE_FILE_NAME "qx11grab-${QX11GRAB_VERSION}")
SET (CPACK_SOURCE_PACKAGE_FILE_NAME "qx11grab-${QX11GRAB_VERSION}_orig")
SET (CPACK_PACKAGE_DESCRIPTION_SUMMARY "a high flexible screencast application for X11 desktop")

##############################################################
## CPack RPM
##############################################################
SET (CPACK_RPM_PACKAGE_LICENSE "GPLv2+")
SET (CPACK_RPM_PACKAGE_GROUP "Productivity/Multimedia/Other")
SET (CPACK_RPM_PACKAGE_URL "http://qx11grab.hjcms.de")
SET (CPACK_RPM_PACKAGE_DESCRIPTION "qt4 applet for recording x11 windows with ffmpeg this prgramm requires ffmpeg from http://ffmpeg.mplayerhq.hu")
SET (CPACK_RPM_PACKAGE_VENDOR "Juergen Heinemann (Undefined) http://www.hjcms.de")
SET (CPACK_RPM_COMPRESSION_TYPE "xz")
SET (CPACK_RPM_PACKAGE_REQUIRES "ffmpeg")
SET (CPACK_RPM_PACKAGE_ARCHITECTURE "${_package_architecture}")

##############################################################
## CPack RPM
##############################################################
SET (CPACK_DEBIAN_PACKAGE_NAME "qx11grab")
SET (CPACK_DEBIAN_PACKAGE_VERSION "${QX11GRAB_VERSION_MAJOR}.${QX11GRAB_VERSION_MINOR}.${QX11GRAB_VERSION_RELEASE}")
IF ("${_package_architecture}" MATCHES "i*86")
  SET (CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
ELSE ("${_package_architecture}" MATCHES "i*86")
  SET (CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${_package_architecture}")
ENDIF ("${_package_architecture}" MATCHES "i*86")
SET (CPACK_DEBIAN_PACKAGE_DEPENDS "cmake libqt4-dev (>= 7.4.2) debhelper (>= 7.3.16) libx11-dev libasound2-dev libpulse-dev libavformat-dev libavcodec-dev libavutil-dev libavfilter-dev libx264-dev libtheora-dev libvpx-dev libxrandr-dev libfreetype6-dev libfontconfig1-dev ffmpeg")
SET (CPACK_DEBIAN_PACKAGE_MAINTAINER "Juergen Heinemann (Undefined) <noreply@hjcms.de>")
SET (CPACK_DEBIAN_PACKAGE_SECTION "kde")
SET (CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET (CPACK_DEBIAN_PACKAGE_HOMEPAGE "http://qx11grab.hjcms.de/")
SET (CPACK_DEBIAN_PACKAGE_DESCRIPTION "systray program that uses FFmpeg for recording X11 desktop windows
This program grabs window dimensions with xevent and prepares a command line
for ffmpeg with configuration options. It sends the generated parameter list
to a new FFmpeg process.
.
qx11grab doesn't record itself, it just prepare commands for FFmpeg.")
SET (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

##EOF
