#############################################################################
# Source files of Common library

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_Gui_SOURCES
  mvdAboutDialog.cxx
  mvdAbstractModelController.cxx
  #mvdAbstractModelRenderer.cxx
  mvdAbstractViewManipulator.cxx
  mvdApplicationsBrowser.cxx
  mvdApplicationsToolBox.cxx
  mvdColorBandDynamicsWidget.cxx
  mvdColorDynamicsController.cxx
  mvdColorDynamicsWidget.cxx
  mvdColorSetupController.cxx
  mvdColorSetupWidget.cxx
  mvdDatasetCreationProgressDialog.cxx
  mvdGLImageWidget.cxx
  mvdGui.cxx
  mvdImageModelRenderer.cxx
  mvdImageViewManipulator.cxx
  mvdMainWindowTitleLoader.cxx
  mvdMyWidget.cxx
  mvdQuicklookViewManipulator.cxx
  mvdStatusBarWidget.cxx
)

#############################################################################
# Qt's MOC header files for Common library (QObject derived classes).

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_Gui_HEADERS_MOC
  mvdAboutDialog.h
  mvdAbstractModelController.h
  mvdAbstractModelRenderer.h
  mvdAbstractViewManipulator.h
  mvdApplicationsBrowser.h
  mvdApplicationsToolBox.h
  mvdColorBandDynamicsWidget.h
  mvdColorDynamicsController.h
  mvdColorDynamicsWidget.h
  mvdColorSetupController.h
  mvdColorSetupWidget.h
  mvdDatasetCreationProgressDialog.h
  mvdGLImageWidget.h
  mvdGui.h
  mvdImageModelRenderer.h
  mvdImageViewManipulator.h
  mvdMainWindowTitleLoader.h
  mvdMyWidget.h
  mvdQuicklookViewManipulator.h
  mvdStatusBarWidget.h
)

#############################################################################
# Qt designer UI files
set( Common_Gui_FORMS
  mvdAboutDialog.ui
  mvdColorBandDynamicsWidget.ui
  mvdColorDynamicsWidget.ui
  mvdColorSetupWidget.ui
  mvdDatasetCreationProgressDialog.ui
  mvdMyWidget.ui
  mvdStatusBarWidget.ui
  mvdVideoColorToolBox.ui
  mvdApplicationsToolBox.ui
)

# Qt resources
set( Common_Gui_RESOURCES
)

include_directories(
  ${Monteverdi2_BINARY_DIR}
  #
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  #
  ${Monteverdi2_BINARY_DIR}/Code/Common
)

#############################################################################
# Qt4 wrapped files.
qt4_wrap_cpp( Common_Gui_SRC_MOC ${Common_Gui_HEADERS_MOC} )
qt4_wrap_ui( Common_Gui_FORMS_HEADERS ${Common_Gui_FORMS}  )
qt4_add_resources( Common_Gui_RESOURCES_RCC ${Common_Gui_RESOURCES} )

########################################################################
# Qt4 translations.

# Sources files to translate.
add_to_qt4_i18n_sources( 
  ${Common_Gui_SOURCES}
  ${Common_Gui_HEADERS_MOC}
  ${Common_Gui_FORMS}
)

#############################################################################
add_library(Monteverdi2_Gui
  ${Common_Gui_SOURCES}
  ${Common_Gui_FORMS_HEADERS}
  ${Common_Gui_SRC_MOC}
  ${Common_Gui_RESOURCES_RCC}
)

#############################################################################
target_link_libraries(Monteverdi2_Gui
  Monteverdi2_Core
  OTBApplicationEngine
  ${OPENGL_LIBRARIES}
  #Qt4::QtCore
  Qt4::QtGui
  Qt4::QtOpenGL
)

set_target_properties(Monteverdi2_Gui
  PROPERTIES ${Monteverdi2_LIBRARY_PROPERTIES}
)

#############################################################################
# INSTALL Common application.
install(
  TARGETS Monteverdi2_Gui
  RUNTIME DESTINATION ${Monteverdi2_INSTALL_BIN_DIR} COMPONENT Runtime
  LIBRARY DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Runtime
  ARCHIVE DESTINATION ${Monteverdi2_INSTALL_LIB_DIR} COMPONENT Development
)

#############################################################################
