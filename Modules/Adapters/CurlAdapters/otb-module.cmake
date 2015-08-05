set(DOCUMENTATION "Adapters for the Curl library.")

otb_module(OTBCurlAdapters
  DEPENDS
    OTBCommon
    OTBITK

  OPTIONAL_DEPENDS
    OTBCurl

  TEST_DEPENDS
    OTBTestKernel
    OTBTinyXML

  DESCRIPTION
    "${DOCUMENTATION}"
)
