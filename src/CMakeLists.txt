# CMakeLists.txt

SET (QX11GRAB_BINARY_PROJECT_INCULDE
  ${QX11GRAB_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/messanger
  ${CMAKE_CURRENT_SOURCE_DIR}/messanger
  ${CMAKE_CURRENT_BINARY_DIR}/avoptions
  ${CMAKE_CURRENT_SOURCE_DIR}/avoptions
  ${CMAKE_CURRENT_BINARY_DIR}/audio
  ${CMAKE_CURRENT_SOURCE_DIR}/audio
  ${CMAKE_CURRENT_BINARY_DIR}/settings
  ${CMAKE_CURRENT_SOURCE_DIR}/settings
  ${CMAKE_CURRENT_BINARY_DIR}/tapping
  ${CMAKE_CURRENT_SOURCE_DIR}/tapping
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
  ${CMAKE_CURRENT_BINARY_DIR}/preseteditor
  ${CMAKE_CURRENT_SOURCE_DIR}/preseteditor
  ${CMAKE_CURRENT_BINARY_DIR}/bookmark
  ${CMAKE_CURRENT_SOURCE_DIR}/bookmark
  ${CMAKE_CURRENT_BINARY_DIR}/filters
  ${CMAKE_CURRENT_SOURCE_DIR}/filters
)

ADD_SUBDIRECTORY (messanger)
ADD_SUBDIRECTORY (avoptions)
ADD_SUBDIRECTORY (audio)
ADD_SUBDIRECTORY (settings)
ADD_SUBDIRECTORY (tapping)
ADD_SUBDIRECTORY (utils)
ADD_SUBDIRECTORY (ffprocess)
ADD_SUBDIRECTORY (tableeditor)
ADD_SUBDIRECTORY (logviewer)
ADD_SUBDIRECTORY (cmd)
ADD_SUBDIRECTORY (preseteditor)
ADD_SUBDIRECTORY (bookmark)
ADD_SUBDIRECTORY (filters)

SET (qx11grab_INTERFACE
  ${QX11GRAB_DBUS_DOMAIN_NAME}.xml
)

SET (qx11grab_SOURCES
  main.cpp
  application.cpp
  menubar.cpp
  toolbar.cpp
  mainwindow.cpp
  systemtray.cpp
  adaptor.cpp
  exportdialog.cpp
  playeraction.cpp
  aboutdialog.cpp
  navigator.cpp
)

SET (qx11grab_HEADERS
  application.h
  menubar.h
  toolbar.h
  mainwindow.h
  systemtray.h
  adaptor.h
  exportdialog.h
  playeraction.h
  aboutdialog.h
  navigator.h
)

QT4_WRAP_CPP (qx11grab_MOC ${qx11grab_HEADERS})

INCLUDE_DIRECTORIES (${QX11GRAB_BINARY_PROJECT_INCULDE})

QT4_ADD_RESOURCES (qx11grab_RESRC qx11grab.qrc)

ADD_EXECUTABLE (${CMAKE_PROJECT_NAME}
  ${qrc_rcresource}
  ${qx11grab_RESRC}
  ${qx11grab_MOC}
  ${qx11grab_HEADERS}
  ${qx11grab_SOURCES}
)

TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME}
  Messanger
  Settings
  TableEditor
  LogViewer
  Cmd
  AVOptions
  Audio
  Tapping
  Utils
  FFProcess
  PresetEditor
  Bookmark
  ${QX11GRAB_LIBRARIES}
  qx11grabinterface
)

SET_TARGET_PROPERTIES (${CMAKE_PROJECT_NAME} PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS} ${APP_PIE_CXXFLAGS}"
  LINK_FLAGS "${APP_PIE_LDFLAGS} -Wl,--as-needed ${QX11GRAB_LDFLAGS}"
)

INSTALL (TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)

##############################################################
## DBUS Interface/Service Files
##############################################################

IF (DBUS_FOUND)

FILE (WRITE ${ARCHIVE_OUTPUT_PATH}/${QX11GRAB_DBUS_DOMAIN_NAME}.service
"[D-BUS Service]
Name=${QX11GRAB_DBUS_DOMAIN_NAME}
Exec=${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_PROJECT_NAME}
"
)

INSTALL (FILES
  ${ARCHIVE_OUTPUT_PATH}/${QX11GRAB_DBUS_DOMAIN_NAME}.service
  DESTINATION ${DBUS_SERVICES_DIR}
  COMPONENT dbus
)

INSTALL (FILES
  ${qx11grab_INTERFACE}
  DESTINATION ${DBUS_INTERFACES_DIR}
  COMPONENT dbus
)

ENDIF (DBUS_FOUND)

## EOF
