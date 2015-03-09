set(DOCUMENTATION "TO BE DOCUMENTED")

otb_module(OTBCommon
  DEPENDS
    OTBBoost
    OTBITK

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
