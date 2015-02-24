set(DOCUMENTATION "This module contains features to read LUM format images.")

otb_module(OTBIOLUM
  DEPENDS
    OTBImageBase
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
