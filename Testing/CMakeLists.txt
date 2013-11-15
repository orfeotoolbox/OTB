# $Id$

project(OTBTesting)

# The Testing directory is setup so it can build either as part of the
# normal OTB build or as a stand-alone project.  This design is useful
# for testing the installation tree of OTB.

file(MAKE_DIRECTORY ${OTBTesting_BINARY_DIR}/Temporary)

if(OTB_BINARY_DIR)
  # We are building inside the tree.
  include(${CMAKE_CURRENT_SOURCE_DIR}/TestingHelper.cmake)
  add_subdirectory(Code)
  add_subdirectory(Fa)
  add_subdirectory(Utilities)

  if(BUILD_APPLICATIONS)
    add_subdirectory(Applications)
  endif()

else()
  # We are building as a stand-alone project.
  cmake_minimum_required(VERSION 2.6)

  set(OTB_TESTING_STANDALONE ON)

  set(LIBRARY_OUTPUT_PATH ${OTBTesting_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
  set(EXECUTABLE_OUTPUT_PATH ${OTBTesting_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
  mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
  set(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

  # Configure Dart testing support.
  #set(PROJECT_SOURCE_DIR "${OTBTesting_SOURCE_DIR}/..")
  include(${CMAKE_ROOT}/Modules/Dart.cmake)
  #set(PROJECT_SOURCE_DIR "${OTBTesting_SOURCE_DIR}")
  mark_as_advanced(TCL_TCLSH)
  if(BUILD_TESTING)
    enable_testing()
  endif()

  # Find the data directory.
  find_path(OTB_DATA_ROOT OTBData.readme ${OTBTesting_SOURCE_DIR}/Data $ENV{OTB_DATA_ROOT})
  mark_as_advanced(OTB_DATA_ROOT)

  option(OTB_DATA_USE_LARGEINPUT "Use Large inputs images test." OFF)
  mark_as_advanced(OTB_DATA_USE_LARGEINPUT)
  if(OTB_DATA_USE_LARGEINPUT)
    find_path(OTB_DATA_LARGEINPUT_ROOT OTBData.readme $ENV{OTB_DATA_LARGEINPUT_ROOT})
    mark_as_advanced(OTB_DATA_LARGEINPUT_ROOT)
  endif()

  option(OTB_DATA_USE_SPOTPHRINPUT "Use PHR data provided by SPOT." OFF)
  mark_as_advanced(OTB_DATA_USE_SPOTPHRINPUT)
  if(OTB_DATA_USE_SPOTPHRINPUT)
    find_path(OTB_DATA_SPOTPHRINPUT_ROOT OTB_PHR_Data.readme $ENV{OTB_DATA_SPOTPHRINPUT_ROOT})
    mark_as_advanced(OTB_DATA_SPOTPHRINPUT_ROOT)
  endif()

  include(${CMAKE_CURRENT_SOURCE_DIR}/TestingHelper.cmake)

  option(OTB_DATA_USE_CNESPHRINPUT "Use PHR data provided by ICC." OFF)
  mark_as_advanced(OTB_DATA_USE_CNESPHRINPUT)
  if(OTB_DATA_USE_CNESPHRINPUT)
    find_path(OTB_DATA_CNESPHRINPUT_ROOT OTB_PHR_Data.readme $ENV{OTB_DATA_CNESPHRINPUT_ROOT})
    mark_as_advanced(OTB_DATA_CNESPHRINPUT_ROOT)
  endif()

  macro(OTB_ADD_EXECUTABLE name sources libraries)
    add_executable(${name} ${sources})
    target_link_libraries(${name}  ${libraries})
    if(OTB_CPP_CHECK_SOURCE_FILE)
      foreach( file ${sources})
        add_custom_command( TARGET ${name}
          PRE_BUILD
          COMMAND ${OTB_CPP_CHECK_PROG}
          ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
          DEPENDS ${libraries}
          COMMENT "============== C++ checking ${file} source file...  =================="
          VERBATIM )
      endforeach()
    endif()
  endmacro()

  # Find OTB and load its settings.
  find_package(OTB PATHS /usr/lib/otb)
  if(OTB_FOUND)
    include(${OTB_USE_FILE})
    add_subdirectory(Code)
  else()
    message(FATAL_ERROR "OTB not found.  Please set OTB_DIR")
  endif()

  configure_file(../CMake/CTestCustom.cmake.in CTestCustom.cmake)

  #Prevents conflicts with visual min/max function
  if(WIN32)
    add_definitions(-DNOMINMAX)
  endif()

endif()

if(OTB_COMPILE_WITH_FULL_WARNING)
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OTB_FULL_WARNING_LIST_PARAMETERS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OTB_FULL_WARNING_LIST_PARAMETERS}")
  endif()
endif()

