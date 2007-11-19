PROJECT(ProjectionsExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

# Adjust the compiler flags to avoid problems with multiline comment.
IF(CMAKE_COMPILER_IS_GNUCXX)
  # Hide warnings in code.
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-comment")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


ADD_EXECUTABLE(SensorModelExample SensorModelExample.cxx )
TARGET_LINK_LIBRARIES(SensorModelExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO)

ADD_EXECUTABLE(OrthoRectificationExample OrthoRectificationExample.cxx )
TARGET_LINK_LIBRARIES(OrthoRectificationExample OTBProjections OTBCommon OTBIO ITKCommon ITKIO)

