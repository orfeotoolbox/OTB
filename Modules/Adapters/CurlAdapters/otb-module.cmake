set(DOCUMENTATION "Adapters for the Curl library.")

otb_module(OTBCurlAdapters 
  DEPENDS
    OTBCommon
  OPTIONAL_DEPENDS
    OTBCurl
  TEST_DEPENDS
    OTBTestKernel
  DESCRIPTION
    "${DOCUMENTATION}"
  )
