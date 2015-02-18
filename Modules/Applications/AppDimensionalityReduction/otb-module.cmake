set(DOCUMENTATION "Dimensionality reduction application( see also
DimensionalityReduction module).")

otb_module(OTBAppDimensionalityReduction 
  DEPENDS
    OTBImageManipulation
    OTBStatistics
    OTBApplicationEngine
    OTBDimensionalityReduction
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
