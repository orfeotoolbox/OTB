cmake_minimum_required(VERSION 2.8.3)

project(libsvm)

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

set(LIBSVM_SRCS "svm.cpp" )
set(LIBSVM_HEADERS "svm.h")

add_library(svm ${LIBSVM_SRCS})

option(BUILD_SAMPLES "Build executables" ON)

add_executable(svm-train svm-train.c)
target_link_libraries(svm-train svm)

add_executable(svm-predict svm-predict.c)
target_link_libraries(svm-predict svm)

add_executable(svm-scale svm-scale.c)
target_link_libraries(svm-scale svm)

install(TARGETS svm
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

install(FILES ${LIBSVM_HEADERS} DESTINATION include)
