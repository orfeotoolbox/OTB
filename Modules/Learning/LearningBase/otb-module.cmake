set(DOCUMENTATION "This module contains OTB generic Machine Learning framework
mainly based on OpenCV.")

otb_module(OTBLearningBase
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageIO
    OTBImageBase

  OPTIONAL_DEPENDS
    OTBShark

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
