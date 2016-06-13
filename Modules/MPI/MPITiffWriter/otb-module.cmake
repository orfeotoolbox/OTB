set(DOCUMENTATION "This module contains MPI tiff writer")

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
    OTBMPIConfig
  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
