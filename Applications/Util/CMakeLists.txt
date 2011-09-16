
OTB_CREATE_APPLICATION(NAME           ExtractROI
                       SOURCES        otbExtractROI.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Rescale
                       SOURCES        otbRescale.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Smoothing
                       SOURCES        otbSmoothing.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ReadImageInfo
                       SOURCES        otbReadImageInfo.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

