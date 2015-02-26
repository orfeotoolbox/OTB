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
  ${OTB6S_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBITK_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBMetadata_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBObjectList_LIBRARIES}
  )

otb_module_target(OTBOpticalCalibration)
