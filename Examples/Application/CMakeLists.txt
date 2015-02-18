project(ApplicationExamples)

add_executable(ApplicationExample ApplicationExample.cxx)
target_link_libraries(ApplicationExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
