#TravisBuild.cmake - custom cmake scripts for travis build
#before running this script you need to set the below env variables
set(ENV{LC_ALL} C)

set(CTEST_PROJECT_NAME "OTB")
set(CTEST_BUILD_CONFIGURATION Release)
set(CTEST_DASHBOARD_ROOT "/home/travis/build")
#set(CTEST_DASHBOARD_ROOT "/tmp/temp")
# define build name&co for easier identification on CDash
set(CTEST_BUILD_NAME "travis-$ENV{TRAVIS_BUILD_NUMBER}-$ENV{TRAVIS_REPO_SLUG}-$ENV{TRAVIS_BRANCH}-$ENV{BUILD_NAME}-$ENV{CXX}")
set(CTEST_SITE "travis-ci.org")
#set(CTEST_SOURCE_DIRECTORY "$ENV{SOURCE_DIRECTORY}")
#set(CTEST_BINARY_DIRECTORY "${CTEST_SOURCE_DIRECTORY}/../_build")

set(CTEST_SOURCE_DIRECTORY "${CTEST_DASHBOARD_ROOT}/orfeotoolbox/OTB")
set(CTEST_BINARY_DIRECTORY "${CTEST_DASHBOARD_ROOT}/orfeotoolbox/build")

if(NOT DEFINED CTEST_GIT_COMMAND)
  find_program(CTEST_GIT_COMMAND NAMES git git.cmd)
endif()

set(CMAKE_MAKE_PROGRAM "$ENV{MAKE_CMD}")
set(CTEST_CMAKE_GENERATOR "$ENV{CMAKE_GEN}")

set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_ERRORS 1000)
set(CTEST_CUSTOM_MAXIMUM_NUMBER_OF_WARNINGS 300)
#set(CTEST_BUILD_FLAGS -j2)

message(STATUS "CTEST_SOURCE_DIRECTORY: ${CTEST_SOURCE_DIRECTORY}")
message(STATUS "CTEST_BINARY_DIRECTORY: ${CTEST_BINARY_DIRECTORY}")

set(INITIAL_CACHE
"CMAKE_PREFIX_PATH:PATH=/tmp/OTB-5.2.0-xdk-Linux64
BUILD_TESTING:BOOL=ON
CMAKE_MAKE_PROGRAM:FILEPATH=${CMAKE_MAKE_PROGRAM}
OTB_USE_OPENCV:BOOL=ON
OTB_USE_MUPARSER:BOOL=ON
OTB_USE_MUPARSERX:BOOL=ON
OTB_USE_QT4:BOOL=ON
CMAKE_C_FLAGS:STRING=-Wall
CMAKE_CXX_FLAGS:STRING=-Wall -Wno-gnu-static-float-init -Wno-\\#warnings
CMAKE_BUILD_TYPE=Release"
)
file(WRITE "${CTEST_BINARY_DIRECTORY}/CMakeCache.txt" ${INITIAL_CACHE})

ctest_start     (Continuous)
ctest_update    (SOURCE ${CTEST_SOURCE_DIRECTORY} RETURN_VALUE _update_rv)

message(STATUS "Found ${_update_rv} changed files")

ctest_configure (BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE _configure_rv)
ctest_read_custom_files(${CTEST_BINARY_DIRECTORY})

ctest_build     (BUILD "${CTEST_BINARY_DIRECTORY}" RETURN_VALUE _build_rv)

ctest_test      (BUILD "${CTEST_BINARY_DIRECTORY}" INCLUDE Tu RETURN_VALUE _test_rv)
# if(NOT _configure_rv EQUAL 0 OR NOT _build_rv EQUAL 0)
# endif ()

ctest_submit()
