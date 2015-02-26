set(DOCUMENTATION "TODO")

otb_module(OTBMathParserX
  DEPENDS
    OTBCommon
    OTBITK
    OTBMuParserX
    OTBStatistics

  TEST_DEPENDS
    OTBImageBase
    OTBImageIO

  DESCRIPTION
    "${DOCUMENTATION}"
)
