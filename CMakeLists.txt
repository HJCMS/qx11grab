# CMakeLists.txt
##############################################################
PROJECT (qx11grab)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0 FATAL_ERROR)

##############################################################
# CMP0002: we have multiple targets with the same name for the unit tests
##############################################################
CMAKE_POLICY (VERSION 2.8)

##############################################################
# where to look first for cmake modules, before ${CMAKE_ROOT}/cmake/Modules/ is checked
##############################################################
SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

##############################################################
## Requiered "QtCore QtGui QtXml QtNetwork"
##############################################################
SET (QT_MIN_VERSION 4.6.0)
FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Qt4 COMPONENTS QTDBUS)
FIND_PACKAGE (PkgConfig REQUIRED)
FIND_PACKAGE (ALSA REQUIRED)

IF (NOT UNIX)
  MESSAGE (FATAL_ERROR "Only Unix is Supported!")
ENDIF (NOT UNIX)

##############################################################
## Set qx11grab Specific Information
##############################################################

SET (QX11GRAB_VERSION_MAJOR 0)
SET (QX11GRAB_VERSION_MINOR 2)
SET (QX11GRAB_VERSION_RELEASE 2)
SET (QX11GRAB_VERSION "${QX11GRAB_VERSION_MAJOR}.${QX11GRAB_VERSION_MINOR}.${QX11GRAB_VERSION_RELEASE}")
SET (QX11GRAB_DISTRIBUTION_TEXT "compiled sources" CACHE STRING "Indicate the distribution in bug reports")
SET (LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)")
SET (QX11GRAB_CXXFLAGS "" CACHE STRING "Defined in pkg-config file")
SET (QX11GRAB_REQUIRES "" CACHE STRING "Defined in pkg-config file")
SET (QX11GRAB_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/qx11grab.qrc")
SET (CMAKE_INSTALL_RPATH_USE_LINK_PATH  TRUE)

EXEC_PROGRAM (${QT_QMAKE_EXECUTABLE}
  ARGS "-query QT_INSTALL_PREFIX"
  OUTPUT_VARIABLE CMAKE_INSTALL_PREFIX
)

##############################################################
## Set Header Definitions
##############################################################

INCLUDE (CheckCXXCompilerFlag)
INCLUDE (MacroEnsureVersion)


##############################################################
## gcc g++ ld
##############################################################

IF (CMAKE_COMPILER_IS_GNUCXX)

  INCLUDE (CheckingGNUCXX)

  IF (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    SET (TWEAKING_LDFLAGS "-Wl,--as-needed")
  ENDIF(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")

ENDIF (CMAKE_COMPILER_IS_GNUCXX)

##############################################################
## APPEND|REMOVE DEFINITIONS
##############################################################

REMOVE_DEFINITIONS (-DQT3_SUPPORT_WARNINGS -DQT3_SUPPORT)

##############################################################
## SET Working output Directory
##############################################################

SET (ARCHIVE_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app
)

SET (EXECUTABLE_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app
)

SET (LIBRARY_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app
)

##############################################################
## FFmpeg avformat
##############################################################

PKG_CHECK_MODULES (FFMPEG REQUIRED libavformat>=52.38.0 libavcodec>=52.35.0)

##############################################################
## Configure files
##############################################################

CONFIGURE_FILE (src/version.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/src/version.h
)

CONFIGURE_FILE (qx11grab.conf.cmake
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.conf
)

##############################################################
## INCLUDES
##############################################################

## QT_HEADERS_DIR
SET (QX11GRAB_INCLUDES
  ${QT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${FFMPEG_INCLUDEDIR}
  ${ALSA_INCLUDE_DIR}
)

##############################################################
## LINKER PATHS
##############################################################

SET (QX11GRAB_LIBRARIES
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${FFMPEG_LDFLAGS}
  ${ALSA_LIBRARY}
)

##############################################################
## SUBDIRECTORIES
##############################################################
ADD_SUBDIRECTORY (src)
ADD_SUBDIRECTORY (i18n)

##############################################################
## LGPL/GPL3 Exceptions
##############################################################

INSTALL (FILES
  NEWS
  README
  AUTHORS
  ChangeLog
  COPYING
  DESTINATION share/qx11grab
)

INSTALL (FILES
  qx11grab.desktop
  DESTINATION share/applications
)

INSTALL (FILES
  src/images/qx11grab.png
  DESTINATION share/pixmaps
)

INSTALL (FILES
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.conf
  DESTINATION /etc/xdg/hjcms.de/
)

## EOF
