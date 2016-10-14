
set(Boost_USE_STATIC_LIBS OFF CACHE BOOL "use static libraries from boost")

set(Boost_USE_MULTITHREADED ON CACHE BOOL "use multi-threaded libraries from boost")

find_package (Boost 1.35.0 REQUIRED)
if (BUILD_TESTING)
  find_package (Boost 1.35.0 QUIET COMPONENTS unit_test_framework)
  if (NOT Boost_UNIT_TEST_FRAMEWORK_FOUND)
    message(STATUS "Boost unit_test_framework not found. Hence otbOGRTests will be skipped")
  else()
    message(STATUS "Found Boost components: unit_test_framework")
  endif()
endif() #BUILD_TESTING
