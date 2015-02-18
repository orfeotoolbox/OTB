set(DOCUMENTATION "This Module contains classes that implement the MeanShift
segmentation algorithm.")

otb_module(OTBMeanShift 
  DEPENDS
    OTBCCOBIA
    OTBConversion
    OTBSmoothing
    OTBMathParser
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
