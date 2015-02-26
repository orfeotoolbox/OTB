set(DOCUMENTATION "This module contains classes related to the handling of
vector data (see also VectorDataBase and VectorDataManipulation modules).")

otb_module(OTBVectorDataManipulation
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageManipulation
    OTBOSSIMAdapters
    OTBObjectList
    OTBPath
    OTBTransform
    OTBVectorDataBase

  TEST_DEPENDS
    OTBCommandLineParser
    OTBImageBase
    OTBImageIO
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
