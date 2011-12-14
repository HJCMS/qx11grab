# CMakeLists.txt

ADD_SUBDIRECTORY (tableeditor)
ADD_SUBDIRECTORY (logviewer)

SET (QX11GRAB_BINARY_PROJECT_INCULDE
  ${QX11GRAB_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/tableeditor
  ${CMAKE_CURRENT_SOURCE_DIR}/tableeditor
  ${CMAKE_CURRENT_BINARY_DIR}/logviewer
  ${CMAKE_CURRENT_SOURCE_DIR}/logviewer
)

SET (qx11grab_UI
  qx11grabmain.ui
)

SET (qx11grab_SOURCES
  desktopinfo.cpp
  defaults.cpp
  ffprocess.cpp
  grabberinfo.cpp
  main.cpp
  metadata.cpp
  qx11grab.cpp
  rubberband.cpp
  screencombobox.cpp
  settings.cpp
  windowgrabber.cpp
  commandlineedit.cpp
  commandpreview.cpp
  audiodevice.cpp
  picrecordinterface.cpp
  qx11grabadaptor.cpp
)

SET (qx11grab_HEADERS
  desktopinfo.h
  defaults.h
  ffprocess.h
  grabberinfo.h
  metadata.h
  qx11grab.h
  rubberband.h
  screencombobox.h
  settings.h
  windowgrabber.h
  commandlineedit.h
  commandpreview.h
  audiodevice.h
  picrecordinterface.h
  qx11grabadaptor.h
)

SET (qx11grab_NOMOC
)

QT4_WRAP_UI (qx11grab_UIC ${qx11grab_UI})

QT4_WRAP_CPP (qx11grab_MOC ${qx11grab_HEADERS})

INCLUDE_DIRECTORIES (${QX11GRAB_BINARY_PROJECT_INCULDE})

QT4_ADD_RESOURCES (qx11grab_RESRC qx11grab.qrc)

ADD_EXECUTABLE (qx11grab
  ${qrc_rcresource}
  ${qx11grab_RESRC}
  ${qx11grab_MOC}
  ${qx11grab_NOMOC}
  ${qx11grab_UIC}
  ${qx11grab_HEADERS}
  ${qx11grab_SOURCES}
)

TARGET_LINK_LIBRARIES (qx11grab
  TableEditor
  LogViewer
  ${QX11GRAB_LIBRARIES}
)

SET_TARGET_PROPERTIES (qx11grab PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_CXXFLAGS}"
  LINK_FLAGS "${APP_PIE_LDFLAGS} ${LIB_FPIC_CXXFLAGS} -Wl,--as-needed ${QX11GRAB_LDFLAGS}"
)

INSTALL (TARGETS qx11grab DESTINATION bin)

## EOF
