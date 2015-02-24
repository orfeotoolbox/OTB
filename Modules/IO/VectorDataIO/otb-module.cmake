set(DOCUMENTATION "This module contains classes related to the reading and the
writing of vector data (see also VectorDataBase and VectorDataManipulation
modules).")

otb_module(OTBVectorDataIO
  DEPENDS
    OTBVectorDataBase
    OTBIOGDAL
    OTBITK
    OTBVectorDataManipulation
    OTBIOKML
    OTBCommon

  TEST_DEPENDS
    OTBTestKernel
    OTBOSSIMAdapters

  DESCRIPTION
    "${DOCUMENTATION}"
)
