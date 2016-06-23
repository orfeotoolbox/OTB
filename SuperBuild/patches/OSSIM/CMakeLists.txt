cmake_minimum_required(VERSION 2.8.8)

project(OTB-OSSIM)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ossim_package_support/cmake/CMakeModules")

#mantis: 1193
#RK: do we need to do this for other dependencies?
if(APPLE)
  set(CMAKE_SYSTEM_FRAMEWORK_PATH "" CACHE PATH "" FORCE)
endif()
message(STATUS "[OTB SuperBuild] CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
add_subdirectory(ossim)
