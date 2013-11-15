project(ContainersExamples)
include_regular_expression("^.*$")

add_executable(TreeContainer TreeContainer.cxx )
target_link_libraries(TreeContainer ${ITK_LIBRARIES})


