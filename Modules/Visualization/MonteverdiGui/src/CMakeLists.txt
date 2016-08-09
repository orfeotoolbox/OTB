set( OTBMonteverdiGUI_SRCS
  mvdAboutDialog.cxx
  mvdAbstractDragAndDropEventFilter.cxx
  mvdAbstractModelController.cxx
  mvdApplicationLauncher.cxx
  mvdApplicationsToolBox.cxx
  mvdApplicationsToolBoxController.cxx
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

add_library( OTBMonteverdiGUI ${OTBMonteverdiGUI_SRCS} )

target_link_libraries( OTBMonteverdiGUI
  ${OTBCommon_LIBRARIES}
  ${OTBIce_LIBRARIES}
  ${OTBIOGDAL_LIBRARIES}
  ${OTBMonteverdiCore_LIBRARIES}
  ${OTBQwt_LIBRARIES}
  )

otb_module_target( OTBMonteverdiGUI )
