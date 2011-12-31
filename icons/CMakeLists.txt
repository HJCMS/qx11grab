## CMakeLists.txt

SET (icon_sizes 192 128 96 64 48 36 32 24 22 16)

FOREACH(s ${icon_sizes})
  INSTALL (FILES
    qx11grab_${s}.png
    DESTINATION share/hicolor/${s}x${s}/apps/
    RENAME qx11grab.png
    COMPONENT data
  )
ENDFOREACH(s)

INSTALL (FILES
  qx11grab.svgz
  DESTINATION share/hicolor/scalable/apps/
  COMPONENT data
)

## Pixmaps
INSTALL (FILES
  qx11grab_36.png
  DESTINATION share/pixmaps/qx11grab.png
  COMPONENT data
)

##EOF
