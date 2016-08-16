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

set( OTBMonteverdiCore_HEADERS_MOC
  ../include/mvdAbstractImageModel.h
  ../include/mvdAbstractLayerModel.h
  ../include/mvdAbstractModel.h
  ../include/mvdAbstractWorker.h
  # ../include/mvdAlgorithm.h
  ../include/mvdBackgroundTask.h
  # ../include/mvdCore.h
  # ../include/mvdDatabaseConnection.h
  # ../include/mvdDatabaseError.h
  # ../include/mvdDatabaseModel.h
  # ../include/mvdDatabaseQueries.h
  # ../include/mvdDatasetDescriptor.h
  # ../include/mvdDatasetModel.h
  ../include/mvdHistogramModel.h
  ../include/mvdI18nCoreApplication.h
  ../include/mvdImageImporter.h
  ../include/mvdImagePlacenameLoader.h
  # ../include/mvdImageProperties.h
  # ../include/mvdMath.h
  ../include/mvdMyClass.h
  ../include/mvdOverviewBuilder.h
  ../include/mvdQuicklookModel.h
  ../include/mvdStackedLayerModel.h
  # ../include/mvdSystemError.h
  # ../include/mvdTypes.h
  ../include/mvdVectorImageModel.h
)

#############################################################################
if( OTBApplicationEngine_ENABLED )
  list( APPEND OTBMonteverdiCore_SRCS
    mvdApplicationsBrowser.cxx
  )
  list( APPEND OTBMonteverdiCore_HEADERS_MOC
    ../include/mvdApplicationsBrowser.h
  )
endif()

#############################################################################

qt4_wrap_cpp( OTBMonteverdiCore_SRC_MOC ${OTBMonteverdiCore_HEADERS_MOC} )

add_library( OTBMonteverdiCore ${OTBMonteverdiCore_SRCS} ${OTBMonteverdiCore_SRC_MOC})

#############################################################################
target_link_libraries( OTBMonteverdiCore
  ${OTBApplicationEngine_LIBRARIES}
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
