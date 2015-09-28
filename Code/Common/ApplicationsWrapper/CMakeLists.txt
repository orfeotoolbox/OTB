#############################################################################
# Source files of Common library

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_ApplicationsWrapper_SOURCES
  mvdApplicationsBrowser.cxx
  mvdApplicationLauncher.cxx
  mvdWrapperQtWidgetView.cxx
  mvdApplicationsToolBoxController.cxx
  mvdOTBApplicationsModel.cxx
)

#############################################################################
# Qt's MOC header files for Common library (QObject derived classes).

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_ApplicationsWrapper_HEADERS_MOC
  mvdApplicationsBrowser.h
  mvdApplicationLauncher.h
  mvdWrapperQtWidgetView.h
  # mvdWrapperQtWidgetParameterInitializers.h
  mvdApplicationsToolBoxController.h
  mvdOTBApplicationsModel.h
)

#############################################################################

include_directories(
  ${Monteverdi_BINARY_DIR}
  #
  ${Monteverdi_SOURCE_DIR}/Code/Common
  #${Monteverdi_SOURCE_DIR}/Code/Common/ApplicationsWrapper
  #
  ${Monteverdi_BINARY_DIR}/Code/Common
  ${Monteverdi_BINARY_DIR}/Code/Common/ApplicationsWrapper
)

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Common_ApplicationsWrapper_SRC_MOC ${Common_ApplicationsWrapper_HEADERS_MOC} )

########################################################################
# Qt4 translations.

# Sources files to translate.
add_to_qt4_i18n_sources( 
  ${Common_ApplicationsWrapper_SOURCES}
  ${Common_ApplicationsWrapper_HEADERS_MOC}
  ${Common_ApplicationsWrapper_FORMS}
)

#############################################################################
add_library(Monteverdi_ApplicationsWrapper
  ${Common_ApplicationsWrapper_SOURCES}
  ${Common_ApplicationsWrapper_FORMS_HEADERS}
  ${Common_ApplicationsWrapper_SRC_MOC}
  ${Common_ApplicationsWrapper_RESOURCES_RCC}
)

#############################################################################
target_link_libraries(Monteverdi_ApplicationsWrapper
  Monteverdi_Core
  Monteverdi_Gui
  ${OTB_LIBRARIES}
  ${OPENGL_LIBRARIES}
  Qt4::QtGui
  Qt4::QtOpenGL # Why OpenGL?
)

set_target_properties(Monteverdi_ApplicationsWrapper
  PROPERTIES ${Monteverdi_LIBRARY_PROPERTIES}
)

#############################################################################
# INSTALL Common application.
install(
  TARGETS Monteverdi_ApplicationsWrapper
  RUNTIME DESTINATION ${Monteverdi_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
