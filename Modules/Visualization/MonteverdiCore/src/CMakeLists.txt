set( MonteverdiCore_SRCS
  mvdAbstractWorker.cxx
  mvdHistogramModel.cxx
  mvdImageSettings.cxx
  mvdProcessObjectObserver.cxx
  mvdTextStream.cxx
  mvdBackgroundTask.cxx
  mvdI18nCoreApplication.cxx
  mvdImageSettingsInterface.cxx
  mvdProgressInterface.cxx
  mvdTypes.cxx
  mvdAbstractImageModel.cxx
  mvdCore.cxx
  mvdImageImporter.cxx
  mvdModifiableInterface.cxx
  mvdQuicklookModel.cxx
  mvdVectorImageModel.cxx
  mvdAbstractLayerModel.cxx
  mvdDataStream.cxx
  mvdImagePlacenameLoader.cxx
  mvdMyClass.cxx
  mvdSerializableInterface.cxx
  mvdVectorImageSettings.cxx
  mvdAbstractModel.cxx
  mvdFilenameInterface.cxx
  mvdImageProperties.cxx
  mvdOverviewBuilder.cxx
  mvdStackedLayerModel.cxx
  mvdVisibleInterface.cxx
  )

add_library( MonteverdiCore ${MonteverdiCore_SRCS} )

target_link_libraries( MonteverdiCore
  ${OTBCommon_LIBRARIES}
  )

otb_module_target( MonteverdiCore )
