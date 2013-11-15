project(ChangeDetectionExamples)
include_regular_expression("^.*$")

# Adjust the compiler flags to avoid problems with multiline comment.
if(CMAKE_COMPILER_IS_GNUCXX)
  set_source_files_properties(
        KullbackLeiblerDistanceChDet.cxx
        PROPERTIES COMPILE_FLAGS -Wno-comment
  )
endif()

add_executable(NAPCAExample NAPCAExample.cxx )
target_link_libraries(NAPCAExample OTBCommon OTBIO OTBBasicFilters)

add_executable(MNFExample MNFExample.cxx )
target_link_libraries(MNFExample OTBCommon OTBIO OTBBasicFilters)

add_executable(ICAExample ICAExample.cxx )
target_link_libraries(ICAExample OTBCommon OTBIO OTBBasicFilters)

add_executable(PCAExample PCAExample.cxx )
target_link_libraries(PCAExample OTBCommon OTBIO OTBBasicFilters)

add_executable(MaximumAutocorrelationFactor MaximumAutocorrelationFactor.cxx)
target_link_libraries(MaximumAutocorrelationFactor OTBIO OTBCommon OTBBasicFilters)
