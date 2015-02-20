set(DOCUMENTATION "This module contains OTB generic Machine Learning framework
mainly based on OpenCV.")

otb_module(OTBLearningBase 
  DEPENDS
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
  )
