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
SET( QT_USE_QTDBUS 1 )
INCLUDE (${QT_USE_FILE})

##############################################################
## Set qx11grab Specific Information
##############################################################

SET (QX11GRAB_VERSION_MAJOR 0)
SET (QX11GRAB_VERSION_MINOR 2)
SET (QX11GRAB_VERSION_RELEASE 1)
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

IF (CMAKE_COMPILER_IS_GNUCXX)

  # gcc under Windows
  IF (MINGW)
    SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--export-all-symbols -Wl,--disable-auto-import")
    SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--export-all-symbols -Wl,--disable-auto-import")
    ADD_DEFINITIONS(-DQT_NO_DEBUG)
  ENDIF (MINGW)

  # visibility support http://gcc.gnu.org/wiki/Visibility
  CHECK_CXX_COMPILER_FLAG (-fvisibility=hidden __CHECK_GCC_VISIBILITY)
  SET (__CHECK_GCC_VISIBILITY ${__CHECK_GCC_VISIBILITY} CACHE BOOL "GCC support for hidden visibility")

  # stack-protector support
  CHECK_CXX_COMPILER_FLAG(-fstack-protector __CHECK_GCC_STACK_PROTECTOR)
  SET (__CHECK_GCC_STACK_PROTECTOR ${__CHECK_GCC_STACK_PROTECTOR} CACHE BOOL "GCC support for stack-protector")

  # buffer checks
  CHECK_CXX_COMPILER_FLAG(-D_FORTIFY_SOURCE=2 __CHECK_GCC_FORTIFY_SOURCE)
  SET (__CHECK_GCC_FORTIFY_SOURCE ${__CHECK_GCC_FORTIFY_SOURCE} CACHE BOOL "GCC support for -DFORTIFY_SOURCE")

  # emit position-independent code
  CHECK_CXX_COMPILER_FLAG(-fPIC __CHECK_GCC_FPIC)
  SET (__CHECK_GCC_FPIC ${__CHECK_GCC_FPIC} CACHE BOOL "GCC support for -fPIC")

  # get the gcc version
  EXEC_PROGRAM (${CMAKE_C_COMPILER} ARGS --version OUTPUT_VARIABLE _gcc_version_info)

  STRING (REGEX MATCH "[345]\\.[0-9]\\.[0-9]" _gcc_version "${_gcc_version_info}")
  # gcc on mac just reports: "gcc (GCC) 3.3 20030304 ..." without the patch level, handle this here:
  IF (NOT _gcc_version)
    STRING (REGEX REPLACE ".*\\(GCC\\).* ([34]\\.[0-9]) .*" "\\1.0" _gcc_version "${_gcc_version_info}")
  ENDIF (NOT _gcc_version)

  MACRO_ENSURE_VERSION ("4.1.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_1)
  MACRO_ENSURE_VERSION ("4.2.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_2)
  MACRO_ENSURE_VERSION ("4.3.0" "${_gcc_version}" GCC_IS_NEWER_THAN_4_3)

  IF (__CHECK_GCC_VISIBILITY AND GCC_IS_NEWER_THAN_4_1)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")

    IF (GCC_IS_NEWER_THAN_4_2)
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
    ENDIF (GCC_IS_NEWER_THAN_4_2)

  ENDIF (__CHECK_GCC_VISIBILITY AND GCC_IS_NEWER_THAN_4_1)

  IF (GCC_IS_NEWER_THAN_4_2 AND __CHECK_GCC_VISIBILITY)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_VISIBILITY")
    ## Bugfix gcc-4.3 need explicit definition
    ADD_DEFINITIONS (-DHAVE_VISIBILITY)
  ENDIF (GCC_IS_NEWER_THAN_4_2 AND __CHECK_GCC_VISIBILITY)

  IF (__CHECK_GCC_STACK_PROTECTOR)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector -Wstack-protector")
    IF(__CHECK_GCC_FORTIFY_SOURCE)
      SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
    ENDIF(__CHECK_GCC_FORTIFY_SOURCE)
  ENDIF (__CHECK_GCC_STACK_PROTECTOR)

  IF (__CHECK_GCC_FPIC)
    SET (LIB_FPIC_CXXFLAGS "-fPIC")
  ELSE (__CHECK_GCC_FPIC)
    SET (LIB_FPIC_CXXFLAGS "")
  ENDIF (__CHECK_GCC_FPIC)

  IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -W -Wextra -Wno-long-long -DQX11GRAB_DEBUG")
  ELSE (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

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
## Configure files
##############################################################

CONFIGURE_FILE (src/version.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/src/version.h
)

##############################################################
## INCLUDES
##############################################################

SET (QX11GRAB_INCLUDES
  ${QT_QTMAIN_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

##############################################################
## LINKER PATHS
##############################################################

SET (QX11GRAB_LIBRARIES
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
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

## EOF