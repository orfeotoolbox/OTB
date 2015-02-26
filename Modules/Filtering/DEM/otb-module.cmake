set(DOCUMENTATION "TODO")

otb_module(OTBDEM
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBOSSIMAdapters
    OTBTransform

  TEST_DEPENDS
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
