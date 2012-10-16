
OTB_CREATE_APPLICATION(NAME LineSegmentDetection
                       SOURCES otbLineSegmentDetection.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
                       
OTB_CREATE_APPLICATION(NAME HaralickTextureExtraction
                       SOURCES otbHaralickTextureExtraction.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
                       
#OTB_CREATE_APPLICATION(NAME ComputeImageFeatures
#                       SOURCES otbComputeImageFeatures.cxx
#                       LINK_LIBRARIES OTBIO;OTBCommon;OTBBasicFilters)
