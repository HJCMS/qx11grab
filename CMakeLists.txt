# CMakeLists.txt
##############################################################
PROJECT (qx11grab)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.1 FATAL_ERROR)
ENABLE_LANGUAGE (CXX)

IF (CMAKE_VERSION STREQUAL 2.8.4)
  CMAKE_POLICY (SET CMP0017 OLD)
ENDIF (CMAKE_VERSION STREQUAL 2.8.4)
SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

##############################################################
## Requiered "QtCore QtGui QtXml QtNetwork"
##############################################################
SET (QT_MIN_VERSION 4.7.1)
FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Qt4 COMPONENTS QTDBUS QTXML QTNETWORK)
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
SET (QX11GRAB_VERSION_MINOR 4)
SET (QX11GRAB_VERSION_RELEASE 0)
SET (QX11GRAB_VERSION_DEVELOPMENT ".rc1")
SET (QX11GRAB_VERSION "${QX11GRAB_VERSION_MAJOR}.${QX11GRAB_VERSION_MINOR}.${QX11GRAB_VERSION_RELEASE}${QX11GRAB_VERSION_DEVELOPMENT}")
# If Git is installed and a '.git' directory is found,
# we append the Git revision to QX11GRAB_VERSION
IF(EXISTS "${qx11grab_SOURCE_DIR}/.git")
  FIND_PROGRAM(GIT_EXECUTABLE NAMES git)
  IF (GIT_EXECUTABLE)
    SET (qx11grab_git_revision "")
    MESSAGE (STATUS "Found git: ${GIT_EXECUTABLE}")
    EXECUTE_PROCESS (COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY ${qx11grab_SOURCE_DIR}
      OUTPUT_VARIABLE qx11grab_git_revision
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    SET (QX11GRAB_VERSION "${QX11GRAB_VERSION}.git(${qx11grab_git_revision})")
  ENDIF (GIT_EXECUTABLE)
ENDIF (EXISTS "${qx11grab_SOURCE_DIR}/.git")

##############################################################
## Set Header Definitions
##############################################################

INCLUDE (CheckCXXCompilerFlag)
INCLUDE (MacroEnsureVersion)

##############################################################
## Compile Optimisation
##############################################################
INCLUDE (CheckingGNUCXX)

SET (QX11GRAB_LDFLAGS "" CACHE STRING "additional project linker flags")

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET (QX11GRAB_LDFLAGS "-Wl,--build-id")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

IF (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  SET (QX11GRAB_LDFLAGS "-Wl,--build-id")
ENDIF (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")

OPTION (BUILD_WERROR "Build with -Werror" OFF)
IF(BUILD_WERROR)
  SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror")
ENDIF (BUILD_WERROR)

SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQX11GRAB_DEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-deprecated-declarations -Wno-unused-result -Wno-unused-parameter -Wno-unused-but-set-variable")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS_RELEASE}")
SET (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${CMAKE_CXX_FLAGS_RELEASE}")

REMOVE_DEFINITIONS (
  -DQT3_SUPPORT_WARNINGS
  -DQT3_SUPPORT
)

## Enable Q4 OpenGL Support
OPTION (ENABLE_OPENGL "Enable Q4 OpenGL Support (ON/OFF)" OFF)
IF (ENABLE_OPENGL)
  SET (QT_USE_QTOPENGL_DEPENDS 1)
  ADD_DEFINITIONS (-DHAVE_OPENGL)
ENDIF (ENABLE_OPENGL)

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
  "Required libavformat${FFMPEG_SUFFIX} Version" FORCE
)

SET (FFMPEG_AVCODEC_VERSION "52.86.1" CACHE STRING
  "Required libavcodec${FFMPEG_SUFFIX} Version" FORCE
)

SET (FFMPEG_AVUTIL_VERSION "50.24.0" CACHE STRING
  "Required libavutil${FFMPEG_SUFFIX} Version" FORCE
)

PKG_CHECK_MODULES (FFMPEG REQUIRED 
  libavformat${FFMPEG_SUFFIX}>=${FFMPEG_AVFORMAT_VERSION}
  libavcodec${FFMPEG_SUFFIX}>=${FFMPEG_AVCODEC_VERSION}
  libavutil${FFMPEG_SUFFIX}>=${FFMPEG_AVUTIL_VERSION}
)

IF (FFMPEG_FOUND)
  ADD_DEFINITIONS (-D__STDC_CONSTANT_MACROS)
ENDIF (FFMPEG_FOUND)

SET (FFMPEG_INCLUDEDIR ${FFMPEG_STATIC_INCLUDE_DIRS})

FIND_PATH (_ffmpeg_sharedir
  NAMES ffmpeg
  PATH_SUFFIXES ${FFMPEG_SUFFIX}
  PATHS
    /usr/share
    /usr/local/share
    $ENV{FFMPEG_DATADIR}
    $ENV{AVCONV_DATADIR}
  DOC "Find FFmpeg data directory for ffpreset files"
  NO_DEFAULT_PATH
  NO_SYSTEM_ENVIRONMENT_PATH
  NO_CMAKE_PATH
)

IF (_ffmpeg_sharedir)
  SET (FFMPEG_SHAREDIR "${_ffmpeg_sharedir}/ffmpeg" CACHE PATH
    "FFmpeg data directory for ffpreset files (PATH)")
ENDIF (_ffmpeg_sharedir)

##############################################################
## PulseAudio 
##############################################################

SET (PULSE_SUFFIX "" CACHE STRING
  "pulse pkg-config or library file suffix (STRING)"
)

SET (PULSE_VERSION "0.9" CACHE STRING
  "Required libpulse${PULSE_SUFFIX} Version" FORCE
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

PKG_CHECK_MODULES (X11 REQUIRED x11>=1.3.4)

##############################################################
## DBus 
##############################################################

PKG_CHECK_MODULES (DBUS REQUIRED dbus-1>=1.5.0)
IF (DBUS_FOUND)
  SET (_dbus_interfaces_dir "")
  EXECUTE_PROCESS (COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=interfaces_dir dbus-1
    OUTPUT_VARIABLE _dbus_interfaces_dir
    RESULT_VARIABLE _dbus_interfaces_dir_result
    ERROR_VARIABLE _dbus_interfaces_dir_error
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  SET (DBUS_INTERFACES_DIR "${_dbus_interfaces_dir}" CACHE PATH "DBus Interfaces Directory" FORCE)

  SET (_dbus_services_dir "")
  EXECUTE_PROCESS (COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=session_bus_services_dir dbus-1
    OUTPUT_VARIABLE _dbus_services_dir
    RESULT_VARIABLE _dbus_services_dir_result
    ERROR_VARIABLE _dbus_services_dir_error
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  SET (DBUS_SERVICES_DIR "${_dbus_services_dir}" CACHE PATH "DBus Services Directory" FORCE)
ELSE (DBUS_FOUND)
  MESSAGE (FATAL_ERROR "DBus not found")
ENDIF (DBUS_FOUND)

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
  ${QT_QTNETWORK_LIBRARY}
)

IF (ENABLE_OPENGL)
  SET (QX11GRAB_LIBRARIES ${QX11GRAB_LIBRARIES} ${QT_QTOPENGL_LIBRARY})
ENDIF (ENABLE_OPENGL)

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

CONFIGURE_FILE (qx11grab.de.1.cmake
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.de.1
)

CONFIGURE_FILE (qx11grab.1.cmake
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.1
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
## SUBDIRECTORIES
##############################################################
ADD_SUBDIRECTORY (src)
IF (QT_LRELEASE_EXECUTABLE)
  ADD_SUBDIRECTORY (i18n)
ENDIF (QT_LRELEASE_EXECUTABLE)
ADD_SUBDIRECTORY (icons)
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

##############################################################
## Summary
##############################################################

MACRO (summary status info optional)
  IF (${status})
    MESSAGE(STATUS " ${info} found")
  ELSEIF (${optional})
    MESSAGE(STATUS " ${info} disabled")
  ELSE (${status})
    MESSAGE(STATUS " ${info} failed")
  ENDIF (${status})
ENDMACRO (summary)

MESSAGE ("")
MESSAGE ("Configuration summary:")
MESSAGE (STATUS "${CMAKE_PROJECT_NAME} Install Directory ${CMAKE_INSTALL_PREFIX}")
MESSAGE (STATUS "Install DBus Interface spec to ${DBUS_INTERFACES_DIR}")
MESSAGE (STATUS "Install DBus Service spec to ${DBUS_SERVICES_DIR}")
IF (INSTALL_FFPRESETS)
  MESSAGE (STATUS "Install extra FFmpeg ffpresets to ${FFMPEG_SHAREDIR}")
ELSE (INSTALL_FFPRESETS)
  MESSAGE (STATUS "Install extra FFmpeg ffpresets disabled")
ENDIF (INSTALL_FFPRESETS)
MESSAGE ("")
MESSAGE ("Required Packages:")
summary (FFMPEG_FOUND "FFmpeg" FALSE)
summary (X11_FOUND "X11" FALSE)
summary (QT4_FOUND "Qt4" FALSE)
summary (ALSA_FOUND "Alsa" FALSE)
MESSAGE ("")
MESSAGE ("Optional Packages:")
summary (PULSE_FOUND "pulseaudio" TRUE)
summary (KDE_FOUND "kde4" TRUE)
MESSAGE ("")

## EOF
