set(DOCUMENTATION "Deprecated classes for SVM learning. See module 'Supervised' for a replacement")

otb_module(OTBSVMLearning
  DEPENDS
    OTBVectorDataBase
    OTBITK
    OTBImageBase
    OTBLibSVM
    OTBLabelMap
    OTBStreaming
    OTBCommon

  TEST_DEPENDS
    OTBStatistics
    OTBImageIO
    OTBMoments
    OTBVectorDataIO
    OTBIOXML
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
