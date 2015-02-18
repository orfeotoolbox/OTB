set(DOCUMENTATION "This module contains classes related to the reading and the
writing of remote sensing images.")

otb_module(OTBImageIO 
  DEPENDS
    OTBIORAD
    OTBIOONERA
    OTBIOJPEG2000
    OTBITKPendingPatches
    OTBIOLUM
    OTBImageBase
    OTBIOMW
    OTBIOMSTAR
    OTBIOBSQ
    OTBIOGDAL
    OTBStreaming
    OTBBoost
    OTBIOTileMap
    OTBCommon
    OTBExtendedFilename
    OTBOSSIMAdapters
    OTBObjectList
  TEST_DEPENDS
    OTBTestKernel
    OTBStatistics
  
  DESCRIPTION
    "${DOCUMENTATION}"
  )
