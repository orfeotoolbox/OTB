set(DOCUMENTATION "Everything users need to extract roads.")

otb_module(OTBRoadExtraction 
  DEPENDS
    OTBVectorDataBase
    OTBImageBase
    OTBVectorDataManipulation
    OTBImageManipulation
    OTBMathParser
    OTBPath
    OTBObjectList
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
    OTBVectorDataIO
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
