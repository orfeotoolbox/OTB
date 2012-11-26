#############################################################################
# Qt4 translations.
#----------------------------------------------------------------------------
# Locale human-readable translation files.
set( Monteverdi2_TS_TRANSLATIONS
  fr_FR.UTF-8.ts
)
#
# Sources to translate.
qt4_create_translation( Monteverdi2_QM_TRANSLATIONS
  ${i18n_SOURCES}
  ${Monteverdi2_TS_TRANSLATIONS}
)
#
# Locale binary translation files.
qt4_add_translation( Monteverdi2_QM_TRANSLATIONS
  ${Monteverdi2_TS_TRANSLATIONS}
)
#
# Custom build target.
add_custom_target( i18n DEPENDS ${Monteverdi2_QM_TRANSLATIONS} )
