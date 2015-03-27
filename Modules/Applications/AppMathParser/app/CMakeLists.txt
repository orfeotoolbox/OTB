set(OTBAppMathParser_LINK_LIBS
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBMathParser_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  )


otb_create_application(
  NAME           BandMath
  SOURCES        otbBandMath.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
