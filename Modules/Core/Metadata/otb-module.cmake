set(DOCUMENTATION "This module contains a set of classes that allow parsing
metadata files from different types of sensor (both optical and radar sensor types
are supported. for instance: Pleiades, SPOT6, TerraSar, and so on).")

otb_module(OTBMetadata 
  DEPENDS
    OTBBoost
    OTBCommon
    OTBOSSIMAdapters
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
