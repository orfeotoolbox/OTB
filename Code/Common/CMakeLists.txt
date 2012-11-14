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

#############################################################################
add_library(Monteverdi2_Common
  ${Common_SOURCES}
  ${Common_FORMS_HEADERS}
  ${Common_HEADERS_MOC}
  ${Common_RESOURCES_RCC}
)

#############################################################################
target_link_libraries(Monteverdi2_Common  OTBIO OTBVisualization
  ${QT_LIBRARIES}
)

#############################################################################
# INSTALL Common application.
if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(
    TARGETS Monteverdi2_Common
    #RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR_CM24} COMPONENT Runtime
    LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR_CM24} COMPONENT Runtime
    #ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR_CM24} COMPONENT Development
)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

#############################################################################
