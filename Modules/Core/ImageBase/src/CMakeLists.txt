set(OTBImageBase_SRC
  otbImageIOBase.cxx
  otbNoDataHelper.cxx
  )

add_library(OTBImageBase ${OTBImageBase_SRC})
target_link_libraries(OTBImageBase 
  ${OTBCommon_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBMetadata_LIBRARIES}

  )

otb_module_target(OTBImageBase)
