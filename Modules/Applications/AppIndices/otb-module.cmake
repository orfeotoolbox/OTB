set(DOCUMENTATION "Classical indices (NDVI, NDWI,) application.")

otb_module(OTBAppIndices 
  DEPENDS
    OTBIndices
    OTBApplicationEngine
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
