set(OTBVectorDataBase_SRC
  otbVectorDataIOBase.cxx
  otbVectorDataKeywordlist.cxx
  )

add_library(OTBVectorDataBase ${OTBVectorDataBase_SRC})
target_link_libraries(OTBVectorDataBase
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  )

otb_module_target(OTBVectorDataBase)
