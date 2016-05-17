set(DOCUMENTATION "TODO")

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
