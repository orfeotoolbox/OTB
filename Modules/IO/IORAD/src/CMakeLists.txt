set(OTBIORAD_SRC
  otbRADImageIOFactory.cxx
  otbRADImageIO.cxx
  )

add_library(OTBIORAD ${OTBIORAD_SRC})
target_link_libraries(OTBIORAD 
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBIORAD)
