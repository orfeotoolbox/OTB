set(OTBPolarimetry_SRC
  otbPolarimetricData.cxx
  )

add_library(OTBPolarimetry ${OTBPolarimetry_SRC})
target_link_libraries(OTBPolarimetry 
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBPolarimetry)
