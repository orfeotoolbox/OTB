set(OTBVectorDataRendering_SRC
  otbVectorDataStyle.cxx
  )

add_library(OTBVectorDataRendering ${OTBVectorDataRendering_SRC})
target_link_libraries(OTBVectorDataRendering 
  ${OTBMapnik_LIBRARIES}
  ${OTBVectorDataBase_LIBRARIES}

  )

otb_module_target(OTBVectorDataRendering)
