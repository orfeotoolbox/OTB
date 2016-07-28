set( MonteverdiGui_SRCS
  mvdColorSetupController.cxx         
  mvdI18nApplication.cxx     
  mvdLayerStackItemModel.cxx       
  mvdShaderWidget.cxx
  mvdColorSetupWidget.cxx  
  mvdI18nMainWindow.cxx 
  mvdLayerStackWidget.cxx   
  mvdStatusBarWidget.cxx
  mvdAboutDialog.cxx           
  mvdDoubleValidator.cxx   
  mvdImageViewManipulator.cxx 
  mvdMainWindowTitleLoader.cxx     
  mvdTaskProgressDialog.cxx
  mvdAbstractDragAndDropEventFilter.cxx
  mvdDropLineEdit.cxx                
  mvdImageViewRenderer.cxx     
  mvdMultiResolutionPyramidWidget.cxx
  mvdTreeWidget.cxx
  mvdAbstractModelController.cxx   
  mvdFilenameDragAndDropEventFilter.cxx
  mvdImageViewWidget.cxx      
  mvdMyWidget.cxx             
  mvdTreeWidgetItem.cxx
  mvdApplicationsToolBox.cxx   
  mvdGui.cxx         
  mvdImportImagesDialog.cxx 
  mvdPixelDescriptionWidget.cxx    
  mvdTreeWidgetItemDragAndDropEventFilter.cxx
  mvdColorBandDynamicsWidget.cxx  
  mvdHistogramController.cxx  
  mvdImportSubDatasetDialog.cxx 
  mvdQuicklookViewManipulator.cxx
  mvdColorDynamicsController.cxx    
  mvdHistogramPlotPicker.cxx   
  mvdKeymapDialog.cxx    
  mvdQuicklookViewRenderer.cxx
  mvdColorDynamicsWidget.cxx         
  mvdHistogramWidget.cxx         
  mvdLayerStackController.cxx 
  mvdSearchableTreeWidget.cxx 
 )

add_library( MonteverdiGui ${MonteverdiGui_SRCS} )

target_link_libraries( MonteverdiGui
  ${OTBCommon_LIBRARIES}
  )

otb_module_target( MonteverdiGui )
