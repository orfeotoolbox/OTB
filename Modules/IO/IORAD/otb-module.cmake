set(DOCUMENTATION "This module contains features to read RAD format images.")

otb_module(OTBIORAD
  DEPENDS
    OTBImageBase
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
