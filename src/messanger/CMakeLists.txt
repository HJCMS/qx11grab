# CMakeLists.txt

SET(_subproject "Messanger")

SET (_subproject_XML
  org.freedesktop.Notifications.xml
)

SET (_subproject_SOURCES
  messanger.cpp
)

SET (_subproject_HEADERS
  messanger.h
)

SET (_subproject_NOMOC_HEADERS
  messanger_p.h
)

QT4_WRAP_CPP (_subproject_MOC ${_subproject_HEADERS})

QT4_ADD_DBUS_INTERFACES (_subproject_IFACE ${_subproject_XML})

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
  ${_subproject_IFACE}
)

SET_TARGET_PROPERTIES (${_subproject} PROPERTIES
  LINKER_LANGUAGE CXX
  OUTPUT_NAME ${_subproject}
  COMPILE_FLAGS " -Wno-old-style-cast ${LIB_FPIC_CXXFLAGS}"
  LINK_FLAGS "${LIB_FPIC_LDFLAGS} ${APP_PIE_LDFLAGS}"
)

TARGET_LINK_LIBRARIES (${_subproject}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
)

## EOF
