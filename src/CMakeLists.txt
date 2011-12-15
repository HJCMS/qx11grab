# CMakeLists.txt

ADD_SUBDIRECTORY (avoptions)
ADD_SUBDIRECTORY (audio)
ADD_SUBDIRECTORY (utils)
ADD_SUBDIRECTORY (ffprocess)
ADD_SUBDIRECTORY (tableeditor)
ADD_SUBDIRECTORY (logviewer)
ADD_SUBDIRECTORY (cmd)

SET (QX11GRAB_BINARY_PROJECT_INCULDE
  ${QX11GRAB_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/avoptions
  ${CMAKE_CURRENT_SOURCE_DIR}/avoptions
  ${CMAKE_CURRENT_BINARY_DIR}/audio
  ${CMAKE_CURRENT_SOURCE_DIR}/audio
  ${CMAKE_CURRENT_BINARY_DIR}/utils
  ${CMAKE_CURRENT_SOURCE_DIR}/utils
  ${CMAKE_CURRENT_BINARY_DIR}/ffprocess
  ${CMAKE_CURRENT_SOURCE_DIR}/ffprocess
  ${CMAKE_CURRENT_BINARY_DIR}/tableeditor
  ${CMAKE_CURRENT_SOURCE_DIR}/tableeditor
  ${CMAKE_CURRENT_BINARY_DIR}/logviewer
  ${CMAKE_CURRENT_SOURCE_DIR}/logviewer
  ${CMAKE_CURRENT_BINARY_DIR}/cmd
  ${CMAKE_CURRENT_SOURCE_DIR}/cmd
)

SET (qx11grab_SOURCES
  settings.cpp
  defaults.cpp
  main.cpp
  menubar.cpp
  toolbar.cpp
  qx11grab.cpp
  systemtray.cpp
  qx11grabadaptor.cpp
)

SET (qx11grab_HEADERS
  settings.h
  defaults.h
  menubar.h
  toolbar.h
  qx11grab.h
  systemtray.h
  qx11grabadaptor.h
)

SET (qx11grab_NOMOC
)

QT4_WRAP_CPP (qx11grab_MOC ${qx11grab_HEADERS})

INCLUDE_DIRECTORIES (${QX11GRAB_BINARY_PROJECT_INCULDE})

QT4_ADD_RESOURCES (qx11grab_RESRC qx11grab.qrc)

ADD_EXECUTABLE (qx11grab
  ${qrc_rcresource}
  ${qx11grab_RESRC}
  ${qx11grab_MOC}
  ${qx11grab_NOMOC}
  ${qx11grab_HEADERS}
  ${qx11grab_SOURCES}
)

TARGET_LINK_LIBRARIES (qx11grab
  TableEditor
  LogViewer
  Cmd
  AVOptions
  Audio
  Utils
  FFProcess
  ${QX11GRAB_LIBRARIES}
)

SET_TARGET_PROPERTIES (qx11grab PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_CXXFLAGS}"
  LINK_FLAGS "${APP_PIE_LDFLAGS} -Wl,--as-needed ${QX11GRAB_LDFLAGS}"
)

INSTALL (TARGETS qx11grab DESTINATION bin)

## EOF
