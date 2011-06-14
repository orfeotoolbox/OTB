project(OTB-Wrapper)

cmake_minimum_required(VERSION 2.6)

if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

include(Definitions)

# Link to the Orfeo ToolBox
find_package(OTB REQUIRED PATHS /usr/lib/otb )
include(${OTB_USE_FILE})

set(BUILD_SHARED_LIBS ${OTB_BUILD_SHARED_LIBS})

add_subdirectory(Code)
add_subdirectory(Example)

# Enable / Disable testing
option(BUILD_TESTING  "Build testing." OFF)

include(${CMAKE_ROOT}/Modules/Dart.cmake)
mark_as_advanced(TCL_TCLSH DART_ROOT)
if(BUILD_TESTING)
  enable_testing()

  find_program(OTB_TEST_DRIVER otbTestDriver PATHS ${OTB_DIR}/bin ${OTB_DIR}/../../bin DOC "Path to the otbTestDriver executable from OTB")

  find_path(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB_SOURCE_DIR}/../OTB-Data)
  mark_as_advanced(OTB_DATA_ROOT)

  set(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  mark_as_advanced(BUILDNAME)
  add_subdirectory(Testing)
endif(BUILD_TESTING)



