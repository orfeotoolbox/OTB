#############################################################################
# Qt4 translations.
#----------------------------------------------------------------------------
# Locale human-readable translation files.
set( Monteverdi2_TS_TRANSLATIONS
# en_US.ts
  fr_FR.ts
)
#
# Configuration option to enable creation/merging of source .ts files
# Default value is: OFF.
option( MERGE_TS "Create/merge .ts source translation files. WARNING: 'make clean' will delete the source .ts files!" OFF)
#
# Sources to translate.
if( MERGE_TS )
  message( AUTHOR_WARNING "WARNING: MERGE_TS:BOOL option is ON. 'make clean' will delete .ts files from source directory!" )
  message( STATUS "I18N source files: " ${Monteverdi2_I18N_SOURCE_FILES} )
  qt4_create_translation( Monteverdi2_QM_TRANSLATIONS
    ${Monteverdi2_I18N_SOURCE_FILES}
    ${Monteverdi2_TS_TRANSLATIONS}
    )
else()
  #
  # Locale binary translation files.
  qt4_add_translation( Monteverdi2_QM_TRANSLATIONS
    ${Monteverdi2_TS_TRANSLATIONS}
  )
endif()

#
# Custom build target.
add_custom_target( i18n ALL DEPENDS ${Monteverdi2_QM_TRANSLATIONS} )
#
#----------------------------------------------------------------------------
# Locale human-readable translation files.
install(
  FILES ${Monteverdi2_QM_TRANSLATIONS}
  DESTINATION ${Monteverdi2_INSTALL_DATA_DIR}/i18n
  COMPONENT Resources
  )
