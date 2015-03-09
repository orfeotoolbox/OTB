project(HyperspectralExamples)

add_executable(HyperspectralUnmixingExample HyperspectralUnmixingExample.cxx)
target_link_libraries(HyperspectralUnmixingExample ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
