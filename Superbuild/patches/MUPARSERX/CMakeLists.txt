cmake_minimum_required(VERSION 2.8.3)

project(muparserx)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(BUILD_SHARED_LIBS ON)
if(MSVC)
set(BUILD_SHARED_LIBS OFF) #force off
endif()

file(GLOB MUPARSERX_SRCS "parser/*.cpp" )
file(GLOB MUPARSERX_HEADERS "parser/*.h")
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/parser")

add_library(muparserx ${MUPARSERX_SRCS})

option(BUILD_SAMPLES "Build sample executable" ON)
include_directories(parser)
add_executable(muparserx-sample sample/timer.cpp sample/example.cpp)
target_link_libraries(muparserx-sample muparserx)

install(TARGETS muparserx
  RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
  LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION lib COMPONENT Development)

install(FILES ${MUPARSERX_HEADERS}
  DESTINATION include
  COMPONENT Development)
