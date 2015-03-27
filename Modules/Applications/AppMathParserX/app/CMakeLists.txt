set(OTBAppMathParserX_LINK_LIBS
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBMathParserX_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  )


otb_create_application(
  NAME           BandMathX
  SOURCES        otbBandMathX.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
