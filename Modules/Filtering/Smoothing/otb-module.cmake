set(DOCUMENTATION "This module contains classes that allow image smoothing (it
contains mean shift smoothing for instance).")

otb_module(OTBSmoothing
  DEPENDS
    OTBImageBase
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
