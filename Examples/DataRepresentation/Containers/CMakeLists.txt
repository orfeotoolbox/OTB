project(ContainersExamples)

add_executable(TreeContainer TreeContainer.cxx )
target_link_libraries(TreeContainer ${ITK_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
