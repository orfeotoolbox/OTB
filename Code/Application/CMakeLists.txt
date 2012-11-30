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
  #${Application_HEADERS_MOC}
  #${Application_FORMS_HEADERS}
)

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
install(
  TARGETS monteverdi2
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
