#############################################################################
set( OTBMonteverdiGUI_SRCS
  mvdAboutDialog.cxx
  mvdAbstractDragAndDropEventFilter.cxx
  mvdAbstractModelController.cxx
  mvdColorBandDynamicsWidget.cxx
  mvdColorDynamicsController.cxx
  mvdColorDynamicsWidget.cxx
  mvdColorSetupController.cxx
  mvdColorSetupWidget.cxx
  mvdDoubleValidator.cxx
  mvdDropLineEdit.cxx
  mvdFilenameDragAndDropEventFilter.cxx
  mvdGui.cxx
  mvdHistogramController.cxx
  mvdHistogramPlotPicker.cxx
  mvdHistogramWidget.cxx
  mvdI18nApplication.cxx
  mvdI18nMainWindow.cxx
  mvdImageViewRenderer.cxx
  mvdImageViewManipulator.cxx
  mvdImageViewWidget.cxx
  mvdImportImagesDialog.cxx
  mvdImportSubDatasetDialog.cxx
  mvdKeymapDialog.cxx
  mvdLayerStackController.cxx
  mvdLayerStackItemModel.cxx
  mvdLayerStackWidget.cxx
  mvdMainWindowTitleLoader.cxx
  mvdMultiResolutionPyramidWidget.cxx
  mvdMyWidget.cxx
  mvdPixelDescriptionWidget.cxx
  mvdQuicklookViewManipulator.cxx
  mvdQuicklookViewRenderer.cxx
  mvdSearchableTreeWidget.cxx
  mvdShaderWidget.cxx
  mvdStatusBarWidget.cxx
  mvdTaskProgressDialog.cxx
  mvdTreeWidget.cxx
  mvdTreeWidgetItem.cxx
  mvdTreeWidgetItemDragAndDropEventFilter.cxx
 )

#############################################################################
set( OTBMonteverdiGUI_HEADERS_MOC
  mvdAboutDialog.h
  mvdAbstractDragAndDropEventFilter.h
  mvdAbstractImageViewManipulator.h
  mvdAbstractImageViewRenderer.h
  mvdAbstractModelController.h
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
  mvdImportImagesDialog.h
  mvdImportSubDatasetDialog.h
  mvdKeymapDialog.h
  mvdLayerStackController.h
  mvdLayerStackItemModel.h
  mvdLayerStackWidget.h
  mvdMainWindowTitleLoader.h
  mvdMultiResolutionPyramidWidget.h
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
  mvdShaderWidget.h
)

#############################################################################
set( OTBMonteverdiGUI_FORMS
  mvdAboutDialog.ui
  mvdColorBandDynamicsWidget.ui
  mvdColorDynamicsWidget.ui
  mvdColorSetupWidget.ui
  mvdDatabaseBrowserWidget.ui
  mvdDatabaseBrowserWidgetTest.ui
  mvdDatasetCreationProgressDialog.ui
  mvdDatasetPropertiesWidget.ui
  mvdHistogramWidget.ui
  mvdImportImagesDialog.ui
  mvdImportSubDatasetDialog.ui
  mvdKeymapDialog.ui
  mvdLayerStackWidget.ui
  mvdMultiResolutionPyramidWidget.ui
  #mvdMyWidget.ui
  mvdPixelDescriptionWidget.ui
  mvdSearchableTreeWidget.ui
  mvdShaderWidget.ui
  mvdStatusBarWidget.ui
  mvdVideoColorToolBox.ui
)

#############################################################################
set( OTBMonteverdiGUI_RESOURCES
  # mvdIcons.qrc
)

#############################################################################
if( OTBQtWidget )
  list( APPEND OTBMonteverdiGUI_SRCS
    mvdApplicationLauncher.cxx
    mvdApplicationsToolBox.cxx
    mvdApplicationsToolBoxController.cxx
  )

  list( APPEND OTBMonteverdiGUI_HEADERS_MOC
    mvdApplicationsToolBox.h
    )

  list( APPEND OTBMonteverdiGUI_FORMS
    mvdApplicationsToolBox.ui
    )

  list( APPEND OTBMonteverdiGUI_RESOURCES
    # mvdApplicationsToolBox.qrc
    )
endif()

#############################################################################
qt4_wrap_cpp( OTBMonteverdiGUI_SRC_MOC ${OTBMonteverdiGUI_HEADERS_MOC} )
qt4_wrap_ui( OTBMonteverdiGUI_FORMS_HEADERS ${OTBMonteverdiGUI_FORMS} )
qt4_add_resources( OTBMonteverdiGUI_RESOURCES_RCC ${OTBMonteverdiGUI_RESOURCES} )

#############################################################################
add_library( OTBMonteverdiGUI ${OTBMonteverdiGUI_SRCS} )

#############################################################################
target_link_libraries( OTBMonteverdiGUI
  ${OTBCommon_LIBRARIES}
  ${OTBIce_LIBRARIES}
  ${OTBIOGDAL_LIBRARIES}
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBQwt_LIBRARIES}
  )

#############################################################################
otb_module_target( OTBMonteverdiGUI )
