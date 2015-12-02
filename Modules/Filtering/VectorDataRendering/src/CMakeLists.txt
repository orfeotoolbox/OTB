set(OTBVectorDataRendering_SRC
  otbVectorDataStyle.cxx
  )

if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties( ${CMAKE_CURRENT_SOURCE_DIR}/otbVectorDataStyle.cxx COMPILE_FLAGS "-fno-var-tracking")
endif()

add_library(OTBVectorDataRendering ${OTBVectorDataRendering_SRC})
target_link_libraries(OTBVectorDataRendering
  ${OTBMapnik_LIBRARIES}
  ${OTBVectorDataBase_LIBRARIES}

  )

otb_module_target(OTBVectorDataRendering)
