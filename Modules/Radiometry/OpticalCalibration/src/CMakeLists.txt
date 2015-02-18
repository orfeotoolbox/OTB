set(OTBOpticalCalibration_SRC
  otbAtmosphericCorrectionParameters.cxx
  otbAeronetData.cxx
  otbWavelengthSpectralBands.cxx
  otbSpectralSensitivityReader.cxx
  otbAeronetFileReader.cxx
  otbSIXSTraits.cxx
  otbAtmosphericRadiativeTerms.cxx
  otbImageMetadataCorrectionParameters.cxx
  )

add_library(OTBOpticalCalibration ${OTBOpticalCalibration_SRC})
target_link_libraries(OTBOpticalCalibration 
  ${OTBImageBase_LIBRARIES}
  ${OTB6S_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBMetadata_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}

  )

otb_module_target(OTBOpticalCalibration)
