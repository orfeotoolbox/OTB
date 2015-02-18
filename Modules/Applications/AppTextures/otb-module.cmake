set(DOCUMENTATION "Textures application (see also Textures module).")

otb_module(OTBAppTextures 
  DEPENDS
    OTBTextures
    OTBImageBase
    OTBObjectList
    OTBApplicationEngine
    OTBImageManipulation
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
