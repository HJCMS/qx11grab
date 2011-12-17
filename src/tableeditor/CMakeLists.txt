# CMakeLists.txt

SET (tableeditor_SOURCES
  tableeditor.cpp
  codectable.cpp
  codectablemodel.cpp
)

SET (tableeditor_HEADERS
  tableeditor.h
  codectable.h
  codectablemodel.h
)

QT4_WRAP_CPP (tableeditor_MOC ${tableeditor_HEADERS})

ADD_DEFINITIONS (-D__STDC_CONSTANT_MACROS)

QT4_ADD_RESOURCES (tableeditor_RESRC ../qx11grab.qrc)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QX11GRAB_INCLUDES}
  ${QX11GRAB_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (TableEditor STATIC
  ${tableeditor_MOC}
  ${tableeditor_RESRC}
  ${tableeditor_HEADERS}
  ${tableeditor_SOURCES}
)

ADD_DEPENDENCIES (TableEditor AVOptions)

TARGET_LINK_LIBRARIES (TableEditor
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  AVOptions
)

SET_TARGET_PROPERTIES (TableEditor PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

## EOF
