set(DOCUMENTATION "Projection application (see also Projection module).")

otb_module(OTBAppProjection
  DEPENDS
    OTBGdalAdapters
    OTBImageManipulation
    OTBProjection
    OTBImageBase
    OTBTransform
    OTBOSSIMAdapters
    OTBCarto
    OTBApplicationEngine
    OTBMathParser
    OTBCommon
    OTBInterpolation
    OTBGDAL
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
