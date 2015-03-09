set(DOCUMENTATION "The region connection calculus (RCC) is used for qualitative
spatial representation and . RCC abstractly describes regions by their possible
relations to each other (8 basic relations are possible between two regions). The
RCC8 constraint language has been popularly adopted by the GIS communities. This
module provides classes related to this purpose.")

otb_module(OTBRCC8
  DEPENDS
    OTBITK
    OTBImageBase
    OTBRoadExtraction
    OTBImageManipulation
    OTBPath
    OTBCommon
    OTBBoost
    OTBObjectList

  TEST_DEPENDS
    OTBVectorDataBase
    OTBTestKernel
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
