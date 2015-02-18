set(DOCUMENTATION "VectorDataTranslation application (translation, rotation and
scale).")

otb_module(OTBAppVectorDataTranslation 
  DEPENDS
    OTBConversion
    OTBApplicationEngine
    OTBTransform
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
