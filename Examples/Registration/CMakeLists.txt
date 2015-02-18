project(RegistrationExamples)

add_executable(ImageRegistration1 ImageRegistration1.cxx)
target_link_libraries(ImageRegistration1 ${OTB_LIBRARIES})

add_executable(ImageRegistration2 ImageRegistration2.cxx)
target_link_libraries(ImageRegistration2 ${OTB_LIBRARIES})

add_executable(ImageRegistration5 ImageRegistration5.cxx)
target_link_libraries(ImageRegistration5 ${OTB_LIBRARIES})

add_executable(ImageRegistration9 ImageRegistration9.cxx)
target_link_libraries(ImageRegistration9 ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
