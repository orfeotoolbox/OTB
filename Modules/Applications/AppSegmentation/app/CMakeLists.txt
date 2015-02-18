set(OTBAppSegmentation_LINK_LIBS
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBLabelMap_LIBRARIES}
  ${OTBConversion_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBWatersheds_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBSmoothing_LIBRARIES}
  ${OTBMetrics_LIBRARIES}
  ${OTBOGRProcessing_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBMeanShift_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBCCOBIA_LIBRARIES}
  ${OTBMathParser_LIBRARIES}
  ${OTBMorphologicalProfiles_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBCommon_LIBRARIES}
)

otb_create_application(
  NAME           Segmentation
  SOURCES        otbSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           ConnectedComponentSegmentation
  SOURCES        otbConnectedComponentSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           LSMSSegmentation
  SOURCES        otbLSMSSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           LSMSVectorization
  SOURCES        otbLSMSVectorization.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           LSMSSmallRegionsMerging
  SOURCES        otbLSMSSmallRegionsMerging.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           HooverCompareSegmentation
  SOURCES        otbHooverCompareSegmentation.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           MeanShiftSmoothing
  SOURCES        otbMeanShiftSmoothing.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
