# This script is a prototype for the futur CI
set (ENV{LANG} "C") # Only ascii output
# Create build directory
get_filename_component(OTB_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

#Ctest setting
set (CTEST_BUILD_CONFIGURATION "Release")
set (CTEST_SOURCE_DIRECTORY "${OTB_SOURCE_DIR}")
set (CTEST_BINARY_DIRECTORY "${OTB_SOURCE_DIR}/build/")
set (CMAKE_COMMAND "cmake")
set (PROJECT_SOURCE_DIR "${OTB_SOURCE_DIR}")

# set (CTEST_BUILD_COMMAND "make")

set (CTEST_CMAKE_GENERATOR "Unix Makefiles")
set (CTEST_BUILD_NAME "CI_TEST")


ctest_start("Nightly")

# ctest_update() no need to update it is done by Gitlab-CI

set (DATA_ROOT_DIR "${OTB_SOURCE_DIR}/otb-data/") # todo
set(otb_configure_option 
"
OTB_USE_OPENGL:BOOL=OFF
OTB_USE_GLEW:BOOL=OFF
OTB_USE_GLFW:BOOL=OFF
OTB_USE_GLUT:BOOL=OFF
OTB_USE_QT:BOOL=OFF
OTB_USE_QWT:BOOL=OFF
OTB_USE_6S:BOOL=OFF
OTB_USE_CURL:BOOL=OFF
OTB_USE_OPENCV:BOOL=OFF
OTB_USE_LIBSVM:BOOL=OFF
OTB_USE_MUPARSER:BOOL=OFF
OTB_USE_MUPARSERX:BOOL=OFF
OTB_WRAP_PYTHON:BOOL=OFF
OTB_WRAP_PYTHON3:BOOL=OFF
OTB_WRAP_JAVA:BOOL=OFF
OTB_USE_SHARK:BOOL=OFF
OTB_USE_OPENMP:BOOL=OFF
BUILD_TESTING:BOOL=OFF
")

set(CONFIGURE_OPTIONS  "")

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}"
    SOURCE "${OTB_SOURCE_DIR}"
    OPTIONS "${CONFIGURE_OPTIONS}"
    RETURN_VALUE _configure_rv)

if ( _configure_rv EQUAL -1 )
  message("An error occurs during ctest_configure ${_configure_rv}")
endif()
message("${_configure_rv}")

# ctest_build()
# ctest_test()