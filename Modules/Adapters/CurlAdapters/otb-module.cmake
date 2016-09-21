set(DOCUMENTATION "Adapters for the Curl library.")

otb_module(OTBCurlAdapters
ENABLE_SHARED
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
