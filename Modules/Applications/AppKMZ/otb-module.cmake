set(DOCUMENTATION "KMZ generation related applications.")

otb_module(OTBAppKMZ
  DEPENDS
    OTBApplicationEngine
    OTBKMZWriter

  TEST_DEPENDS
    OTBCommandLine
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
