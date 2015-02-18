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
  NAME           StereoFramework
  SOURCES        otbStereoFramework.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           StereoRectificationGridGenerator
  SOURCES        otbStereoRectificationGridGenerator.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           FineRegistration
  SOURCES        otbFineRegistration.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           GeneratePlyFile
  SOURCES        otbGeneratePlyFile.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           DisparityMapToElevationMap
  SOURCES        otbDisparityMapToElevationMap.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})

otb_create_application(
  NAME           BlockMatching
  SOURCES        otbBlockMatching.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
