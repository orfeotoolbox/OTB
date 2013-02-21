#############################################################################
# Input files.
file( GLOB Common_SOURCES "*.cxx" )
file( GLOB Common_HEADERS "*.h" )
file( GLOB Common_FORMS "*.ui" )
file( GLOB Common_RESOURCES "*.qrc" )

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Common_HEADERS_MOC ${Common_HEADERS} )
qt4_wrap_ui( Common_FORMS_HEADERS ${Common_FORMS}  )
qt4_add_resources( Common_RESOURCES_RCC ${Common_RESOURCES} )

########################################################################
# Qt4 translations.

# Sources files to translate.
add_to_qt4_i18n_sources( 
  ${Common_SOURCES}
  ${Common_HEADERS}
  ${Common_FORMS}
  #${Common_HEADERS_MOC}
  #${Common_FORMS_HEADERS}
)

#############################################################################
add_library(Monteverdi2_Common
  ${Common_SOURCES}
  ${Common_FORMS_HEADERS}
  ${Common_HEADERS_MOC}
  ${Common_RESOURCES_RCC}
)

#############################################################################
target_link_libraries(Monteverdi2_Common
  OTBIO
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES}
)

#############################################################################
# INSTALL Common application.
install(
  TARGETS Monteverdi2_Common
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
