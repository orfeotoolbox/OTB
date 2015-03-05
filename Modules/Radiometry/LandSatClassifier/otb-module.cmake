set(DOCUMENTATION "TODO")

otb_module(OTBLandSatClassifier
  DEPENDS
    OTBIndices

  TEST_DEPENDS
    OTBITK
    OTBImageBase
    OTBImageIO
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
