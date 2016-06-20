set(OTBAppShark_LINK_LIBS
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBConversion_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBColorMap_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
  ${OTBVectorDataIO_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBIndices_LIBRARIES}
  ${OTBMathParser_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBIOXML_LIBRARIES}
  ${OTBVectorDataManipulation_LIBRARIES}
  ${OTBStreaming_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  )

# otb_create_application(
#   NAME           SharkTrainImagesClassifier
#   SOURCES        otbSharkTrainImagesClassifier.cxx
#   LINK_LIBRARIES ${${otb-module}_LIBRARIES} ${OTBAppShark_LINK_LIBS})
