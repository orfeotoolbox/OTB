set(DOCUMENTATION "Fusion of classifications application.")

otb_module(OTBAppFusion
  DEPENDS
    OTBITK
    OTBPanSharpening
    OTBImageBase
    OTBApplicationEngine
    OTBProjection
    OTBInterpolation

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
