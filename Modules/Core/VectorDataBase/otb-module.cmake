set(DOCUMENTATION "This module contains base classes for representing vector
data (see also VectorDataIO and VectorDataManipulation modules).")

otb_module(OTBVectorDataBase
ENABLE_SHARED
  DEPENDS
    OTBCommon
    OTBGDAL
    OTBITK
    OTBImageBase
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBTestKernel
    OTBVectorDataIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
