set(DOCUMENTATION "This module contains MPI mappers")

otb_module(OTBMPIMappers
  DEPENDS
    OTBSPTW
    OTBCommon
    OTBImageIO
  TEST_DEPENDS
    OTBSPTW
    OTBCommon
    OTBTestKernel
    OTBImageIO
    OTBImageBase
		OTBImageManipulation
		OTBInterpolation
    OTBITK
  DESCRIPTION
    "${DOCUMENTATION}"
  )
