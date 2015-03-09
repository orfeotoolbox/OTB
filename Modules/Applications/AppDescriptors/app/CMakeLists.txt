set(OTBAppDescriptors_LINK_LIBS
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBDescriptors_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
)

otb_create_application(
  NAME           HomologousPointsExtraction
  SOURCES        otbHomologousPointsExtraction.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
