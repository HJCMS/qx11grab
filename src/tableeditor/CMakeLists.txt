# CMakeLists.txt

SET (tableeditor_SOURCES
  abstracttableeditor.cpp
  videotableeditor.cpp
  audiotableeditor.cpp
  codectable.cpp
  codectablemodel.cpp
  codectabledelegate.cpp
  abstractselection.cpp
  filtermenu.cpp
  abstractedit.cpp
  selectacodecpresets.cpp
  selectvcodecpresets.cpp
  defaultedit.cpp
)

SET (tableeditor_HEADERS
  abstracttableeditor.h
  videotableeditor.h
  audiotableeditor.h
  codectable.h
  codectablemodel.h
  codectabledelegate.h
  abstractselection.h
  filtermenu.h
  abstractedit.h
  selectacodecpresets.h
  selectvcodecpresets.h
  defaultedit.h
)

QT4_WRAP_CPP (tableeditor_MOC ${tableeditor_HEADERS})

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

ADD_DEPENDENCIES (TableEditor AVOptions Settings qx11grabinterface)

TARGET_LINK_LIBRARIES (TableEditor
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  AVOptions
  Settings
  qx11grabinterface
)

SET_TARGET_PROPERTIES (TableEditor PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

## EOF
