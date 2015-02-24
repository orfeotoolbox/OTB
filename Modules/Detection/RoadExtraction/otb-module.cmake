set(DOCUMENTATION "Everything users need to extract roads.")

otb_module(OTBRoadExtraction
  DEPENDS
    OTBVectorDataBase
    OTBITK
    OTBImageBase
    OTBVectorDataManipulation
    OTBImageManipulation
    OTBMathParser
    OTBPath
    OTBObjectList
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBProjection
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
