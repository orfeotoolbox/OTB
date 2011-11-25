
OTB_CREATE_APPLICATION(NAME           MeanShiftSegmentation
                       SOURCES        otbMeanShiftSegmentation.cxx
                       LINK_LIBRARIES OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ConnectedComponentSegmentation
                       SOURCES        otbConnectedComponentSegmentation.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
