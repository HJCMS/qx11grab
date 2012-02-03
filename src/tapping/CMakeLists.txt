# CMakeLists.txt

SET(_subproject "Tapping")

SET (_subproject_SOURCES
  desktoptapping.cpp
  desktopinfo.cpp
  rubberband.cpp
)

SET (_subproject_HEADERS
  desktoptapping.h
  desktopinfo.h
  rubberband.h
)

SET (_subproject_NOMOC_HEADERS
)

QT4_WRAP_CPP (_subproject_MOC ${_subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QX11GRAB_INCLUDES}
)

ADD_LIBRARY (${_subproject} STATIC
  ${_subproject_MOC}
  ${_subproject_HEADERS}
  ${_subproject_NOMOC_HEADERS}
  ${_subproject_SOURCES}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} ${X11_CFLAGS}"
  LINK_FLAGS "${APP_PIE_LDFLAGS} ${X11_LDFLAGS}"
)

TARGET_LINK_LIBRARIES (${_subproject}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${XLIBS_LIBRARIES}
)

## EOF
