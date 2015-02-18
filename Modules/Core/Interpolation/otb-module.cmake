set(DOCUMENTATION "This module contains additional interpolation functions
(compared to the ones already available in itk -see itk::InterpolateImageFunction-).")

otb_module(OTBInterpolation 
  DEPENDS
    OTBImageBase
    OTBBoost
    OTBCommon
    OTBITKPendingPatches
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
