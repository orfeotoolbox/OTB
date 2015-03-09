set(DOCUMENTATION "Morphomath application.")

otb_module(OTBAppMorphology
  DEPENDS
    OTBImageBase
    OTBObjectList
    OTBITK
    OTBApplicationEngine

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
