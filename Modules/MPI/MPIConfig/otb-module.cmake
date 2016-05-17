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
    OTBInterpolation
    OTBMPITiffWriter
    OTBPanSharpening
    OTBProjection
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
