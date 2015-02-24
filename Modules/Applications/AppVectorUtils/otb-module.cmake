set(DOCUMENTATION "VectorUtils application.")

otb_module(OTBAppVectorUtils
  DEPENDS
    OTBVectorDataBase
    OTBProjection
    OTBVectorDataManipulation
    OTBCarto
    OTBApplicationEngine
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel
    OTBCommandLine

  DESCRIPTION
    "${DOCUMENTATION}"
)
