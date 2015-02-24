set(DOCUMENTATION "This module contains classes related to the reading and the
writing of remote sensing images.")

otb_module(OTBImageIO
  DEPENDS
    OTBIOMW
    OTBIORAD
    OTBIOMSTAR
    OTBITK
    OTBITKPendingPatches
    OTBIOLUM
    OTBBoost
    OTBIOJPEG2000
    OTBIOBSQ
    OTBIOGDAL
    OTBIOONERA
    OTBIOTileMap
    OTBCommon
    OTBImageBase
    OTBStreaming
    OTBExtendedFilename
    OTBOSSIMAdapters
    OTBObjectList

  TEST_DEPENDS
    OTBTestKernel
    OTBStatistics

  DESCRIPTION
    "${DOCUMENTATION}"
)
