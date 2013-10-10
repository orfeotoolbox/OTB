
OTB_CREATE_APPLICATION(NAME           MeanShiftSmoothing
                       SOURCES        otbMeanShiftSmoothing.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ConnectedComponentSegmentation
                       SOURCES        otbConnectedComponentSegmentation.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           HooverCompareSegmentation
                       SOURCES        otbHooverCompareSegmentation.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           Segmentation
                       SOURCES        otbSegmentation.cxx
                       LINK_LIBRARIES OTBBasicFilters;OTBOBIA;OTBOGRAdapters)

OTB_CREATE_APPLICATION(NAME           LSMSSegmentation
                       SOURCES        otbLSMSSegmentation.cxx
                       LINK_LIBRARIES OTBBasicFilters;OTBOBIA;OTBOGRAdapters)

OTB_CREATE_APPLICATION(NAME           LSMSSmallRegionsMerging
                       SOURCES        otbLSMSSmallRegionsMerging.cxx
                       LINK_LIBRARIES OTBBasicFilters;OTBOBIA;OTBOGRAdapters)

OTB_CREATE_APPLICATION(NAME           LSMSVectorization
                       SOURCES        otbLSMSVectorization.cxx
                       LINK_LIBRARIES OTBBasicFilters;OTBOBIA;OTBOGRAdapters)
