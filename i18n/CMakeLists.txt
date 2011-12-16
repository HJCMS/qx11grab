# CMakeLists.txt

SET (single_file_lng
  lt
)

## merge with qt_*.ts
SET (multi_file_lng
  de
  cs
)

EXEC_PROGRAM (${QT_QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_TRANSLATIONS" OUTPUT_VARIABLE INSTALL_TRANSLATIONS)

FILE (MAKE_DIRECTORY ${ARCHIVE_OUTPUT_PATH})
FOREACH (lng ${single_file_lng})
  EXEC_PROGRAM (${QT_LRELEASE_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS -compress -silent -nounfinished qx11grab_${lng}.ts -qm ${ARCHIVE_OUTPUT_PATH}/qx11grab_${lng}.qm
  )
ENDFOREACH (lng)

FOREACH (lng ${multi_file_lng})
  EXEC_PROGRAM (${QT_LRELEASE_EXECUTABLE}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ARGS -compress -silent -nounfinished qt_${lng}.ts qx11grab_${lng}.ts -qm ${ARCHIVE_OUTPUT_PATH}/qx11grab_${lng}.qm
  )
ENDFOREACH (lng)

FILE(GLOB QM_FILES "${ARCHIVE_OUTPUT_PATH}/qx11grab_*.qm")
INSTALL (FILES ${QM_FILES}
  DESTINATION ${QT_TRANSLATIONS_DIR}
  COMPONENT translation
)

INSTALL (FILES
  qx11grab_untranslated.ts
  DESTINATION share/qx11grab
  COMPONENT translation
)

## EOF
