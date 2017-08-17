set(OTBAppStereo_LINK_LIBS
  ${OTBStereo_LIBRARIES}
  ${OTBStatistics_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBInterpolation_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBDisplacementField_LIBRARIES}
  ${OTBDisparityMap_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBMathParser_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
)

otb_create_application(
  NAME           CVF
  SOURCES        otbApplicationCVF.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})


