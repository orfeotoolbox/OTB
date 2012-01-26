PROJECT(ChangeDetectionExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

# Adjust the compiler flags to avoid problems with multiline comment.
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET_SOURCE_FILES_PROPERTIES(
        KullbackLeiblerDistanceChDet.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

ADD_EXECUTABLE(PCAExample PCAExample.cxx )
TARGET_LINK_LIBRARIES(PCAExample OTBCommon OTBIO OTBBasicFilters)

ADD_EXECUTABLE(MaximumAutocorrelationFactor MaximumAutocorrelationFactor.cxx)
TARGET_LINK_LIBRARIES(MaximumAutocorrelationFactor OTBIO OTBCommon OTBBasicFilters)
