set(DOCUMENTATION "TODO")

otb_module(OTBDEM
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBOSSIMAdapters
    OTBTransform

  TEST_DEPENDS
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
