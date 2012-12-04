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
option( MERGE_TS "Create/merge .ts source translation files. WARNING: 'make clean' will delete the source .ts files!." OFF)
#
# Sources to translate.
if( MERGE_TS )
  #message( "Merging I18N sources/translations." )
  #message( "I18N sources: '${Monteverdi2_I18N_SOURCE_FILES}'" )
  #message( "I18N translations: '${Monteverdi2_TS_TRANSLATIONS}'" )
  qt4_create_translation( Monteverdi2_QM_TRANSLATIONS
    ${Monteverdi2_I18N_SOURCE_FILES}
    ${Monteverdi2_TS_TRANSLATIONS}
    )
endif()
#
# Locale binary translation files.
qt4_add_translation( Monteverdi2_QM_TRANSLATIONS
  ${Monteverdi2_TS_TRANSLATIONS}
)
#
# Custom build target.
add_custom_target( i18n ALL DEPENDS ${Monteverdi2_QM_TRANSLATIONS} )
#
#----------------------------------------------------------------------------
# Locale human-readable translation files.
install(
  FILES ${Monteverdi2_QM_TRANSLATIONS} DESTINATION ${Monteverdi2_INSTALL_DATA_DIR}/i18n
  )
