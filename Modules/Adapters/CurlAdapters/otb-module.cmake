set(DOCUMENTATION "Adapters for the Curl library.")

otb_module(OTBCurlAdapters 
  DEPENDS
    OTBCurl
    OTBCommon
  TEST_DEPENDS
    OTBTestKernel
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
