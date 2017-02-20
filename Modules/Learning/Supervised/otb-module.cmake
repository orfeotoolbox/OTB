set(DOCUMENTATION "This module provides the Orfeo Toolbox supervised
classification and regression framework, currently based on
OpenCV and/or libSVM")

otb_module(OTBSupervised
ENABLE_SHARED
  DEPENDS
    OTBBoost
    OTBCommon
    OTBITK
    OTBImageBase
    OTBLearningBase

  OPTIONAL_DEPENDS
    OTBOpenCV
    OTBSVMLearning
    OTBShark
    
  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase
    OTBLearningBase
    OTBBoost

  DESCRIPTION
    "${DOCUMENTATION}"
)
