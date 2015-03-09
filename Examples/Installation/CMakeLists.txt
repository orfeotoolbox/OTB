project(InstallationExamples)

add_executable(HelloWorld HelloWorld.cxx)
target_link_libraries(HelloWorld ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
