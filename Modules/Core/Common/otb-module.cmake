set(DOCUMENTATION "TO BE DOCUMENTED")

otb_module(OTBCommon
  DEPENDS
    OTBBoost
    OTBITK
    OTBITKPendingPatches

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
