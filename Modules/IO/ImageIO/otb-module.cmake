set(DOCUMENTATION "This module contains classes related to the reading and the
writing of remote sensing images.")

otb_module(OTBImageIO
  DEPENDS
    OTBBoostAdapters
    OTBCommon
    OTBExtendedFilename
    OTBIOBSQ
    OTBIOGDAL
    OTBIOLUM
    OTBIOMSTAR
    OTBIOONERA
    OTBIORAD
    OTBIOTileMap
    OTBITK
    OTBImageBase
    OTBOSSIMAdapters
    OTBObjectList
    OTBStreaming

  TEST_DEPENDS
    OTBStatistics
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
