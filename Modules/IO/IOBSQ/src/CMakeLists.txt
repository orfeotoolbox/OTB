set(OTBIOBSQ_SRC
  otbBSQImageIOFactory.cxx
  otbBSQImageIO.cxx
  )

add_library(OTBIOBSQ ${OTBIOBSQ_SRC})
target_link_libraries(OTBIOBSQ 
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBIOBSQ)
