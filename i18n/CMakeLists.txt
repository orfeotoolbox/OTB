#############################################################################
# Qt4 translations.
#----------------------------------------------------------------------------
# Locale human-readable translation files.
set( OTB_TS_TRANSLATIONS
# en_US.ts
  fr_FR.ts
)
unset(OTB_QM_TRANSLATIONS)

#----------------------------------------------------------------------------
# Configuration option to enable creation/merging of source .ts files
# Default value is: OFF.
option( OTB_I18N_MERGE_TS
  "Create/merge .ts source translation files. WARNING: If ON, 'make clean' will delete the .ts files from source directory!"
  OFF
  )

#----------------------------------------------------------------------------
if( OTBQt4_ENABLED )
  # Generate commands to build Qt translations according to option.
  if( OTB_I18N_MERGE_TS )
    message(
      AUTHOR_WARNING
      "WARNING: OTB_I18N_MERGE_TS:BOOL option is set to ON. 'make clean' will delete .ts files from source directory!"
      )

    message( STATUS "Qt I18N source files: ${OTB_QT_I18N_SOURCE_FILES}")

    set( OTB_QT_I18N_PRO "${CMAKE_CURRENT_BINARY_DIR}/i18n.pro" )

    generate_qt4_project( ${OTB_QT_I18N_PRO} )

    foreach(_ts_name ${OTB_TS_TRANSLATIONS})
      get_filename_component(_abs_ts ${_ts_name} ABSOLUTE)

      message( STATUS "Merge TS : ${_abs_ts}")
      add_custom_command(OUTPUT ${_abs_ts}
        COMMAND ${QT_LUPDATE_EXECUTABLE}
        ARGS -locations none -pro ${OTB_QT_I18N_PRO} -ts ${_abs_ts}
        DEPENDS
          ${OTB_QT_I18N_HEADER_FILES}
          ${OTB_QT_I18N_SOURCE_FILES}
          ${OTB_QT_I18N_FORM_FILES}
          ${OTB_QT_I18N_PRO}
        VERBATIM)
    endforeach()
  endif()

  qt4_add_translation(OTB_QM_TRANSLATIONS
      ${OTB_TS_TRANSLATIONS}
      )

  # Custom build targets.
  add_custom_target( i18n ALL DEPENDS i18n_qt )
  add_custom_target( i18n_qt DEPENDS ${OTB_QM_TRANSLATIONS} )

  # Locale human-readable translation files.
  install(
    FILES ${OTB_QM_TRANSLATIONS}
    DESTINATION ${OTB_INSTALL_DATA_DIR}/i18n
    COMPONENT Resources
    )
endif()
