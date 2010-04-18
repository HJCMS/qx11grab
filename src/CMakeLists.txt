# CMakeLists.txt

SET (QX11GRAB_BINARY_PROJECT_INCULDE
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/tableeditor
  ${CMAKE_CURRENT_SOURCE_DIR}/tableeditor
  ${QX11GRAB_INCLUDES}
)

ADD_SUBDIRECTORY (tableeditor)

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
  qx11grabadaptor.cpp
  qx11grab.cpp
  rubberband.cpp
  screencombobox.cpp
  settings.cpp
  windowgrabber.cpp
)

SET (qx11grab_HEADERS
  desktopinfo.h
  defaults.h
  ffprocess.h
  grabberinfo.h
  metadata.h
  qx11grabadaptor.h
  qx11grab.h
  rubberband.h
  screencombobox.h
  settings.h
  windowgrabber.h
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
  ${QX11GRAB_LIBRARIES}
  TableEditor
)

INSTALL (TARGETS qx11grab DESTINATION bin)

## EOF
