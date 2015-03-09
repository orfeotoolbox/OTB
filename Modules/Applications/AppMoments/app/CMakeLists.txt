set(OTBAppMoments_LINK_LIBS
  ${OTBMoments_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
)

otb_create_application(
  NAME           LocalStatisticExtraction
  SOURCES        otbLocalStatisticExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
