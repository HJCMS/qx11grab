## drawtext Plugin

SET (target_plugin "drawtext")

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(plugin_SOURCES
  drawtextplugin.cpp
  drawtext.cpp
  fontpreview.cpp
  dropshadowbox.cpp
  colorpreview.cpp
  textposition.cpp
)

SET(plugin_HEADERS
  drawtext.h
  drawtextplugin.h
  fontpreview.h
  dropshadowbox.h
  colorpreview.h
  textposition.h
)

QT4_WRAP_CPP (plugin_MOCS ${plugin_HEADERS})


INCLUDE_DIRECTORIES (
  ${QX11GRAB_INCLUDES}
  ${FONTS_INCLUDE_DIRS}
)

ADD_LIBRARY (${target_plugin} MODULE
  ${plugin_SOURCES}
  ${plugin_HEADERS}
  ${plugin_MOCS}
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${FONTS_LIBRARIES}
  qx11grabinterface
)

SET_TARGET_PROPERTIES (${target_plugin} PROPERTIES
  OUTPUT_NAME ${target_plugin}
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} -Wno-missing-declarations"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS} ${QX11GRAB_LDFLAGS}"
)

INSTALL (TARGETS ${target_plugin}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/qx11grab
  COMPONENT plugins
)

##EOF

