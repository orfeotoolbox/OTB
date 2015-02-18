set(OTBProjection_SRC
  otbGeometriesProjectionFilter.cxx
  otbPleiadesPToXSAffineTransformCalculator.cxx
  )

add_library(OTBProjection ${OTBProjection_SRC})
target_link_libraries(OTBProjection 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBMetadata_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBCommon_LIBRARIES}

  )

otb_module_target(OTBProjection)
