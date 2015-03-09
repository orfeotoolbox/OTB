set(OTBIOLUM_SRC
  otbLUMImageIOFactory.cxx
  otbLUMImageIO.cxx
  )

add_library(OTBIOLUM ${OTBIOLUM_SRC})
target_link_libraries(OTBIOLUM 
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBIOLUM)
