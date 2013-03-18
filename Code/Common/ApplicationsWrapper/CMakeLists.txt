#############################################################################
# Source files of Common library

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_ApplicationsWrapper_SOURCES
  mvdApplicationsBrowser.cxx
  mvdApplicationsToolBox.cxx
  mvdApplicationLauncher.cxx  
)

#############################################################################
# Qt's MOC header files for Common library (QObject derived classes).

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_ApplicationsWrapper_HEADERS_MOC
  mvdApplicationsBrowser.h
  mvdApplicationsToolBox.h
  mvdApplicationLauncher.h
)

#############################################################################
# Qt designer UI files
set( Common_ApplicationsWrapper_FORMS
  mvdApplicationsToolBox.ui
)

# Qt resources
set( Common_ApplicationsWrapper_RESOURCES
)

include_directories(
  ${Monteverdi2_BINARY_DIR}
  #
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  #
  ${Monteverdi2_BINARY_DIR}/Code/Common
  #
  ${OTB_SOURCE_DIR}/Code/Wrappers/QtWidget
)

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Common_ApplicationsWrapper_SRC_MOC ${Common_ApplicationsWrapper_HEADERS_MOC} )
qt4_wrap_ui( Common_ApplicationsWrapper_FORMS_HEADERS ${Common_ApplicationsWrapper_FORMS}  )
qt4_add_resources( Common_ApplicationsWrapper_RESOURCES_RCC ${Common_ApplicationsWrapper_RESOURCES} )

########################################################################
# Qt4 translations.

# Sources files to translate.
add_to_qt4_i18n_sources( 
  ${Common_ApplicationsWrapper_SOURCES}
  ${Common_ApplicationsWrapper_HEADERS_MOC}
  ${Common_ApplicationsWrapper_FORMS}
)

#############################################################################
add_library(Monteverdi2_ApplicationsWrapper
  ${Common_ApplicationsWrapper_SOURCES}
  ${Common_ApplicationsWrapper_FORMS_HEADERS}
  ${Common_ApplicationsWrapper_SRC_MOC}
  ${Common_ApplicationsWrapper_RESOURCES_RCC}
)

#############################################################################
target_link_libraries(Monteverdi2_ApplicationsWrapper
  #Monteverdi2_Core
  OTBApplicationEngine
  OTBWrapperQtWidget
  ${OPENGL_LIBRARIES}
  #Qt4::QtCore
  Qt4::QtGui
  Qt4::QtOpenGL
)

set_target_properties(Monteverdi2_ApplicationsWrapper
  PROPERTIES ${Monteverdi2_LIBRARY_PROPERTIES}
)

#############################################################################
# INSTALL Common application.
install(
  TARGETS Monteverdi2_ApplicationsWrapper
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
