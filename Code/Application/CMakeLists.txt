#############################################################################
# Input files.
file( GLOB Application_SOURCES "*.cxx" )
file( GLOB Application_HEADERS "*.h" )
file( GLOB Application_FORMS "*.ui" )
file( GLOB Application_RESOURCES "*.qrc" )

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Application_HEADERS_MOC ${Application_HEADERS} )
qt4_wrap_ui( Application_FORMS_HEADERS ${Application_FORMS}  )
qt4_add_resources( Application_RESOURCES_RCC ${Application_RESOURCES} )

#############################################################################
# Qt4 translations.

# Sources files to translate.
add_to_qt4_i18n_sources( 
  ${Application_SOURCES}
  ${Application_HEADERS}
  ${Application_FORMS}
  ${Application_HEADERS_MOC}
  ${Application_FORMS_HEADERS}
)

# set( Application_I18N_SOURCES
#   ${Application_SOURCES}
#   ${Application_HEADERS}
#   ${Application_FORMS}
#   ${Application_HEADERS_MOC}
#   ${Application_FORMS_HEADERS}
# )

# # Locale human-readable translation files.
# set( Application_TS_TRANSLATIONS
#   i18n/fr_FR.UTF-8.ts
# )

# # Sources to translate.
# qt4_create_translation( Application_QM_TRANSLATIONS
#   ${Application_I18N_SOURCES}
#   ${Application_TS_TRANSLATIONS}
# )

# # Locale binary translation files.
# qt4_add_translation( Application_QM_TRANSLATIONS
#   ${Application_TS_TRANSLATIONS}
# )

#############################################################################
add_executable(monteverdi2
  ${Application_SOURCES}
  ${Application_FORMS_HEADERS}
  ${Application_HEADERS_MOC}
  ${Application_RESOURCES_RCC}
  ${Application_QM_TRANSLATIONS}
)

#############################################################################
target_link_libraries(monteverdi2 Monteverdi2_Common
  ${QT_LIBRARIES}
)

#############################################################################
# INSTALL Application application.
if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(
    TARGETS monteverdi2
    RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR_CM24} COMPONENT Runtime
    #LIBRARY DESTINATION ${Application_INSTALL_LIB_DIR_CM24} COMPONENT Runtime
    #ARCHIVE DESTINATION ${Application_INSTALL_LIB_DIR_CM24} COMPONENT Development
)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

#############################################################################
