set(DOCUMENTATION "This module contains classes related to the handling of
vector data (see also VectorDataBase and VectorDataManipulation modules).")

otb_module(OTBVectorDataManipulation
  DEPENDS
    OTBVectorDataBase
    OTBITK
    OTBTransform
    OTBImageManipulation
    OTBPath
    OTBCommon
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBVectorDataIO
    OTBTestKernel
    OTBImageIO
    OTBImageBase

  DESCRIPTION
    "${DOCUMENTATION}"
)
