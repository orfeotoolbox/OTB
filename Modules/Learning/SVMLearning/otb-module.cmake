set(DOCUMENTATION "OTB implementation of the Support Vector Machine.")

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
    OTBOpenCVAdapters
    OTBVectorDataIO
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
