set(DOCUMENTATION "SAR Utils application.")

otb_module(OTBAppSARUtils
  DEPENDS
    OTBApplicationEngine
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
  )
