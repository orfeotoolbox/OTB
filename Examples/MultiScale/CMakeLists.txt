PROJECT(MultiScale)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(MorphologicalPyramidAnalyseFilterExample MorphologicalPyramidAnalyseFilterExample.cxx )
TARGET_LINK_LIBRARIES(MorphologicalPyramidAnalyseFilterExample OTBCommon OTBIO OTBMultiScale ITKCommon ITKIO)


