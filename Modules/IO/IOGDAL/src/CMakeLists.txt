set(OTBIOGDAL_SRC
  otbGDALImageIOFactory.cxx
  otbGDALImageIO.cxx
  otbOGRVectorDataIO.cxx
  otbOGRIOHelper.cxx
  otbOGRVectorDataIOFactory.cxx
  otbGDALDriverManagerWrapper.cxx
  )

add_library(OTBIOGDAL ${OTBIOGDAL_SRC})
target_link_libraries(OTBIOGDAL 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}

  )

otb_module_target(OTBIOGDAL)
