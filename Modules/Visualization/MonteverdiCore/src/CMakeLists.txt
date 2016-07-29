set( MonteverdiCore_SRCS
  mvdAbstractWorker.cxx
  mvdApplicationsBrowser.cxx
  mvdAbstractModel.cxx
  mvdAbstractImageModel.cxx
  mvdAbstractLayerModel.cxx
  mvdBackgroundTask.cxx
  mvdCore.cxx
  mvdDataStream.cxx
  mvdFilenameInterface.cxx
  mvdHistogramModel.cxx
  mvdI18nCoreApplication.cxx
  mvdImageImporter.cxx
  mvdImagePlacenameLoader.cxx
  mvdImageProperties.cxx
  mvdImageSettings.cxx
  mvdImageSettingsInterface.cxx
  mvdModifiableInterface.cxx
  mvdMyClass.cxx
  mvdOverviewBuilder.cxx
  mvdProcessObjectObserver.cxx
  mvdProgressInterface.cxx
  mvdQuicklookModel.cxx
  mvdSerializableInterface.cxx
  mvdStackedLayerModel.cxx
  mvdTextStream.cxx
  mvdTypes.cxx
  mvdVectorImageModel.cxx
  mvdVectorImageSettings.cxx
  mvdVisibleInterface.cxx
  )

add_library( MonteverdiCore ${MonteverdiCore_SRCS} )

target_link_libraries( MonteverdiCore
  ${OTBCommon_LIBRARIES}
  )

otb_module_target( MonteverdiCore )
