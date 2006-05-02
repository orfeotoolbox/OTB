PROJECT(Learning)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(GenerateTrainingImageExample GenerateTrainingImageExample.cxx )
TARGET_LINK_LIBRARIES(GenerateTrainingImageExample OTBCommon OTBIO OTBLearning ITKCommon ITKIO)

