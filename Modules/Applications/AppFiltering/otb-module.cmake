set(DOCUMENTATION "Basic filters application.")

otb_module(OTBAppFiltering 
  DEPENDS
    OTBImageNoise
    OTBImageManipulation
    OTBApplicationEngine
    OTBImageBase
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
