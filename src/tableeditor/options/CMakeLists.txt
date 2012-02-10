# CMakeLists.txt

SET (options_XML
  libvpx.xml
  libx264.xml
  options.dtd
)

SET (options_SOURCES
  codecoptions.cpp
  optionsfinder.cpp
)

SET (options_HEADERS
  codecoptions.h
  optionsfinder.h
)

QT4_WRAP_CPP (options_MOC ${options_HEADERS})

INCLUDE_DIRECTORIES (
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${QX11GRAB_INCLUDES}
)

ADD_LIBRARY (CodecOptions STATIC
  ${options_MOC}
  ${options_HEADERS}
  ${options_SOURCES}
)

TARGET_LINK_LIBRARIES (CodecOptions
  ${QT_QTMAIN_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTXML_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
)

SET_TARGET_PROPERTIES (CodecOptions PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${LIB_FPIC_CXXFLAGS}"
)

INSTALL (FILES
  ${options_XML}
  DESTINATION share/qx11grab/options
  COMPONENT data
)

## EOF
