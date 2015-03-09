set(OTBAppVectorDataTranslation_LINK_LIBS
  ${OTBConversion_LIBRARIES}
  ${OTBApplicationEngine_LIBRARIES}
  ${OTBTransform_LIBRARIES}
)

otb_create_application(
  NAME           Rasterization
  SOURCES        otbRasterization.cxx
  LINK_LIBRARIES ${${otb-module}_LIBRARIES})
