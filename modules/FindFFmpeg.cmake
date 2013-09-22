# - Find FFmpeg
#
# Find the FFmpeg libraries for qx11grab
#
# This module defines the following variables:
#   FFMPEG_FOUND       - True if FFmpeg are found
#   FFMPEG_LIBRARIES   - Set when libav* Libraries found
#   FFMPEG_INCLUDEDIR  - where to find headers
#   FFMPEG_SHAREDIR    - Path to FFmpeg's DATA Directory FFMPEG_DATADIR or AVCONV_DATADIR
#   FFMPEG_BINARY      - Path to FFmpeg's application ffmpeg or avconv
#
#=============================================================================
# Copyright 2012 Juergen Heinemann (Undefined) http://www.hjcms.de
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#
#=============================================================================
# Configuration
#=============================================================================

## Currently Supported Versions
##  ffmpeg >= 0.10.7
##  libav >= 0.9
SET (FFMPEG_SUFFIX "" CACHE STRING
  "ffmpeg pkg-config or library file suffix (STRING)"
)

SET (FFMPEG_AVFORMAT_VERSION "53.32.100" CACHE STRING
  "Required libavformat${FFMPEG_SUFFIX} Version" FORCE
)

SET (FFMPEG_AVCODEC_VERSION "53.61.100" CACHE STRING
  "Required libavcodec${FFMPEG_SUFFIX} Version" FORCE
)

SET (FFMPEG_AVUTIL_VERSION "51.35.100" CACHE STRING
  "Required libavutil${FFMPEG_SUFFIX} Version" FORCE
)

SET (FFMPEG_AVFILTER_VERSION "2.61.100" CACHE STRING
  "Required libavfilter${FFMPEG_SUFFIX} Version" FORCE
)

#=============================================================================
# Main
#=============================================================================

SET (FFMPEG_FOUND 0)

IF (NOT PKG_CONFIG_FOUND)
  FIND_PACKAGE (PkgConfig REQUIRED)
ENDIF(NOT PKG_CONFIG_FOUND)

PKG_CHECK_MODULES (_ffmpeg REQUIRED
  libavformat${FFMPEG_SUFFIX}>=${FFMPEG_AVFORMAT_VERSION}
  libavcodec${FFMPEG_SUFFIX}>=${FFMPEG_AVCODEC_VERSION}
  libavutil${FFMPEG_SUFFIX}>=${FFMPEG_AVUTIL_VERSION}
  libavfilter${FFMPEG_SUFFIX}>=${FFMPEG_AVFILTER_VERSION}
)

IF (NOT _ffmpeg_FOUND)
  MESSAGE (FATAL_ERROR "Required FFmpeg libraries not found")
ENDIF (NOT _ffmpeg_FOUND)

SET (FFMPEG_FOUND ${_ffmpeg_FOUND})

#=============================================================================
# Minimize Library Dependeces
#=============================================================================

FIND_LIBRARY (FFMPEG_AVFORMAT_LIBRARY
  NAMES avformat${FFMPEG_SUFFIX} avformat
  PATHS ${_ffmpeg_libavformat_LIBDIR}
)

FIND_LIBRARY (FFMPEG_AVCODEC_LIBRARY
  NAMES avcodec${FFMPEG_SUFFIX} avcodec
  PATHS ${_ffmpeg_libavcodec_LIBDIR}
)

FIND_LIBRARY (FFMPEG_AVUTIL_LIBRARY
  NAMES avutil${FFMPEG_SUFFIX} avutil
  PATHS ${_ffmpeg_libavutil_LIBDIR}
)

FIND_LIBRARY (FFMPEG_AVFILTER_LIBRARY
  NAMES avfilter${FFMPEG_SUFFIX} avfilter
  PATHS ${_ffmpeg_libavfilter_LIBDIR}
)

SET (FFMPEG_LIBRARIES
  ${FFMPEG_AVFORMAT_LIBRARY}
  ${FFMPEG_AVCODEC_LIBRARY}
  ${FFMPEG_AVUTIL_LIBRARY}
  ${FFMPEG_AVFILTER_LIBRARY}
  CACHE PATHS "FFmpeg Libraries" FORCE
)

#=============================================================================
# CFLAGS/CPPFLAGS
#=============================================================================

ADD_DEFINITIONS (-D__STDC_CONSTANT_MACROS)
IF (_ffmpeg_INCLUDE_DIRS)
  SET (FFMPEG_INCLUDEDIR ${_ffmpeg_INCLUDE_DIRS})
ELSE (_ffmpeg_INCLUDE_DIRS)
  SET (FFMPEG_INCLUDEDIR ${_ffmpeg_libavcodec_INCLUDEDIR})
ENDIF (_ffmpeg_INCLUDE_DIRS)

#=============================================================================
# Data Directory
#=============================================================================

FIND_FILE (FFMPEG_SHAREDIR
  NAMES ffmpeg avconv
  PATH_SUFFIXES ${FFMPEG_SUFFIX}
  PATHS
    /usr/share
    /usr/local/share
    $ENV{FFMPEG_DATADIR}
    $ENV{AVCONV_DATADIR}
    ${CMAKE_INSTALL_PREFIX}/share
    ${CMAKE_INSTALL_PREFIX}/local/share
  DOC "Find FFmpeg data directory for ffpreset files"
  NO_DEFAULT_PATH
)

#=============================================================================
# Optional Binary
#=============================================================================

FIND_PROGRAM (FFMPEG_BINARY
  NAMES ffmpeg avconv
  PATH_SUFFIXES ${FFMPEG_SUFFIX}
  PATHS
    /usr/bin
    /usr/local/bin
    $ENV{PATH}
    ${CMAKE_INSTALL_PREFIX}/bin
    ${CMAKE_INSTALL_PREFIX}/local/bin
  DOC "Find FFmpeg Application"
  NO_DEFAULT_PATH
)

IF (NOT FFMPEG_BINARY)
  SET (FFMPEG_BINARY "${CMAKE_INSTALL_PREFIX}/bin/ffmpeg${FFMPEG_SUFFIX}" CACHE FILEPATH
    "Fallback FFmpeg application path" FORCE
  )
ENDIF (NOT FFMPEG_BINARY)

#=============================================================================
# Check for AVCodecID
#=============================================================================

MACRO (FFMPEG_ABI_ENUM_CHECK)
TRY_RUN (__run_ret _compile_ret
  ${CMAKE_BINARY_DIR}/CMakeTmp
  ${CMAKE_SOURCE_DIR}/modules/check_ffmpeg_codecid.c
  COMPILE_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS} -std=gnu89"
  CMAKE_FLAGS
    "-DINCLUDE_DIRECTORIES:STRING=${FFMPEG_INCLUDEDIR}"
    "-DLINK_LIBRARIES:STRING=${FFMPEG_AVCODEC_LIBRARY}"
  OUTPUT_VARIABLE OUTPUT
)
IF (_compile_ret)
  MESSAGE (STATUS "AVCodecID supported." )
  REMOVE_DEFINITIONS (-DQX11GRAB_OLD_AVCODEC_ID)
ELSE (_compile_ret)
  MESSAGE (WARNING "This version of ffmpeg using Deprecated CodecID. The most recent version of ffmpeg now using AVCodecID")
  ADD_DEFINITIONS (-DQX11GRAB_OLD_AVCODEC_ID)
ENDIF (_compile_ret)
ENDMACRO (FFMPEG_ABI_ENUM_CHECK)

FFMPEG_ABI_ENUM_CHECK ()

#=============================================================================
# Find Filters Macro
#=============================================================================

MACRO (FFMPEG_FILTER_EXISTS FILTER VARIABLE)
  TRY_RUN (filter_arg _filter_supported
    ${CMAKE_BINARY_DIR}/CMakeTmp
    ${CMAKE_SOURCE_DIR}/modules/check_ffmpeg_filter.c
    COMPILE_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS} -std=gnu89"
    CMAKE_FLAGS
      "-DINCLUDE_DIRECTORIES:STRING=${FFMPEG_INCLUDEDIR}"
      "-DLINK_LIBRARIES:STRING=${FFMPEG_AVUTIL_LIBRARY};${FFMPEG_AVFILTER_LIBRARY}"
    OUTPUT_VARIABLE OUTPUT
    ARGS ${FILTER}
  )
  MESSAGE (STATUS "FFmpeg compiled with ${FILTER} filter - ${_filter_supported}")
  SET (${VARIABLE} ${_filter_supported} CACHE INTERNAL "Have ffmpeg Filter ${FILTER}")
ENDMACRO (FFMPEG_FILTER_EXISTS)

#=============================================================================
# Final
#=============================================================================

MARK_AS_ADVANCED (FFMPEG_INCLUDEDIR FFMPEG_LIBRARIES FFMPEG_SHAREDIR FFMPEG_BINARY)

#=============================================================================
##EOF
