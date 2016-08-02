set(DOCUMENTATION "TODO")

otb_module(OTBCommandLineParser
  DEPENDS
    OTBCommon
    OTBITK

  TEST_DEPENDS
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
