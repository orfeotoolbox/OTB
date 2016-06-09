set(DOCUMENTATION "This module provides the Orfeo Toolbox supervised
classification and regression framework, currently based on
OpenCV and/or libSVM")

otb_module(OTBSupervised
  DEPENDS
    OTBBoost
    OTBCommon
    OTBITK
    OTBImageBase
    OTBShark

  OPTIONAL_DEPENDS
    OTBOpenCV
    OTBSVMLearning

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBImageBase
    OTBBoost

  DESCRIPTION
    "${DOCUMENTATION}"
)
