set(DOCUMENTATION "This module contains classes related to streaming
functionalities: only the data of a particular requested region are produced. This is
useful when big images must be processed: processing is done piecewise, which prevents
the system from running out of memory.")

otb_module(OTBStreaming
  DEPENDS
    OTBCommon
    OTBITK
    OTBImageBase
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBImageIO
    OTBStatistics
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
