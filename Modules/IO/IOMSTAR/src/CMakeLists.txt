set(OTBIOMSTAR_SRC
  otbMSTARImageIOFactory.cxx
  otbMSTARImageIO.cxx
  )

add_library(OTBIOMSTAR ${OTBIOMSTAR_SRC})
target_link_libraries(OTBIOMSTAR 
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBIOMSTAR)
