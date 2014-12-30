# CMakeLists.txt

ADD_SUBDIRECTORY (extensionselecter)

SET (Q_JSON_LIB "")
IF(QJSON_FOUND)
SET (Q_JSON_LIB "-lqjson")
ENDIF(QJSON_FOUND)

SET(_subproject "Settings")

SET (_subproject_SOURCES
  settings.cpp
  configdialog.cpp
  abstractconfigwidget.cpp
  mainfcuntions.cpp
  outputedit.cpp
  targetswidget.cpp
  configextensions.cpp
  configrubberband.cpp
  loglevelcombobox.cpp
  extraoptions.cpp
  iconthemeitem.cpp
  iconthememodel.cpp
  iconthemeselector.cpp
  graphicsengineselecter.cpp
  threadsspinbox.cpp
  twitch.cpp
)

SET (_subproject_HEADERS
  settings.h
  configdialog.h
  abstractconfigwidget.h
  mainfcuntions.h
  outputedit.h
  targetswidget.h
  configextensions.h
  configrubberband.h
  loglevelcombobox.h
  extraoptions.h
  iconthememodel.h
  iconthemeselector.h
  graphicsengineselecter.h
  threadsspinbox.h
  twitch.h
)

SET (_subproject_NOMOC_HEADERS
  iconthemeitem.h
)

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QX11GRAB_INCLUDES}
  ${QX11GRAB_BINARY_PROJECT_INCULDE}
  ${CMAKE_CURRENT_BINARY_DIR}/extensionselecter
  ${CMAKE_CURRENT_SOURCE_DIR}/extensionselecter
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
  ${QT_QTNETWORK_LIBRARY}
  ${Q_JSON_LIB}
  ExtensionSelecter
)

## EOF
