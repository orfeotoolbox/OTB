set(DOCUMENTATION "VectorUtils application.")

otb_module(OTBAppVectorUtils 
  DEPENDS
    OTBVectorDataBase
    OTBProjection
    OTBVectorDataManipulation
    OTBCarto
    OTBCommon
    OTBApplicationEngine
  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
