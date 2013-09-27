#cmake_minimum_required(VERSION 2.8)
#PROJECT(ContrastEnhancement)
#INCLUDE_REGULAR_EXPRESSION("^.*$")

#FIND_PACKAGE(OTB REQUIRED)
#INCLUDE(${OTB_USE_FILE})
#ADD_EXECUTABLE(HistogramEqualize HistogramEqualize.cxx )
#TARGET_LINK_LIBRARIES(HistogramEqualize OTBCommon OTBIO OTBContrastEnhancementFilters)

#DD_EXECUTABLE(HistogramEqualize_gray HistogramEqualize_gray.cxx )
#TARGET_LINK_LIBRARIES(HistogramEqualize_gray OTBCommon OTBIO OTBContrastEnhancementFilters)

#ADD_EXECUTABLE(ColorSpace ColorSpace.cxx )
#TARGET_LINK_LIBRARIES(ColorSpace OTBCommon OTBIO OTBContrastEnhancementFilters)
OTB_CREATE_APPLICATION(NAME           ColorSpaceTransform
                       SOURCES        ColorSpaceTransform.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBContrastEnhancementFilters)

OTB_CREATE_APPLICATION(NAME           MonoChannelHistogramEqualize
                       SOURCES        MonoChannelHistogramEqualize.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBContrastEnhancementFilters)

OTB_CREATE_APPLICATION(NAME           ColorHistogramEqualize
                       SOURCES        ColorHistogramEqualize.cxx
                       LINK_LIBRARIES OTBIO;OTBCommon;OTBContrastEnhancementFilters)


#OTB_CREATE_APPLICATION(NAME FindPixelType 
#                      SOURCES FindPixelType.cxx
#                       LINK_LIBRARIES OTBIO;OTBCommon)
                       

