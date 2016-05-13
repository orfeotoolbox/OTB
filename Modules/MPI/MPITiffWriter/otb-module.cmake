set(DOCUMENTATION "This module contains MPI sources and mappers")

otb_module(OTBMPITiffWriter
  DEPENDS
    OTBMPI
    OTBSPTW
    OTBCommon
    OTBImageIO

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
  )
