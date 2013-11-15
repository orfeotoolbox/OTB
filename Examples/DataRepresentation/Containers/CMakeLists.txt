PROJECT(ContainersExamples)
INCLUDE_REGULAR_EXPRESSION("^.*$")

ADD_EXECUTABLE(TreeContainer TreeContainer.cxx )
TARGET_LINK_LIBRARIES(TreeContainer ${ITK_LIBRARIES})


