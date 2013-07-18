## webcam Plugin

SET (target_plugin "webcam")

SET (LIBRARY_OUTPUT_PATH
  ${PLUGIN_OUTPUT_PATH}/
)

SET(plugin_SOURCES
  v4l2-api.cpp
  webcamplugin.cpp
  webcam.cpp
  webcampreview.cpp
  webcamdevicechooser.cpp
  webcamcaptureframes.cpp
)

SET(plugin_HEADERS
  webcam.h
  webcamplugin.h
  webcampreview.h
  webcamdevicechooser.h
  webcamcaptureframes.h
)

SET(plugin_NOMOC_HEADERS
  v4l2-api.h
  webcamdeviceinfo.h
)

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
  ADD_DEFINITIONS (-DQX11EMBED_DEBUG)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")

QT4_WRAP_CPP (plugin_MOCS ${plugin_HEADERS})

INCLUDE_DIRECTORIES (
  ${QX11GRAB_INCLUDES}
  ${V4L2_INCLUDE_DIRS}
)

ADD_LIBRARY (${target_plugin} MODULE
  ${plugin_SOURCES}
  ${plugin_HEADERS}
  ${plugin_MOCS}
  ${plugin_NOMOC_HEADERS}
)

TARGET_LINK_LIBRARIES (${target_plugin}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${V4L2_LIBRARIES}
  qx11grabinterface
)

SET_TARGET_PROPERTIES (${target_plugin} PROPERTIES
  OUTPUT_NAME ${target_plugin}
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} -Wno-stack-protector -Wno-missing-declarations"
  LINK_FLAGS "${LIB_FPIC_CXXFLAGS} ${LIB_RDYNAMIC_CXXFLAGS} ${QX11GRAB_LDFLAGS}"
)

INSTALL (TARGETS ${target_plugin}
  LIBRARY DESTINATION lib${LIB_SUFFIX}/qx11grab
  COMPONENT plugins
)

##EOF

