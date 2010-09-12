# CMakeLists.txt

SET (logviewer_SOURCES
  logviewer.cpp
  textview.cpp
  highlighter.cpp
)

SET (logviewer_HEADERS
  logviewer.h
  textview.h
  highlighter.h
)

QT4_WRAP_CPP (logviewer_MOC ${logviewer_HEADERS})

ADD_DEFINITIONS (-D__STDC_CONSTANT_MACROS)

QT4_ADD_RESOURCES (logviewer_RESRC ../qx11grab.qrc)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QX11GRAB_INCLUDES}
)

ADD_LIBRARY (LogViewer STATIC
  ${logviewer_MOC}
  ${logviewer_RESRC}
  ${logviewer_HEADERS}
  ${logviewer_SOURCES}
)

TARGET_LINK_LIBRARIES (LogViewer
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

SET_TARGET_PROPERTIES (LogViewer PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

## EOF
