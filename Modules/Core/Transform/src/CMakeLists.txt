set(OTBTransform_SRC
  otbGeoInformationConversion.cxx
  )

add_library(OTBTransform ${OTBTransform_SRC})
target_link_libraries(OTBTransform 
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}

  )

otb_module_target(OTBTransform)
