set(DOCUMENTATION "OTB implementation of Watersheds segmentation algorithm.")

otb_module(OTBWatersheds
  DEPENDS
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
