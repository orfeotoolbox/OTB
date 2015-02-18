set(OTBIOKML_SRC
  otbKMLVectorDataIO.cxx
  otbKMLVectorDataIOFactory.cxx
  )

add_library(OTBIOKML ${OTBIOKML_SRC})
target_link_libraries(OTBIOKML 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBlibkml_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}

  )

otb_module_target(OTBIOKML)
