set(OTBVectorDataIO_SRC
  otbVectorDataIOFactory.cxx
  )

add_library(OTBVectorDataIO ${OTBVectorDataIO_SRC})
target_link_libraries(OTBVectorDataIO 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBIOGDAL_LIBRARIES}
  ${OTBIOKML_LIBRARIES}

  )

otb_module_target(OTBVectorDataIO)
