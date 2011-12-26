# CMakeLists.txt
##############################################################
PROJECT (qx11grab)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0 FATAL_ERROR)

##############################################################
# CMP0017: we have multiple targets for CheckCXXSourceCompiles with kde4
##############################################################
# CMAKE_POLICY (SET CMP0017 OLD)
ENABLE_LANGUAGE (CXX)

##############################################################
# where to look first for cmake modules, before ${CMAKE_ROOT}/cmake/Modules/ is checked
##############################################################
SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

##############################################################
## Requiered "QtCore QtGui QtXml QtNetwork"
##############################################################
SET (QT_MIN_VERSION 4.6.0)
FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Qt4 COMPONENTS QTDBUS QTXML)
FIND_PACKAGE (PkgConfig REQUIRED)
FIND_PACKAGE (ALSA REQUIRED)

## Disable KDE4 Framework Support
OPTION (ENABLE_KDE_SUPPORT "Enable KDE4 Environment (ON/OFF)" OFF)
IF (ENABLE_KDE_SUPPORT)
  FIND_PACKAGE (KDE4 4.5.0)
ENDIF (ENABLE_KDE_SUPPORT)

OPTION (INSTALL_FFPRESETS "Install extra FFmpeg ffpresets (ON/OFF)" OFF)

SET (QX11GRAB_DBUS_DOMAIN_NAME "de.hjcms.${CMAKE_PROJECT_NAME}")

IF (NOT UNIX)
  MESSAGE (FATAL_ERROR "Only Unix is Supported!")
ENDIF (NOT UNIX)

##############################################################
## Set qx11grab Specific Information
##############################################################

SET (QX11GRAB_VERSION_MAJOR 0)
SET (QX11GRAB_VERSION_MINOR 3)
SET (QX11GRAB_VERSION_RELEASE 0)
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

  SET (QX11GRAB_LDFLAGS "" CACHE STRING "additional project linker flags")

  IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET (QX11GRAB_LDFLAGS "${QX11GRAB_LDFLAGS} -Wl,--build-id")
  ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

ENDIF (CMAKE_COMPILER_IS_GNUCXX)

REMOVE_DEFINITIONS (
  -DQT3_SUPPORT_WARNINGS
  -DQT3_SUPPORT
)

IF (CMAKE_BUILD_TYPE STREQUAL "Release")
  ADD_DEFINITIONS (-Wno-deprecated-declarations -Wno-unused-result)
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Release")

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
  "ffmpeg pkg-config or library file suffix (STRING)"
)

SET (FFMPEG_AVFORMAT_VERSION "52.78.3" CACHE STRING
  "overwrite required libavformat Version (STRING)" FORCE
)

SET (FFMPEG_AVCODEC_VERSION "52.86.1" CACHE STRING
  "overwrite required libavcodec Version (STRING)" FORCE
)

SET (FFMPEG_AVUTIL_VERSION "50.24.0" CACHE STRING
  "overwrite required libavutil Version (STRING)" FORCE
)

PKG_CHECK_MODULES (FFMPEG REQUIRED 
  libavformat${FFMPEG_SUFFIX}>=${FFMPEG_AVFORMAT_VERSION}
  libavcodec${FFMPEG_SUFFIX}>=${FFMPEG_AVCODEC_VERSION}
  libavutil${FFMPEG_SUFFIX}>=${FFMPEG_AVUTIL_VERSION}
)

SET (FFMPEG_INCLUDEDIR ${FFMPEG_STATIC_INCLUDE_DIRS})

FIND_PATH (_ffmpeg_sharedir
  NAMES ffmpeg
  PATH_SUFFIXES ${FFMPEG_SUFFIX}
  PATHS
    /usr/share
    /usr/local/share
  DOC "Find FFmpeg data directory for ffpreset files"
  NO_DEFAULT_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_PATH
)

IF (_ffmpeg_sharedir)
  SET (FFMPEG_SHAREDIR "${_ffmpeg_sharedir}/ffmpeg" CACHE PATH
    "FFmpeg data directory for ffpreset files (PATH)")
ENDIF (_ffmpeg_sharedir)

## required by FFmpeg
IF (FFMPEG_FOUND)
  ADD_DEFINITIONS (-D__STDC_CONSTANT_MACROS)
ENDIF (FFMPEG_FOUND)

##############################################################
## PulseAudio 
##############################################################

SET (PULSE_SUFFIX "" CACHE STRING
  "pulse pkg-config or library file suffix (STRING)"
)

SET (PULSE_VERSION "0.9" CACHE STRING
  "overwrite required libpulse Version (STRING)" FORCE
)

PKG_CHECK_MODULES (PULSE 
  libpulse${PULSE_SUFFIX}>=${PULSE_VERSION}
)

IF (PULSE_FOUND)
  ADD_DEFINITIONS (-DHAVE_PULSE)
ENDIF (PULSE_FOUND)

##############################################################
## Xorg 
##############################################################

PKG_CHECK_MODULES (X11 REQUIRED x11>=1.3.4 )

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
  ${FFMPEG_INCLUDEDIR}
  ${PULSE_INCLUDEDIR}
)

##############################################################
## LINKER PATHS
##############################################################

SET (QX11GRAB_LIBRARIES
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
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
  STRING (REGEX REPLACE "\\-fno\\-exceptions" "-fexceptions" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  ADD_DEFINITIONS ("-DHAVE_KDE4_SUPPORT")
ENDIF (KDE4_FOUND)

##############################################################
## Configure Source Files
##############################################################

CONFIGURE_FILE (src/version.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/src/version.h
)

##############################################################
## Default Configuration File
##############################################################

CONFIGURE_FILE (qx11grab.conf.cmake
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.conf
)

INSTALL (FILES
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.conf
  DESTINATION /etc/xdg/hjcms.de/
)

##############################################################
## Desktop File
##############################################################

CONFIGURE_FILE (qx11grab.desktop.cmake
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.desktop
)

INSTALL (FILES
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.desktop
  DESTINATION share/applications
)

##############################################################
## DBUS Service
##############################################################

FILE (WRITE ${ARCHIVE_OUTPUT_PATH}/${QX11GRAB_DBUS_DOMAIN_NAME}.service
"[D-BUS Service]
Name=${QX11GRAB_DBUS_DOMAIN_NAME}
Exec=${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_PROJECT_NAME}
"
)

INSTALL (FILES
  ${ARCHIVE_OUTPUT_PATH}/${QX11GRAB_DBUS_DOMAIN_NAME}.service
  DESTINATION share/dbus-1/services/
  COMPONENT dbus
)

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
ADD_SUBDIRECTORY (ffpresets)

##############################################################
## LGPL/GPL3 Exceptions
##############################################################

INSTALL (FILES
  NEWS
  README
  AUTHORS
  ChangeLog
  COPYING
  DESTINATION share/${CMAKE_PROJECT_NAME}
  COMPONENT data
)

INSTALL (FILES
  src/images/qx11grab.png
  DESTINATION share/pixmaps
  COMPONENT data
)

## EOF
