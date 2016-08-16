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
  #mvdMyWidget.cxx
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
  ../include/mvdAboutDialog.h
  ../include/mvdAbstractDragAndDropEventFilter.h
  ../include/mvdAbstractImageViewManipulator.h
  ../include/mvdAbstractImageViewRenderer.h
  ../include/mvdAbstractModelController.h
  ../include/mvdColorBandDynamicsWidget.h
  ../include/mvdColorDynamicsController.h
  ../include/mvdColorDynamicsWidget.h
  ../include/mvdColorSetupController.h
  ../include/mvdColorSetupWidget.h
  #../include/mvdDatabaseBrowserController.h
  #../include/mvdDatabaseBrowserWidget.h
  #../include/mvdDatabaseBrowserWidgetTest.h
  #../include/mvdDatabaseTreeWidget.h
  #../include/mvdDatasetCreationProgressDialog.h
  #../include/mvdDatasetDragAndDropEventFilter.h
  #../include/mvdDatasetPropertiesController.h
  #../include/mvdDatasetPropertiesWidget.h
  ../include/mvdDoubleValidator.h
  ../include/mvdDropLineEdit.h
  ../include/mvdFilenameDragAndDropEventFilter.h
  #mvdGui.h
  ../include/mvdHistogramController.h
  ../include/mvdHistogramPlotPicker.h
  ../include/mvdHistogramWidget.h
  ../include/mvdI18nApplication.h
  ../include/mvdI18nMainWindow.h
  ../include/mvdImageViewManipulator.h
  ../include/mvdImageViewRenderer.h
  ../include/mvdImageViewWidget.h
  ../include/mvdImportImagesDialog.h
  ../include/mvdImportSubDatasetDialog.h
  ../include/mvdKeymapDialog.h
  ../include/mvdLayerStackController.h
  ../include/mvdLayerStackItemModel.h
  ../include/mvdLayerStackWidget.h
  ../include/mvdMainWindowTitleLoader.h
  ../include/mvdMultiResolutionPyramidWidget.h
  #mvdMyWidget.h
  ../include/mvdPixelDescriptionWidget.h
  ../include/mvdQuicklookViewManipulator.h
  ../include/mvdQuicklookViewRenderer.h
  ../include/mvdSearchableTreeWidget.h
  ../include/mvdStatusBarWidget.h
  ../include/mvdTaskProgressDialog.h
  ../include/mvdTreeWidget.h
  #mvdTreeWidgetItem.h
  ../include/mvdTreeWidgetItemDragAndDropEventFilter.h
  ../include/mvdShaderWidget.h
)

#############################################################################
set( OTBMonteverdiGUI_FORMS
  mvdAboutDialog.ui
  mvdColorBandDynamicsWidget.ui
  mvdColorDynamicsWidget.ui
  mvdColorSetupWidget.ui
  #mvdDatabaseBrowserWidget.ui
  #mvdDatabaseBrowserWidgetTest.ui
  #mvdDatasetCreationProgressDialog.ui
  #mvdDatasetPropertiesWidget.ui
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
if( OTBQtWidget_ENABLED )
  list( APPEND OTBMonteverdiGUI_SRCS
    mvdApplicationLauncher.cxx
    mvdApplicationsToolBox.cxx
    mvdApplicationsToolBoxController.cxx
    mvdOTBApplicationsModel.cxx
    mvdQtWidgetView.cxx
  )

  list( APPEND OTBMonteverdiGUI_HEADERS_MOC
    ../include/mvdApplicationLauncher.h
    ../include/mvdApplicationsToolBox.h
    ../include/mvdApplicationsToolBoxController.h
    ../include/mvdOTBApplicationsModel.h
    ../include/mvdQtWidgetView.h
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
add_library( OTBMonteverdiGUI
  ${OTBMonteverdiGUI_SRCS}
  ${OTBMonteverdiGUI_FORMS_HEADERS}
  ${OTBMonteverdiGUI_SRC_MOC}
  ${OTBMonteverdiGUI_RESOURCES_RCC}
)

#############################################################################
target_link_libraries( OTBMonteverdiGUI
  ${OTBCommon_LIBRARIES}
  ${OTBIce_LIBRARIES}
  ${OTBIOGDAL_LIBRARIES}
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBQtWidget_LIBRARIES}
  ${OTBQwt_LIBRARIES}
  )

#############################################################################
otb_module_target( OTBMonteverdiGUI )
