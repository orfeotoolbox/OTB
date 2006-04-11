PROJECT(FeatureExtraction)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(ChangeDetectionFrameworkExample ChangeDetectionFrameworkExample.cxx )
TARGET_LINK_LIBRARIES(ChangeDetectionFrameworkExample OTBCommon OTBIO ITKCommon ITKIO)


