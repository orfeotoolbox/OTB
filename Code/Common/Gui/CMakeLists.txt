############################################################################
# Source files of Common library

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_Gui_SOURCES
  mvdAboutDialog.cxx
  mvdAbstractDragAndDropEventFilter.cxx
  mvdAbstractModelController.cxx
  mvdApplicationsToolBox.cxx
  mvdColorBandDynamicsWidget.cxx
  mvdColorDynamicsController.cxx
  mvdColorDynamicsWidget.cxx
  mvdColorSetupController.cxx
  mvdColorSetupWidget.cxx
  mvdDatabaseBrowserController.cxx
  mvdDatabaseBrowserWidget.cxx
  mvdDatabaseBrowserWidgetTest.cxx
  mvdDatabaseTreeWidget.cxx
  mvdDatasetCreationProgressDialog.cxx
  mvdDatasetDragAndDropEventFilter.cxx
  mvdDatasetPropertiesController.cxx
  mvdDatasetPropertiesWidget.cxx
  mvdDoubleValidator.cxx
  mvdDropLineEdit.cxx
  mvdFilenameDragAndDropEventFilter.cxx
  mvdGui.cxx
  mvdHistogramController.cxx
  mvdHistogramPlotPicker.cxx
  mvdHistogramWidget.cxx
  mvdI18nApplication.cxx
  mvdI18nMainWindow.cxx
  mvdImageViewManipulator.cxx
  mvdImageViewRenderer.cxx
  mvdImageViewWidget.cxx
  mvdLayerStackController.cxx
  mvdLayerStackItemModel.cxx
  mvdLayerStackWidget.cxx
  mvdMainWindowTitleLoader.cxx
  #mvdMyWidget.cxx
  mvdPixelDescriptionWidget.cxx
  mvdQuicklookViewManipulator.cxx
  mvdQuicklookViewRenderer.cxx
  mvdSearchableTreeWidget.cxx
  mvdStatusBarWidget.cxx
  mvdTaskProgressDialog.cxx
  mvdTreeWidget.cxx
  mvdTreeWidgetItem.cxx
  mvdTreeWidgetItemDragAndDropEventFilter.cxx
)

#############################################################################
# Qt's MOC header files for Common library (QObject derived classes).

# Package GUI (files ordered by lexicographical order i.e. ease of search).
set( Common_Gui_HEADERS_MOC
  mvdAboutDialog.h
  mvdAbstractDragAndDropEventFilter.h
  mvdAbstractImageViewManipulator.h
  mvdAbstractImageViewRenderer.h
  mvdAbstractModelController.h
  mvdApplicationsToolBox.h
  mvdColorBandDynamicsWidget.h
  mvdColorDynamicsController.h
  mvdColorDynamicsWidget.h
  mvdColorSetupController.h
  mvdColorSetupWidget.h
  mvdDatabaseBrowserController.h
  mvdDatabaseBrowserWidget.h
  mvdDatabaseBrowserWidgetTest.h
  mvdDatabaseTreeWidget.h
  mvdDatasetCreationProgressDialog.h
  mvdDatasetDragAndDropEventFilter.h
  mvdDatasetPropertiesController.h
  mvdDatasetPropertiesWidget.h
  mvdDoubleValidator.h
  mvdDropLineEdit.h
  mvdFilenameDragAndDropEventFilter.h
  #mvdGui.h
  mvdHistogramController.h
  mvdHistogramPlotPicker.h
  mvdHistogramWidget.h
  mvdI18nApplication.h
  mvdI18nMainWindow.h
  mvdImageViewManipulator.h
  mvdImageViewRenderer.h
  mvdImageViewWidget.h
  mvdLayerStackController.h
  mvdLayerStackItemModel.h
  mvdLayerStackWidget.h
  mvdMainWindowTitleLoader.h
  #mvdMyWidget.h
  mvdPixelDescriptionWidget.h
  mvdQuicklookViewManipulator.h
  mvdQuicklookViewRenderer.h
  mvdSearchableTreeWidget.h
  mvdStatusBarWidget.h
  mvdTaskProgressDialog.h
  mvdTreeWidget.h
  #mvdTreeWidgetItem.h
  mvdTreeWidgetItemDragAndDropEventFilter.h
)

#############################################################################
# Qt designer UI files
set( Common_Gui_FORMS
  mvdAboutDialog.ui
  mvdApplicationsToolBox.ui
  mvdColorBandDynamicsWidget.ui
  mvdColorDynamicsWidget.ui
  mvdColorSetupWidget.ui
  mvdDatabaseBrowserWidget.ui
  mvdDatabaseBrowserWidgetTest.ui
  mvdDatasetCreationProgressDialog.ui
  mvdDatasetPropertiesWidget.ui
  mvdHistogramWidget.ui
  mvdLayerStackWidget.ui
  #mvdMyWidget.ui
  mvdPixelDescriptionWidget.ui
  mvdSearchableTreeWidget.ui
  mvdStatusBarWidget.ui
  mvdVideoColorToolBox.ui
)

# Qt resources
set( Common_Gui_RESOURCES
  # mvdApplicationsToolBox.qrc
  # mvdDatabaseBrowserController.qrc
  # mvdIcons.qrc
)

include_directories(
  ${ICE_INCLUDE_DIRS}
  #
  ${Monteverdi2_BINARY_DIR}
  #
  ${Monteverdi2_SOURCE_DIR}/Code/Common
  #${Monteverdi2_SOURCE_DIR}/Code/Common/Gui
  #
  ${Monteverdi2_BINARY_DIR}/Code/Common
  ${Monteverdi2_BINARY_DIR}/Code/Common/Gui
  #
  ${QWT_INCLUDE_DIRS}
)

# message( "INCLUDE_DIRECTORIES: ${INCLUDE_DIRECTORIES}" )
# message( "ICE_INCLUDE_DIRS: ${ICE_INCLUDE_DIRS}" )
# message( "ICE_LIBRARIES: ${ICE_LIBRARIES}" )

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
  ${ICE_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  #Qt4::QtCore
  Qt4::QtGui
  Qt4::QtOpenGL
  ${QWT_LIBRARIES}
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
