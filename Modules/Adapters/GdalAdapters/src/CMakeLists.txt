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

if(OTB_USE_GDAL_20)
  set(OTBGdalAdapters_SRC ${OTBGdalAdapters_SRC} otbOGRVersionProxy2x.cxx)
else(Otb_use_gdal_20)
  set(OTBGdalAdapters_SRC ${OTBGdalAdapters_SRC} otbOGRVersionProxy1x.cxx)
endif(OTB_USE_GDAL_20)

add_library(OTBGdalAdapters ${OTBGdalAdapters_SRC})
target_link_libraries(OTBGdalAdapters 
  ${OTBBoost_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}

  )

otb_module_target(OTBGdalAdapters)
