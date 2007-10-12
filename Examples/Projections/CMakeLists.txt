PROJECT(ProjectionsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(SensorModelExample SensorModelExample.cxx )
TARGET_LINK_LIBRARIES(SensorModelExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(OrthoRectificationExample OrthoRectificationExample.cxx )
TARGET_LINK_LIBRARIES(OrthoRectificationExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO)

