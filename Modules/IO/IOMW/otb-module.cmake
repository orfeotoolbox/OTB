set(DOCUMENTATION "This module contains features to read MW format images.")

otb_module(OTBIOMW
  DEPENDS
    OTBImageBase
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
