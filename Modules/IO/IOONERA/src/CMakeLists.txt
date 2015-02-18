set(OTBIOONERA_SRC
  otbONERAImageIOFactory.cxx
  otbONERAImageIO.cxx
  )

add_library(OTBIOONERA ${OTBIOONERA_SRC})
target_link_libraries(OTBIOONERA 
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBIOONERA)
