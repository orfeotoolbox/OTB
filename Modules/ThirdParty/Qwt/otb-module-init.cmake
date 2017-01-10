find_package(Qwt REQUIRED)

if(NOT QWT_MAJOR_VERSION EQUAL 5)
  message(FATAL_ERROR "Only Qwt 5 is supported. Found version ${QWT_VERSION_STRING}")
endif()

if(WIN32)
  add_definitions(-DQWT_DLL)
endif(WIN32)
