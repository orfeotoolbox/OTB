set(DOCUMENTATION "This module deals with image simulation algorithm. Using
objects transmittance and reflectance and sensor characteristics, it can be possible
to generate realistic hyperspectral synthetic set of data. This module includes
PROSPECT (leaf optical properties) and SAIL (canopy bidirectional reflectance)
models, as well as PROSAIL, which is the combination of the two previous ones.")

otb_module(OTBSimulation 
  DEPENDS
    OTBStatistics
    OTBMetadata
    OTBImageBase
    OTBLabelMap
    OTBImageManipulation
    OTBCommon
    OTBIndices
    OTBOpticalCalibration
    OTBObjectList
    OTBBoost
    OTBInterpolation
  TEST_DEPENDS
    OTBTestKernel
    OTBConversion
    OTBLearningBase
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
