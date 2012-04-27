## CMakeLists.txt

SET (_noinst_templates
  qx11grab_template.svg
  positions_template.svg
)

SET (_position_templates
  position_bottom_center
  position_bottom_left
  position_bottom_rigth
  position_middle_center
  position_middle_left
  position_middle_right
  position_top_center
  position_top_left
  position_top_right
  position_unknown
)

FIND_PROGRAM (INKSCAPE_BINARY NAMES inkscape)
IF (INKSCAPE_BINARY)
  MESSAGE (STATUS "Icon Theme Icons")

  FILE (MAKE_DIRECTORY ${ARCHIVE_OUTPUT_PATH})
  FOREACH(tpl ${_position_templates})
    EXECUTE_PROCESS (COMMAND ${INKSCAPE_BINARY} --export-png=${ARCHIVE_OUTPUT_PATH}/${tpl}.png --without-gui --vacuum-defs --export-dpi=72 --export-width=48 --export-height=48 --export-background=ffffffff ${CMAKE_CURRENT_SOURCE_DIR}/${tpl}.svg)
    INSTALL (FILES
      ${ARCHIVE_OUTPUT_PATH}/${tpl}.png
      DESTINATION share/icons/hicolor/48x48/actions/
      RENAME qx11grab_${tpl}.png
      COMPONENT data
    )
  ENDFOREACH(tpl)
ELSE (INKSCAPE_BINARY)
    MESSAGE (WARNING "Missing Inkscape for building extra icons!")
ENDIF (INKSCAPE_BINARY)

##EOF
