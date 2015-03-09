set(DOCUMENTATION "This module contains classes related to the handling of
vector data (see also VectorDataBase and VectorDataManipulation modules).")

otb_module(OTBVectorDataManipulation
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBImageManipulation
    OTBOSSIMAdapters
    OTBObjectList
    OTBPath
    OTBTransform
    OTBVectorDataBase

  TEST_DEPENDS
    OTBCommandLineParser
    OTBImageIO
    OTBProjection
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
