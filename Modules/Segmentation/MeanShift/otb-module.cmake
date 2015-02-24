set(DOCUMENTATION "This Module contains classes that implement the MeanShift
segmentation algorithm.")

otb_module(OTBMeanShift
  DEPENDS
    OTBCCOBIA
    OTBConversion
    OTBSmoothing
    OTBITK
    OTBMathParser

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
