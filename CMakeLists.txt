# CMakeLists.txt
##############################################################
PROJECT (qx11grab)

CMAKE_MINIMUM_REQUIRED (VERSION 2.8.0 FATAL_ERROR)
ENABLE_LANGUAGE (CXX)

IF (POLICY CMP0017)
  CMAKE_POLICY (SET CMP0017 NEW)
ENDIF (POLICY CMP0017)

IF (POLICY CMP0022)
  CMAKE_POLICY (SET CMP0022 OLD)
ENDIF (POLICY CMP0022)

IF (POLICY CMP0026)
  CMAKE_POLICY (SET CMP0026 OLD)
ENDIF (POLICY CMP0026)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")

##############################################################
## Main Dependeces
##############################################################
SET (QT_MIN_VERSION 4.7.2)
FIND_PACKAGE (Qt4 REQUIRED)
FIND_PACKAGE (Qt4 COMPONENTS QTDBUS QTXML QTNETWORK)
FIND_PACKAGE (QJSON REQUIRED)
FIND_PACKAGE (PkgConfig REQUIRED)
FIND_PACKAGE (ALSA REQUIRED)
FIND_PACKAGE (FFmpeg REQUIRED)
FIND_PACKAGE (V4L2)
## PulseAudio
OPTION (ENABLE_PULSE "Enable PulseAudio Support (ON/OFF)" ON)
IF (ENABLE_PULSE)
  FIND_PACKAGE (Pulse REQUIRED)
ENDIF (ENABLE_PULSE)

OPTION (INSTALL_EXTRA_FFPRESETS "Install extra FFmpeg ffpresets (ON/OFF)" OFF)

IF (NOT UNIX)
  MESSAGE (FATAL_ERROR "Only Unix is Supported!")
ENDIF (NOT UNIX)

##############################################################
## Set qx11grab Specific Information
##############################################################

SET (QX11GRAB_VERSION_MAJOR 0)
SET (QX11GRAB_VERSION_MINOR 4)
SET (QX11GRAB_VERSION_RELEASE 12)
SET (QX11GRAB_VERSION_DEVELOPMENT "")
SET (QX11GRAB_VERSION "${QX11GRAB_VERSION_MAJOR}.${QX11GRAB_VERSION_MINOR}.${QX11GRAB_VERSION_RELEASE}${QX11GRAB_VERSION_DEVELOPMENT}")
SET (QX11GRAB_DBUS_DOMAIN_NAME "de.hjcms.qx11grab")
# If Git is installed and a '.git' directory is found,
# we append the Git revision to QX11GRAB_VERSION
IF(EXISTS "${qx11grab_SOURCE_DIR}/.git")
  FIND_PROGRAM (GIT_EXECUTABLE NAMES git)
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
## Macros
##############################################################

INCLUDE (CheckCXXCompilerFlag)
INCLUDE (MacroEnsureVersion)

##############################################################
## Twitch JSON Script URL
## https://github.com/justintv/twitch-api
##############################################################

SET (TWITCH_JSON_SERVERS "https://api.twitch.tv/kraken/ingests")

##############################################################
## Compile Optimisation
##############################################################
INCLUDE (CheckingGNUCXX)

SET (QX11GRAB_LDFLAGS "" CACHE STRING "additional project linker flags")

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET (QX11GRAB_LDFLAGS "-Wl,--build-id -Wl,--warn-common")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

IF (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  SET (QX11GRAB_LDFLAGS "-Wl,--build-id")
ENDIF (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")

OPTION (BUILD_WERROR "Build with -Werror" OFF)
IF(BUILD_WERROR)
  SET (QX11GRAB_LDFLAGS "-Wl,--build-id -Wl,--warn-common -Wl,--warn-constructors -Wl,--warn-multiple-gp")
  SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Werror -Wno-unused-local-typedefs")
ENDIF (BUILD_WERROR)

SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DQX11GRAB_DEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-deprecated-declarations -Wno-unused-result -Wno-unused-parameter -Wno-unused-but-set-variable")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS_RELEASE}")
SET (CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${CMAKE_CXX_FLAGS_RELEASE}")

## Enable Q4 OpenGL Support
OPTION (ENABLE_OPENGL "Enable Q4 OpenGL Support (ON/OFF)" OFF)
IF (ENABLE_OPENGL)
  SET (QT_USE_QTOPENGL_DEPENDS 1)
  ADD_DEFINITIONS (-DHAVE_OPENGL)
ENDIF (ENABLE_OPENGL)

## Enable Optional KDE4 Framework Support
OPTION (ENABLE_KDE_SUPPORT "Enable KDE4 Environment (ON/OFF)" OFF)
IF (ENABLE_KDE_SUPPORT)
  FIND_PACKAGE (KDE4 4.5.0)
  IF (KDE4_FOUND)
    ADD_DEFINITIONS (-DHAVE_KDE4_SUPPORT)
  ENDIF (KDE4_FOUND)
ENDIF (ENABLE_KDE_SUPPORT)

## Enable Experimental Extensions
OPTION (ENABLE_EXPERIMENTAL "Enable Experimental Code (ON/OFF)" OFF)
IF (ENABLE_EXPERIMENTAL)
  ADD_DEFINITIONS (-DENABLE_EXPERIMENTAL)
ENDIF (ENABLE_EXPERIMENTAL)

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

SET (PLUGIN_OUTPUT_PATH
  ${CMAKE_CURRENT_BINARY_DIR}/app/plugins
)

##############################################################
## FFmpeg Filters
##############################################################
FFMPEG_FILTER_EXISTS ("fade" ENABLE_FADE_FILTER)
FFMPEG_FILTER_EXISTS ("setdar" ENABLE_SETDAR_FILTER)
FFMPEG_FILTER_EXISTS ("movie" ENABLE_WATERMARK_FILTER)
FFMPEG_FILTER_EXISTS ("drawtext" ENABLE_DRAWTEXT_FILTER)
FFMPEG_FILTER_EXISTS ("overlay" ENABLE_WEBCAM_FILTER)
IF (ENABLE_DRAWTEXT_FILTER)
  PKG_CHECK_MODULES (FONTS REQUIRED freetype2>=2.4.0 fontconfig>=2.4.0)
  IF (NOT FONTS_FOUND)
    MESSAGE (FATAL_ERROR "freetype2 or fontconfig not found but drawtext filter enabled")
  ENDIF (NOT FONTS_FOUND)
ENDIF (ENABLE_DRAWTEXT_FILTER)

IF (ENABLE_EXPERIMENTAL)
  IF (ENABLE_WEBCAM_FILTER AND NOT V4L2_FOUND)
    SET (ENABLE_WEBCAM_FILTER FALSE)
  ENDIF (ENABLE_WEBCAM_FILTER AND NOT V4L2_FOUND)
ELSE (ENABLE_EXPERIMENTAL)
  SET (ENABLE_WEBCAM_FILTER FALSE)
ENDIF (ENABLE_EXPERIMENTAL)

##############################################################
## Xorg 
##############################################################

PKG_CHECK_MODULES (XLIBS REQUIRED x11>=1.3.4 xrandr>=1.3)
IF (NOT XLIBS_FOUND)
  MESSAGE (FATAL_ERROR "X11 or XRandr not found")
ENDIF (NOT XLIBS_FOUND)


##############################################################
## DBus-1
## The current stable branch is 1.4.x
## http://www.freedesktop.org/wiki/Software/dbus
##############################################################

PKG_CHECK_MODULES (DBUS REQUIRED dbus-1>=1.4.12)
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
## libnotify>=0.6
## ftp://ftp.gnome.org/pub/gnome/sources/libnotify/
##############################################################

FIND_PROGRAM (NOTIFYSEND_BINARY NAMES notify-send)
IF (NOTIFYSEND_BINARY_FOUND)
  MESSAGE (STATUS "libnotify:notify-send exists")
ENDIF (NOTIFYSEND_BINARY_FOUND)

##############################################################
## Inkscape
##############################################################

FIND_PROGRAM (INKSCAPE_BINARY NAMES inkscape)
IF (INKSCAPE_BINARY)
  MESSAGE (STATUS "Create extra theme pixmaps")
ELSE (INKSCAPE_BINARY)
  MESSAGE (WARNING "Missing Inkscape for building extra icons!")
ENDIF (INKSCAPE_BINARY)

##############################################################
## my debugger targets
##############################################################

FIND_PROGRAM (VALGRIND_BINARY NAMES valgrind)
IF (VALGRIND_BINARY)
  SET (VALGRIND_OPTIONS
    --tool=memcheck --leak-check=full --show-leak-kinds=all --trace-children=yes --track-origins=yes
    CACHE STRING "force my default options to ignore ~/.valgrindrc" FORCE
  )
  ADD_CUSTOM_TARGET(memcheck
    COMMAND ${VALGRIND_BINARY} ${VALGRIND_OPTIONS} ./${CMAKE_PROJECT_NAME}
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    COMMENT "run the valgrind tool to called memcheck"
    VERBATIM
  )
ENDIF (VALGRIND_BINARY)

FIND_PROGRAM (GDB_BINARY NAMES gdb)
IF (GDB_BINARY)
  SET (GDB_OPTIONS
    --nh --directory=${CMAKE_CURRENT_SOURCE_DIR}/src
    CACHE STRING "force my default options to ignore ~/.gdbinit" FORCE
  )
  ADD_CUSTOM_TARGET(gdb
    COMMAND ${GDB_BINARY} ${GDB_OPTIONS} ./${CMAKE_PROJECT_NAME}
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    COMMENT "run the gdb debugger"
    VERBATIM
  )
ENDIF (GDB_BINARY)

FIND_PROGRAM (DDD_BINARY NAMES ddd)
IF (DDD_BINARY)
  SET (DDD_OPTIONS
    -x --nh --directory=${CMAKE_CURRENT_SOURCE_DIR}/src
    CACHE STRING "create my default options to ignore ~/.gdbinit" FORCE
  )
  ADD_CUSTOM_TARGET(ddd
    COMMAND ${DDD_BINARY} ${DDD_OPTIONS} ./${CMAKE_PROJECT_NAME}
    WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
    COMMENT "run the ddd debugger"
    VERBATIM
  )
ENDIF (DDD_BINARY)

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
## Definitions
## NOTE set this behind ENABLE_KDE_SUPPORT and ENABLE_OPENGL !!!
##############################################################

REMOVE_DEFINITIONS (
  -DQT3_SUPPORT_WARNINGS
  -DQT3_SUPPORT
)

##############################################################
## Configure Source Files
##############################################################

CONFIGURE_FILE (src/version.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/src/version.h
)

##############################################################
## Manpages
##############################################################

CONFIGURE_FILE (qx11grab.de.1.cmake
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.de.1
)

INSTALL (FILES
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.de.1
  RENAME qx11grab.1
  DESTINATION share/man/de/man1/
  COMPONENT data
)

CONFIGURE_FILE (qx11grab.1.cmake
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.1
)

INSTALL (FILES
  ${ARCHIVE_OUTPUT_PATH}/qx11grab.1
  DESTINATION share/man/man1/
  COMPONENT data
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
  COMPONENT core
)

##############################################################
## Application Package (PackageKit)
##############################################################

CONFIGURE_FILE (qx11grab.app.cmake
  ${ARCHIVE_OUTPUT_PATH}/info
)

##############################################################
## RPM/PKGBUILD Helper
##############################################################

CONFIGURE_FILE (qx11grab.spec.cmake qx11grab.spec)

##############################################################
## SUBDIRECTORIES
##############################################################
ADD_SUBDIRECTORY (src)
IF (QT_LRELEASE_EXECUTABLE)
  ADD_SUBDIRECTORY (i18n)
ENDIF (QT_LRELEASE_EXECUTABLE)
ADD_SUBDIRECTORY (icons)
ADD_SUBDIRECTORY (ffpresets)
IF (INKSCAPE_BINARY)
  ADD_SUBDIRECTORY (svg)
ENDIF (INKSCAPE_BINARY)

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
## cppcheck
##############################################################

CONFIGURE_FILE (qx11grab.cppcheck.cmake
  ${CMAKE_CURRENT_SOURCE_DIR}/qx11grab.cppcheck
)

##############################################################
## CPack
##############################################################
INCLUDE (QX11Grab.cpack)

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
MESSAGE ("Configuration summary for:")
MESSAGE (STATUS "${CMAKE_PROJECT_NAME} Install Directory ${CMAKE_INSTALL_PREFIX}")
MESSAGE (STATUS "Install DBus Interface spec to ${DBUS_INTERFACES_DIR}")
MESSAGE (STATUS "Install DBus Service spec to ${DBUS_SERVICES_DIR}")
MESSAGE (STATUS "Install ffpreset to ${FFMPEG_SHAREDIR}")
IF (INSTALL_EXTRA_FFPRESETS)
  MESSAGE (STATUS "Install extra FFmpeg ffpresets to ${FFMPEG_SHAREDIR}")
ELSE (INSTALL_EXTRA_FFPRESETS)
  MESSAGE (STATUS "Install extra FFmpeg ffpresets disabled")
ENDIF (INSTALL_EXTRA_FFPRESETS)
MESSAGE ("")
MESSAGE ("Required Packages:")
summary (FFMPEG_FOUND "FFmpeg" FALSE)
summary (XLIBS_FOUND "XLIBS" FALSE)
summary (DBUS_FOUND "DBus" FALSE)
summary (QT4_FOUND "Qt4" FALSE)
summary (ALSA_FOUND "Alsa" FALSE)
MESSAGE ("")
MESSAGE ("Optional Filters:")
summary (ENABLE_FADE_FILTER "fade filter" TRUE)
summary (ENABLE_SETDAR_FILTER "setdar filter" TRUE)
summary (ENABLE_WATERMARK_FILTER "watermark filter" TRUE) 
summary (ENABLE_DRAWTEXT_FILTER "drawtext filter" TRUE)
summary (ENABLE_WEBCAM_FILTER "webcam filter" TRUE)
MESSAGE ("")
MESSAGE ("Optional Packages:")
summary (NOTIFYSEND_BINARY "notify-send" TRUE)
summary (INKSCAPE_BINARY "inkscape" TRUE)
summary (PULSE_FOUND "pulseaudio" TRUE)
summary (KDE4_FOUND "kde4" TRUE)
MESSAGE ("")

## EOF
