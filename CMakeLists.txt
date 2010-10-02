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
SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

##############################################################
## Requiered "QtCore QtGui QtXml QtNetwork"
##############################################################
SET (QT_MIN_VERSION 4.6.0)
FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Qt4 COMPONENTS QTDBUS)
FIND_PACKAGE (PkgConfig REQUIRED)
FIND_PACKAGE (ALSA REQUIRED)

## Disable KDE4 Framework Support
OPTION (DISABLE_KDE_SUPPORT "Enable KDE4 Environment (ON/OFF)" OFF)
IF (NOT DISABLE_KDE_SUPPORT)
  FIND_PACKAGE (KDE4 4.5.0)
ENDIF (NOT DISABLE_KDE_SUPPORT)

IF (NOT UNIX)
  MESSAGE (FATAL_ERROR "Only Unix is Supported!")
ENDIF (NOT UNIX)

##############################################################
## Set qx11grab Specific Information
##############################################################

SET (QX11GRAB_VERSION_MAJOR 0)
SET (QX11GRAB_VERSION_MINOR 2)
SET (QX11GRAB_VERSION_RELEASE 3)
SET (QX11GRAB_VERSION "${QX11GRAB_VERSION_MAJOR}.${QX11GRAB_VERSION_MINOR}.${QX11GRAB_VERSION_RELEASE}")
SET (QX11GRAB_DISTRIBUTION_TEXT "compiled sources" CACHE STRING "Indicate the distribution in bug reports")
SET (QX11GRAB_RESOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/qx11grab.qrc")

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
## FFmpeg 
##############################################################

SET (FFMPEG_SUFFIX "" CACHE STRING
  "ffmpeg pkg-config File Suffix (STRING)" FORCE
)

SET (FFMPEG_AVFORMAT_VERSION "52.38.0" CACHE STRING
  "overwrite required libavformat Version (STRING)" FORCE
)

SET (FFMPEG_AVCODEC_VERSION "52.35.0" CACHE STRING
  "overwrite required libavcodec Version (STRING)" FORCE
)

PKG_CHECK_MODULES (FFMPEG REQUIRED 
  libavformat${FFMPEG_SUFFIX}>=${FFMPEG_AVFORMAT_VERSION}
  libavcodec${FFMPEG_SUFFIX}>=${FFMPEG_AVCODEC_VERSION}
)

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
## KDE4 Framework Support
##############################################################

IF (KDE4_FOUND)
  SET (QX11GRAB_INCLUDES
    ${QX11GRAB_INCLUDES}
    ${KDE4_INCLUDE_DIR}
  )
  SET (QX11GRAB_LIBRARIES
    ${QX11GRAB_LIBRARIES}
    ${KDE4_KDECORE_LIBRARY}
    ${KDE4_KDEUI_LIBRARY}
  )
  ## NOTE Exeptions needed by QX11Grab::record()
  REMOVE_DEFINITIONS ("-DQT_NO_EXCEPTIONS")
  STRING (REGEX REPLACE "\\-fno\\-exceptions" "-fexceptions" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  ADD_DEFINITIONS ("-DHAVE_KDE4_SUPPORT")
ENDIF (KDE4_FOUND)

##############################################################
## Debugger CXXFLAGS
##############################################################

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQX11GRAB_DEBUG")
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

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
