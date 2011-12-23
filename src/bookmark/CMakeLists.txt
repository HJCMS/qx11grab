# CMakeLists.txt

SET(_subproject "Bookmark")

SET (_subproject_SOURCES
  bookmarkdialog.cpp
  bookmark.cpp
  bookmarkentry.cpp
  bookmarkselecter.cpp
)

SET (_subproject_HEADERS
  bookmarkdialog.h
  bookmarkselecter.h
)

SET (_subproject_NOMOC_HEADERS
  bookmark.h
  bookmarkentry.h
)

QT4_WRAP_CPP (_subproject_MOC ${_subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QX11GRAB_INCLUDES}
  ${QX11GRAB_BINARY_PROJECT_INCULDE}
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
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
  LINK_FLAGS "${APP_PIE_LDFLAGS}"
)

TARGET_LINK_LIBRARIES (${_subproject}
  ${QX11GRAB_LIBRARIES}
  ${QT_QTXML_LIBRARY}
)

## EOF
