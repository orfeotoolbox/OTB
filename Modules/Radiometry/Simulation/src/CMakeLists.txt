set(OTBSimulation_SRC
  otbSailModel.cxx
  otbProspectModel.cxx
  otbDataSpecP5B.cxx
  otbLeafParameters.cxx
  )

add_library(OTBSimulation ${OTBSimulation_SRC})
target_link_libraries(OTBSimulation 
  ${OTBMetadata_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBOpticalCalibration_LIBRARIES}
  ${OTBBoost_LIBRARIES}

  )

otb_module_target(OTBSimulation)
