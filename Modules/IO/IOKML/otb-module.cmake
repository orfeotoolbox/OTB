set(DOCUMENTATION "This module contains features to read and write KML format
vector data.")

otb_module(OTBIOKML
  DEPENDS
    OTBVectorDataBase
    OTBProjection
    OTBlibkml
    OTBGDAL
    OTBCommon
    OTBOSSIMAdapters
    OTBITK

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
