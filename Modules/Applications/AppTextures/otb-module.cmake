set(DOCUMENTATION "Textures application (see also Textures module).")

otb_module(OTBAppTextures
  DEPENDS
    OTBTextures
    OTBITK
    OTBImageBase
    OTBApplicationEngine
    OTBImageManipulation
    OTBObjectList

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
