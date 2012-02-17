# CMakeLists.txt

SET (single_file_lng
  lt
  sr_RS
)

## merge with qt_*.ts
SET (multi_file_lng
  de
  cs
)

EXECUTE_PROCESS (COMMAND ${QT_QMAKE_EXECUTABLE} -query QT_INSTALL_TRANSLATIONS
OUTPUT_VARIABLE INSTALL_TRANSLATIONS
)

MESSAGE ("")
MESSAGE ("Install Translations to: ${QT_TRANSLATIONS_DIR}")

FILE (MAKE_DIRECTORY ${ARCHIVE_OUTPUT_PATH})
FOREACH (lng ${single_file_lng})
  EXECUTE_PROCESS (COMMAND ${QT_LRELEASE_EXECUTABLE} -compress -silent -nounfinished ${CMAKE_CURRENT_SOURCE_DIR}/qx11grab_${lng}.ts -qm ${ARCHIVE_OUTPUT_PATH}/qx11grab_${lng}.qm)
  MESSAGE (STATUS "qx11grab_${lng}.qm")
ENDFOREACH (lng)

FOREACH (lng ${multi_file_lng})
  EXECUTE_PROCESS (COMMAND ${QT_LRELEASE_EXECUTABLE} -compress -silent -nounfinished ${CMAKE_CURRENT_SOURCE_DIR}/qt_${lng}.ts ${CMAKE_CURRENT_SOURCE_DIR}/qx11grab_${lng}.ts -qm ${ARCHIVE_OUTPUT_PATH}/qx11grab_${lng}.qm)
  MESSAGE (STATUS "qx11grab_${lng}.qm")
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
