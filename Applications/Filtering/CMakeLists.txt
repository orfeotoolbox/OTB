OTB_CREATE_APPLICATION(NAME           Despeckle
                       SOURCES        otbDespeckle.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Smoothing
                       SOURCES        otbSmoothing.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
