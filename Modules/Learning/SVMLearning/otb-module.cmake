set(DOCUMENTATION "Deprecated classes for SVM learning. See module 'Supervised' for a replacement")

otb_module(OTBSVMLearning 
  DEPENDS
    OTBVectorDataBase
    OTBImageBase
    OTBLibSVM
    OTBLabelMap
    OTBStreaming
    OTBCommon
  TEST_DEPENDS
    OTBMoments
    OTBTestKernel
    OTBIOXML
    OTBVectorDataIO  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
