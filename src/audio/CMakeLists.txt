# CMakeLists.txt

SET(_subproject "Audio")

SET (_subproject_SOURCES
  audiodevicewidget.cpp
  abstractaudiodialog.cpp
  alsaaudiodialog.cpp
  opensounddialog.cpp
  audiodevice.cpp
)

SET (_subproject_HEADERS
  audiodevicewidget.h
  abstractaudiodialog.h
  alsaaudiodialog.h
  opensounddialog.h
)

IF (PULSE_FOUND)

LINK_DIRECTORIES (
  ${PULSE_LIBRARY_DIRS}
)

SET (_subproject_SOURCES
  ${_subproject_SOURCES}
  pulseaudiodialog.cpp
)

SET (_subproject_HEADERS
  ${_subproject_HEADERS}
  pulseaudiodialog.h
)

ENDIF (PULSE_FOUND)

SET (_subproject_NOMOC_HEADERS
  audiodevice.h
)

QT4_WRAP_CPP (_subproject_MOC ${_subproject_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QX11GRAB_INCLUDES}
)

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
  LINK_FLAGS "${LIB_FPIC_LDFLAGS} ${APP_PIE_LDFLAGS}"
)

TARGET_LINK_LIBRARIES (${_subproject}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${ALSA_LIBRARY}
  ${PULSE_LDFLAGS}
)

## EOF
