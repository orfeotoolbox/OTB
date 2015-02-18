set(DOCUMENTATION "This module contains OTB generic Machine Learning framework
mainly based on OpenCV.")

otb_module(OTBLearningBase 
  DEPENDS
    OTBOpenCVAdapters
    OTBCommon
    OTBSVMLearning
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
