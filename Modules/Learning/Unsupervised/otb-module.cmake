set(DOCUMENTATION "This module provides the Orfeo Toolbox unsupervised
classification and regression framework, currently based on Shark")

otb_module(OTBUnsupervised
  DEPENDS
  OTBCommon
  OTBITK
  OTBImageBase
  OTBLearningBase

  OPTIONAL_DEPENDS
  OTBShark

  TEST_DEPENDS
  OTBTestKernel
  OTBImageIO
  OTBImageBase

  DESCRIPTION
  "${DOCUMENTATION}"
  )
