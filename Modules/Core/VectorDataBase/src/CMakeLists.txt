set(OTBVectorDataBase_SRC
  otbVectorDataIOBase.cxx
  otbVectorDataKeywordlist.cxx
  )

add_library(OTBVectorDataBase ${OTBVectorDataBase_SRC})
target_link_libraries(OTBVectorDataBase 
  ${OTBCommon_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBGDAL_LIBRARIES}

  )

otb_module_target(OTBVectorDataBase)
