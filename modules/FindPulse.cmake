# - Find pulse
#
# Find the pulse libraries for qx11grab
#
# This module defines the following variables:
#   PULSE_FOUND       - True if pulse are found
#   PULSE_LIBRARIES   - Set when libav* Libraries found
#   PULSE_INCLUDEDIR  - where to find headers
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

SET (PULSE_SUFFIX "" CACHE STRING
  "pulse pkg-config or library file suffix (STRING)"
)

SET (PULSE_MIN_VERSION "0.9" CACHE STRING
  "Required libpulse${PULSE_SUFFIX} Version" FORCE
)

#=============================================================================
# Main
#=============================================================================

SET (PULSE_FOUND 0)

IF (NOT PKG_CONFIG_FOUND)
  FIND_PACKAGE (PkgConfig REQUIRED)
ENDIF(NOT PKG_CONFIG_FOUND)

PKG_CHECK_MODULES (PULSE libpulse${PULSE_SUFFIX}>=${PULSE_MIN_VERSION})

#=============================================================================
# Optional - pulseaudio library target
#=============================================================================
IF (PULSE_FOUND)
  INCLUDE (MacroEnsureVersion)
  ## from pulse version 1.1 to 2.0 the pulsecommon library was moved in a sub directory
  MACRO_ENSURE_VERSION_RANGE (${PULSE_MIN_VERSION} ${PULSE_VERSION} "1.9" PULSE_OLD_API)

  IF (NOT PULSE_OLD_API)
    MESSAGE (STATUS "  search pulseaudio linker target for pulse>=${PULSE_VERSION}")
    FIND_FILE (PULSE_LIBRARY_DIRS
      NAMES pulseaudio
      PATH_SUFFIXES ${PULSE_SUFFIX}
      PATHS
        ${CMAKE_INSTALL_PREFIX}/lib
        ${CMAKE_INSTALL_PREFIX}/lib64
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
      DOC "Find pulseaudio directory for pulsecommon and pulsedsp"
      NO_DEFAULT_PATH
    )
    LINK_DIRECTORIES (${PULSE_LIBRARY_DIRS})
  ENDIF (NOT PULSE_OLD_API)

  ADD_DEFINITIONS (-DHAVE_PULSE)

ENDIF (PULSE_FOUND)

MARK_AS_ADVANCED (PULSE_LIBRARIES PULSE_INCLUDEDIR)

#=============================================================================
##EOF
