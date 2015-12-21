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
    OTBIOMW
    OTBIOONERA
    OTBIORAD
    OTBIOTileMap
    OTBITK
    OTBImageBase
    OTBOSSIMAdapters
    OTBObjectList
    OTBStreaming

  OPTIONAL_DEPENDS
    OTBIOJPEG2000

  TEST_DEPENDS
    OTBStatistics
    OTBTestKernel

  DESCRIPTION
    "${DOCUMENTATION}"
)
