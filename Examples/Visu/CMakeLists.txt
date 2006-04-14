PROJECT(FeatureExtraction)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(GreyVisuExample GreyVisuExample.cxx )
TARGET_LINK_LIBRARIES(GreyVisuExample OTBVisu OTBCommon OTBIO ITKCommon ITKIO)


