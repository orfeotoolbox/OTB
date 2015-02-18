set(OTBCarto_SRC
  otbCoordinateToName.cxx
  otbOSMDataToVectorDataGenerator.cxx
  otbPlaceNameToLonLat.cxx
  )

add_library(OTBCarto ${OTBCarto_SRC})
target_link_libraries(OTBCarto 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBCurlAdapters_LIBRARIES}
  ${OTBVectorDataIO_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}

  )

otb_module_target(OTBCarto)
