# CMakeLists.txt

SET(_subproject "AVOptions")

SET (options_XML
  options/libvpx.xml
  options/libx264.xml
  options/options.dtd
)

SET (_subproject_SOURCES
  avoptions.cpp
  avfiltermodel.cpp
  codecselecter.cpp
  formatmenu.cpp
  codecoptions.cpp
  ffoptionsmenu.cpp
  optionsfinder.cpp
  avfilteritem.cpp
)

SET (_subproject_HEADERS
  avoptions.h
  codecselecter.h
  formatmenu.h
  codecoptions.h
  ffoptionsmenu.h
)

# IF (ENABLE_EXPERIMENTAL)
#   SET (_subproject_SOURCES
#     ${_subproject_SOURCES}
#     x11device.cpp
#   )
#   SET (_subproject_HEADERS
#     ${_subproject_HEADERS}
#     x11device.h
#   )
# ENDIF (ENABLE_EXPERIMENTAL)

SET (_subproject_NOMOC_HEADERS
  avfiltermodel.h
  optionsfinder.h
  avfilteritem.h
)

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
  ${QT_QTXML_LIBRARY}
  ${FFMPEG_LIBRARIES}
)

INSTALL (FILES
  ${options_XML}
  DESTINATION share/qx11grab/options
  COMPONENT data
)

## EOF
