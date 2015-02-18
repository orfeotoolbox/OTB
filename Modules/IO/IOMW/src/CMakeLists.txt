set(OTBIOMW_SRC
  otbMWImageIOFactory.cxx
  otbMWImageIO.cxx
  )

add_library(OTBIOMW ${OTBIOMW_SRC})
target_link_libraries(OTBIOMW 
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBIOMW)
