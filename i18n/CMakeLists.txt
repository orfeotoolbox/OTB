#############################################################################
# Qt4 translations.
#----------------------------------------------------------------------------
# Locale human-readable translation files.
set( Monteverdi2_TS_TRANSLATIONS
  fr_FR.ts
)
#
# Configuration option to enable creation/merging of source .ts files
# Default value is: OFF.
option( MERGE_TS_TRANSLATIONS "Create/merge .ts source translation files. WARNING: 'make clean' will delete the source .ts files!.")
#
# Sources to translate.
if( MERGE_TS_TRANSLATIONS )
  qt4_create_translation( Monteverdi2_QM_TRANSLATIONS
    ${i18n_SOURCES}
    ${Monteverdi2_TS_TRANSLATIONS}
    )
endif( MERGE_TS_TRANSLATIONS )
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
# if( NOT OTB_INSTALL_NO_LIBRARIES )
#   install(
#     TARGETS i18n
#     FILES DESTINATION ${}
#     )
# endif( NOT OTB_INSTALL_NO_LIBRARIES )
