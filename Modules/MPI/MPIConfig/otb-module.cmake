set(DOCUMENTATION "Provides a helper class to configure and use MPI.")

otb_module(OTBMPIConfig
  DEPENDS
    OTBITK
    OTBImageIO
    OTBStreaming
    OTBMPI
  TEST_DEPENDS
    OTBBoost
    OTBImageBase
    OTBImageManipulation
    OTBMPITiffWriter
    OTBTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
)
