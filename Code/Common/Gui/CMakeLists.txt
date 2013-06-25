############################################################################
# Source files of Common library

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_Gui_SOURCES
  mvdAboutDialog.cxx
  mvdAbstractModelController.cxx
  #mvdAbstractModelRenderer.cxx
  mvdAbstractViewManipulator.cxx
  mvdCheckableLineEdit.cxx
  mvdColorBandDynamicsWidget.cxx
  mvdColorDynamicsController.cxx
  mvdColorDynamicsWidget.cxx
  mvdColorSetupController.cxx
  mvdColorSetupWidget.cxx
  mvdDatabaseBrowserController.cxx
  mvdDatabaseBrowserWidget.cxx
  mvdDatasetCreationProgressDialog.cxx
  mvdDoubleValidator.cxx
  mvdGLImageWidget.cxx
  mvdGui.cxx
  mvdI18nApplication.cxx
  mvdI18nMainWindow.cxx
  mvdImageModelRenderer.cxx
  mvdImageViewManipulator.cxx
  mvdMainWindowTitleLoader.cxx
  mvdMyWidget.cxx
  mvdQuicklookViewManipulator.cxx
  mvdStatusBarWidget.cxx
  mvdTaskProgressDialog.cxx
  mvdApplicationsToolBox.cxx
  mvdDatabaseTreeWidget.cxx
  mvdDatasetPropertiesWidget.cxx
  mvdDatasetPropertiesController.cxx
  mvdDatasetTreeWidgetItem.cxx
  mvdPixelDescriptionWidget.cxx
)

#############################################################################
# Qt's MOC header files for Common library (QObject derived classes).

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_Gui_HEADERS_MOC
  mvdAboutDialog.h
  mvdAbstractModelController.h
  mvdAbstractModelRenderer.h
  mvdAbstractViewManipulator.h
  mvdCheckableLineEdit.h
  mvdColorBandDynamicsWidget.h
  mvdColorDynamicsController.h
  mvdColorDynamicsWidget.h
  mvdColorSetupController.h
  mvdColorSetupWidget.h
  mvdDatabaseBrowserController.h
  mvdDatabaseBrowserWidget.h
  mvdDatasetCreationProgressDialog.h
  mvdDoubleValidator.h
  mvdGLImageWidget.h
  # mvdGui.h
  mvdI18nApplication.h
  mvdI18nMainWindow.h
  mvdImageModelRenderer.h
  mvdImageViewManipulator.h
  mvdMainWindowTitleLoader.h
  mvdMyWidget.h
  mvdQuicklookViewManipulator.h
  mvdStatusBarWidget.h
  mvdTaskProgressDialog.h
  mvdApplicationsToolBox.h
  mvdDatabaseTreeWidget.h
  mvdDatasetPropertiesWidget.h
  mvdDatasetPropertiesController.h
  mvdDatasetTreeWidgetItem.h
  mvdPixelDescriptionWidget.h
)

#############################################################################
# Qt designer UI files
set( Common_Gui_FORMS
  mvdAboutDialog.ui
  mvdApplicationsToolBox.ui
  mvdCheckableLineEdit.ui
  mvdColorBandDynamicsWidget.ui
  mvdColorDynamicsWidget.ui
  mvdColorSetupWidget.ui
  mvdDatabaseBrowserWidget.ui
  mvdDatasetCreationProgressDialog.ui
  mvdMyWidget.ui
  mvdStatusBarWidget.ui
  mvdVideoColorToolBox.ui
  mvdDatasetPropertiesWidget.ui
  mvdPixelDescriptionWidget.ui
)

# Qt resources
set( Common_Gui_RESOURCES
  mvdApplicationsToolBox.qrc
  mvdDatabaseBrowserController.qrc
  mvdIcons.qrc
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
