#############################################################################
# Input files.
set( Application_SOURCES
       mvdApplication.cxx
       mvdImageModel.cxx
       mvdMainWindow.cxx
)
set( Application_HEADERS
       mvdApplication.h
       mvdImageModel.h
       mvdMainWindow.h
)
set( Application_FORMS
       mvdMainWindow.ui
)
set( Application_RESOURCES
)

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Application_HEADERS_MOC ${Application_HEADERS} )
qt4_wrap_ui( Application_FORMS_HEADERS ${Application_FORMS}  )
qt4_add_resources( Application_RESOURCES_RCC ${Application_RESOURCES} )

#############################################################################
# Sources files to translate.
add_to_qt4_i18n_sources(
  ${Application_SOURCES}
  ${Application_HEADERS}
  ${Application_FORMS}
)

#############################################################################
# Monteverdi2_Application library
# STATIC so that the monteverdi2 executable does not depend on another shared lib
# The library is also used in tests, waiting for a better test strategy
# The library is not installed

set(Monteverdi2_Application_SOURCES ${Application_SOURCES})
add_library(Monteverdi2_Application
  STATIC
  ${Monteverdi2_Application_SOURCES}
  ${Application_FORMS_HEADERS}
  ${Application_HEADERS_MOC}
  ${Application_RESOURCES_RCC}
)
target_link_libraries(Monteverdi2_Application
  Monteverdi2_Common
  ${QT_LIBRARIES}
)

#############################################################################
# monteverdi2 executable
add_executable(monteverdi2
  WIN32
  main.cxx
)
target_link_libraries(monteverdi2
  Monteverdi2_Application
  Monteverdi2_Common
  ${QT_LIBRARIES}
)


#############################################################################
install(
  TARGETS monteverdi2
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
