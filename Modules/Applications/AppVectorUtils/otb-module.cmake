set(DOCUMENTATION "VectorUtils application.")

otb_module(OTBAppVectorUtils
  DEPENDS
    OTBApplicationEngine
    OTBCarto
    OTBCommon
    OTBITK
    OTBImageBase
    OTBProjection
    OTBVectorDataBase
    OTBVectorDataManipulation

  TEST_DEPENDS
    OTBCommandLine
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
