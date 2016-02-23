set(DOCUMENTATION "This module contains MPI sources and mappers")

otb_module(MPIImageIO
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
