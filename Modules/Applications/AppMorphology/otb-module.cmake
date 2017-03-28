set(DOCUMENTATION "Morphomath application.")

otb_module(OTBAppMorphology
  DEPENDS
    OTBImageBase
    OTBObjectList
    OTBITK
    OTBApplicationEngine
    OTBMorphologicalProfiles

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
