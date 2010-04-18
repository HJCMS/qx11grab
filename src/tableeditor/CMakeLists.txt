# CMakeLists.txt

SET (tableeditor_UI
  tableeditorui.ui
)

SET (tableeditor_SOURCES
  tableeditor.cpp
)

SET (tableeditor_HEADERS
  tableeditor.h
)

QT4_WRAP_UI (tableeditor_UIC ${tableeditor_UI})

QT4_WRAP_CPP (tableeditor_MOC ${tableeditor_HEADERS})

# ADD_DEFINITIONS (${QT_DEFINITIONS})

QT4_ADD_RESOURCES (tableeditor_RESRC ../qx11grab.qrc)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${XHTMLDBG_BINARY_PROJECT_INCULDE}
)

ADD_LIBRARY (TableEditor STATIC
  ${tableeditor_MOC}
  ${tableeditor_UIC}
  ${tableeditor_RESRC}
  ${tableeditor_HEADERS}
  ${tableeditor_SOURCES}
)

TARGET_LINK_LIBRARIES (TableEditor
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
)

## EOF
