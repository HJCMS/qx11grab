# CMakeLists.txt

SET(_subproject "AVOptions")

SET (_subproject_SOURCES
  avoptions.cpp
)

SET (_subproject_HEADERS
  avoptions.h
)

SET (_subproject_NOMOC_HEADERS
)

## NOTE add in main CMakeLists.txt
# ADD_DEFINITIONS (-D__STDC_CONSTANT_MACROS)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QX11GRAB_INCLUDES}
)

QT4_WRAP_CPP (_subproject_MOC ${_subproject_HEADERS})

ADD_LIBRARY (${_subproject} STATIC
  ${_subproject_MOC}
  ${_subproject_HEADERS}
  ${_subproject_NOMOC_HEADERS}
  ${_subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
  LINK_FLAGS "${APP_PIE_LDFLAGS}"
)

TARGET_LINK_LIBRARIES (${_subproject}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${FFMPEG_LDFLAGS}
  ${ALSA_LIBRARY}
  ${PULSE_LDFLAGS}
)

## EOF