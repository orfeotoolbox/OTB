#TravisBuild.cmake - custom cmake scripts for travis build
#before running this script you need to set the below env variables
set(ENV{LC_ALL} C)

set(CTEST_PROJECT_NAME "OTB")
set(CTEST_BUILD_CONFIGURATION Release)
set(CTEST_DASHBOARD_ROOT "/home/travis/build")

set(BUILD_NAME_PREFIX "travis-$ENV{TRAVIS_BUILD_NUMBER}")
if(NOT "$ENV{TRAVIS_PULL_REQUEST}" MATCHES "false")
  set(BUILD_NAME_PREFIX "PR-travis-$ENV{TRAVIS_PULL_REQUEST}")
endif()

set(CTEST_BUILD_NAME "${BUILD_NAME_PREFIX}-$ENV{CC}-${CTEST_BUILD_CONFIGURATION}-$ENV{TRAVIS_BRANCH}")
set(CTEST_SITE "travis-ci.org")

set(CTEST_SOURCE_DIRECTORY "${CTEST_DASHBOARD_ROOT}/orfeotoolbox/OTB")
set(CTEST_BINARY_DIRECTORY "${CTEST_DASHBOARD_ROOT}/orfeotoolbox/build")

if(NOT DEFINED CTEST_GIT_COMMAND)
  find_program(CTEST_GIT_COMMAND NAMES git git.cmd)
endif()

if(DEFINED ENV{MAKE_CMD})
  set(CMAKE_MAKE_PROGRAM "$ENV{MAKE_CMD}")
else()
  message(FATAL_ERROR "MAKE_CMD environment variable is not defined")
endif()

if(DEFINED ENV{CMAKE_GEN})
  set(CTEST_CMAKE_GENERATOR "$ENV{CMAKE_GEN}")
else()
  message(FATAL_ERROR "CMAKE_GEN environment variable is not defined")
endif()

set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_ERRORS 1000)
set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS 300)
#set(CTEST_BUILD_FLAGS -j2)

message(STATUS "CTEST_SOURCE_DIRECTORY: ${CTEST_SOURCE_DIRECTORY}")
message(STATUS "CTEST_BINARY_DIRECTORY: ${CTEST_BINARY_DIRECTORY}")

set(INITIAL_CACHE
"CMAKE_PREFIX_PATH:PATH=/tmp/OTB-5.2.1-Linux64
BUILD_TESTING:BOOL=ON
CMAKE_MAKE_PROGRAM:FILEPATH=${CMAKE_MAKE_PROGRAM}
OTB_USE_OPENCV:BOOL=ON
OTB_USE_MUPARSER:BOOL=ON
OTB_USE_MUPARSERX:BOOL=ON
OTB_USE_QT4:BOOL=ON
CMAKE_C_FLAGS:STRING=-Wextra
CMAKE_CXX_FLAGS:STRING=-Wextra -Wno-gnu-static-float-init -Wno-\\#warnings
CMAKE_BUILD_TYPE=Release"
)
file(WRITE "${CTEST_BINARY_DIRECTORY}/CMakeCache.txt" ${INITIAL_CACHE})

ctest_start     (Continuous)
ctest_update    (SOURCE ${CTEST_SOURCE_DIRECTORY} RETURN_VALUE _update_rv)

message(STATUS "Found ${_update_rv} changed files")

ctest_configure (BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE _configure_rv)
ctest_read_custom_files(${CTEST_BINARY_DIRECTORY})

ctest_build     (BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE _build_rv)

ctest_test      (BUILD "${CTEST_BINARY_DIRECTORY}" INCLUDE New RETURN_VALUE _test_rv)
# if(NOT _configure_rv EQUAL 0 OR NOT _build_rv EQUAL 0)
# endif ()

ctest_submit()
