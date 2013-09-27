OTB_CREATE_APPLICATION(NAME           ColorSpaceTransform
                       SOURCES        otbColorSpaceTransform.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBContrastEnhancementFilters)

OTB_CREATE_APPLICATION(NAME           MonoChannelHistogramEqualize
                       SOURCES        otbMonoChannelHistogramEqualize.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBContrastEnhancementFilters)

OTB_CREATE_APPLICATION(NAME           ColorHistogramEqualize
                       SOURCES        otbColorHistogramEqualize.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBContrastEnhancementFilters)


#OTB_CREATE_APPLICATION(NAME FindPixelType 
#                      SOURCES FindPixelType.cxx
#                       LINK_LIBRARIES OTBIO;OTBCommon)
                       

