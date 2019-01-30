# This script is a prototype for the futur CI
set (ENV{LANG} "C") # Only ascii output
# Create build directory
get_filename_component(OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

# Build Configuration : Release, Debug..
set (CTEST_BUILD_CONFIGURATION "Release")

# Directory variable
set (CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}")
set (CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build/")
set (CTEST_INSTALL_DIRECTORY "${OTB_SOURCE_DIR}/install/")
set (PROJECT_SOURCE_DIR "${OTB_SOURCE_DIR}")

# Ctest command value
set (CMAKE_COMMAND "cmake")
set (CTEST_BUILD_FLAGS "-j8") # number of thread used for compilation
set (CTEST_BUILD_COMMAND "make ${CTEST_BUILD_FLAGS}")

set (CTEST_CMAKE_GENERATOR "Unix Makefiles")
set (CTEST_BUILD_NAME "CI_TEST")


# Data directory setting
set (OTB_DATA_ROOT "${OTB_SOURCE_DIR}/otb-data/") # todo
set (OTB_LARGEINPUT_ROOT "") # todo


#The following file set the CONFIGURE_OPTIONS variable
set (CONFIGURE_OPTIONS  "")
include ( "${CMAKE_CURRENT_LIST_DIR}/configure_option.cmake" )

# End of configuration


ctest_start ("Nightly")

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${OTB_SOURCE_DIR}"
    OPTIONS "${CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv
    CAPTURE_CMAKE_ERROR _configure_error)

if ( _configure_rv EQUAL -1 )
  message("An error occurs during ctest_configure:
    ${_configure_error}")
endif()

ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}"
            RETURN_VALUE _build_rv]
            CAPTURE_CMAKE_ERROR _build_error]
            )

if ( _configure_rv EQUAL -1 )
  message("An error occurs during ctest_build:
    ${_build_error}")
endif()

ctest_test()