set(DOCUMENTATION "TODO")

otb_module(OTBAppDomainTransform
DEPENDS
  OTBWavelet
  OTBApplicationEngine

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)

