#############################################################################
set( OTBMonteverdiCore_SRCS
  mvdAbstractWorker.cxx
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

#############################################################################
if( OTBApplicationEngine_ENABLED )
  list( APPEND OTBMonteverdiCore_SRCS
    mvdApplicationsBrowser.cxx
  )
endif()

#############################################################################
add_library( OTBMonteverdiCore ${OTBMonteverdiCore_SRCS} )

#############################################################################
target_link_libraries( OTBMonteverdiCore
  ${OTBCarto_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBIOGDAL_LIBRARIES}
  ${OTBMetadata_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBQt4_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  )

#############################################################################
otb_module_target( OTBMonteverdiCore )
