set(DOCUMENTATION "This module contains filters that can apply any mathematical
expression to the pixels of an image. Note that classes based on MuParserX have
better capabilities than the ones based on MuParser.")

otb_module(OTBMathParser
  DEPENDS
    OTBVectorDataBase
    OTBMuParser
    OTBITK
    OTBMuParserX
    OTBStatistics
    OTBCommon
    OTBPath
    OTBImageManipulation

  TEST_DEPENDS
    OTBTestKernel
    OTBImageIO
    OTBProjection
    OTBVectorDataIO
    OTBImageBase
    OTBObjectList

  DESCRIPTION
    "${DOCUMENTATION}"
)
