OTB_CREATE_APPLICATION(NAME           EstimateImagesStatistics
                       SOURCES        otbEstimateImagesStatistics.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)

OTB_CREATE_APPLICATION(NAME           ImageSVMClassifier
                       SOURCES        otbImageSVMClassifier.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
