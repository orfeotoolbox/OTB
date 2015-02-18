set(DOCUMENTATION "Morphomath application.")

otb_module(OTBAppMorphology 
  DEPENDS
    OTBImageBase
    OTBApplicationEngine
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
