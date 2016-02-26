cmake_minimum_required(VERSION 2.8.8)

project(OTB-OSSIM)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ossim_package_support/cmake/CMakeModules")
message(STATUS "[OTB SuperBuild] CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
add_subdirectory(ossim)