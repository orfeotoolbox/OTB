set(DOCUMENTATION "TO BE DOCUMENTED")

otb_module(OTBCommon 
  DEPENDS
    OTBITKPendingPatches
    OTBITK
    OTBConfigFile
  TEST_DEPENDS
    OTBTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
)
