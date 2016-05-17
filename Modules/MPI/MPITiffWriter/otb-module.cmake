set(DOCUMENTATION "This module contains MPI sources and mappers")

otb_module(OTBMPITiffWriter
  DEPENDS
    OTBBoost
    OTBCommon
    OTBExtendedFilename
    OTBGDAL
    OTBGdalAdapters
    OTBITK
    OTBImageBase
    OTBImageIO
    OTBOSSIMAdapters
    OTBSPTW
    OTBStreaming
    OTBMPI

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
