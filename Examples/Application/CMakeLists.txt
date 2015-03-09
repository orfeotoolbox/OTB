project(ApplicationExamples)

otb_create_application(
  NAME           ApplicationExample
  SOURCES        ApplicationExample.cxx
  LINK_LIBRARIES ${OTB_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
