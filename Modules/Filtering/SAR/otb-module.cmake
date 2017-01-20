set(DOCUMENTATION "This module provides basic SAR processing operations.")

otb_module(OTBSAR
  DEPENDS
    OTBCommon
    OTBITK
    OTBOSSIMAdapters
  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
