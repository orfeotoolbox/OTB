set(OTBGdalAdapters_SRC
  otbOGRFieldWrapper.cxx
  otbGeometriesSet.cxx
  otbOGRFeatureWrapper.cxx
  otbOGRLayerWrapper.cxx
  otbOGRGeometryWrapper.cxx
  otbGeometriesSource.cxx
  otbOGRDriversInit.cxx
  otbGeometriesToGeometriesFilter.cxx
  otbOGRDataSourceWrapper.cxx
  )

add_library(OTBGdalAdapters ${OTBGdalAdapters_SRC})
target_link_libraries(OTBGdalAdapters 
  ${OTBBoost_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}

  )

otb_module_target(OTBGdalAdapters)
