include(CheckCXXSourceRuns)

unset(CMAKE_REQUIRED_INCLUDES)
unset(CMAKE_REQUIRED_LIBRARIES)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_FLAGS})
unset(CMAKE_REQUIRED_DEFINES)

# check if unique_ptr is defined
file(READ ${CMAKE_SOURCE_DIR}/CMake/otbTestUniquePtr.cpp
  OTB_TEST_UNIQUE_PTR_SOURCE)
check_cxx_source_runs(
  "${OTB_TEST_UNIQUE_PTR_SOURCE}"
  OTB_IS_UNIQUE_PTR_DEFINED
  )

# check if override keyword is known
file(READ ${CMAKE_SOURCE_DIR}/CMake/otbTestOverride.cpp
  OTB_TEST_OVERRIDE_SOURCE)
check_cxx_source_runs(
  "${OTB_TEST_OVERRIDE_SOURCE}"
  OTB_IS_OVERRIDE_DEFINED
  )

# check if nullptr keyword is known
file(READ ${CMAKE_SOURCE_DIR}/CMake/otbTestNullPtr.cpp
  OTB_TEST_NULLPTR_SOURCE)
check_cxx_source_runs(
  "${OTB_TEST_NULLPTR_SOURCE}"
  OTB_IS_NULLPTR_DEFINED
  )

unset(CMAKE_REQUIRED_FLAGS)
