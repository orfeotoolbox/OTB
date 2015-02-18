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
    OTBGDAL
    OTBInterpolation
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
