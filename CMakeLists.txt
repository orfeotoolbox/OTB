if(BUILD_APPLICATIONS)
  cmake_minimum_required(VERSION 2.8.3)
else()
  cmake_minimum_required(VERSION 2.6)
endif()

if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

#-----------------------------------------------------------------------------
# Sanity check -- prevent in-source builds
#
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/PreventInSourceBuilds.cmake)

#For the support of cmake 2.6 which does not have VERSION_LESS
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CompareVersionStrings.cmake)

project(OTB)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

include(CMakeDependentOption)

include(SourceStatus)

#-----------------------------------------------------------------------------
# OTB version number.  An even minor number corresponds to releases.
set(OTB_VERSION_MAJOR "4")
set(OTB_VERSION_MINOR "2")
set(OTB_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
set(OTB_VERSION_STRING "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}")

if(NOT OTB_NO_LIBRARY_VERSION)
  # This setting of SOVERSION assumes that any API change
  # will increment either the minor or major version number of OTB.
  set(OTB_LIBRARY_PROPERTIES
      VERSION   "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}"
      SOVERSION "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
endif()

#-----------------------------------------------------------------------------
# Test for some required system information.
include(${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityC.cmake)
include(${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityCXX.cmake)

#-----------------------------------------------------------------------------
# Configure Dart testing support.  This should be done before any
# message(FATAL_ERROR ...) commands are invoked.

option(BUILD_TESTING  "Build testing." OFF)
option(BUILD_EXAMPLES "Build the Examples directory." OFF)
option(BUILD_APPLICATIONS "Build applications" ON)
option(BUILD_BUG_TRACKER_TESTS "Build failing tests showing bugs" OFF)
mark_as_advanced(BUILD_BUG_TRACKER_TESTS)
include(${CMAKE_ROOT}/Modules/Dart.cmake)
configure_file(CMake/CTestCustom.cmake.in CTestCustom.cmake)
mark_as_advanced(TCL_TCLSH DART_ROOT)
enable_testing()
if(BUILD_TESTING)
  set(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  mark_as_advanced(BUILDNAME)
endif()

include(CommonDefinitions)


#-----------------------------------------------------------------------------
# Option to activate deprecated classes
option(OTB_USE_DEPRECATED "Turn on the use and test of deprecated classes" OFF)
mark_as_advanced(OTB_USE_DEPRECATED)

#-----------------------------------------------------------------------------
# SHOW_ALL_MSG_DEBUG option
option(OTB_SHOW_ALL_MSG_DEBUG "OTB developers : Show all messages (debug, dev, etc...) use only for otb developers." OFF)
mark_as_advanced(OTB_SHOW_ALL_MSG_DEBUG)

#-----------------------------------------------------------------------------
# Option to define streaming activation in applications
# Use by otbConfigure.h.in
# Note: 8*4000*4000 = 128000000 (double 4000x4000 image)
set(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING 128000000 CACHE STRING "Image size to activate using streaming for applications.")
mark_as_advanced(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING)
set(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING 128000000 CACHE STRING "Max size buffer for streaming.")
mark_as_advanced(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING)

#-----------------------------------------------------------------------------
# Option for generate Patented examples
option(OTB_USE_PATENTED "Build patented examples. ITK must be generated whith patented option (ITK_USE_PATENTED = ON)." OFF)
mark_as_advanced(OTB_USE_PATENTED)

#
# Setup the option for each laguage
#
option( OTB_WRAP_PYTHON "Wrap Python" OFF )
option( OTB_WRAP_JAVA   "Wrap Java"   OFF )
#option( OTB_WRAP_LUA    "Wrap Lua"    OFF )
#option( OTB_WRAP_RUBY   "Wrap Ruby"   OFF )
#option( OTB_WRAP_TCL    "Wrap Tcl"    OFF )
#option( OTB_WRAP_CSHARP "Wrap CSharp" OFF )
#option( OTB_WRAP_R      "Wrap R"      OFF )

# Generate Qt based GUI
option( OTB_WRAP_QT "Enable Qt wrappers" OFF )

# Do not release this yet
#option( OTB_WRAP_PYQT "Enable PyQt wrappers" OFF )

#-----------------------------------------------------------------------------
# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for all ARCHIVE products (static libs, import libs)")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for all LIBRARY products (so, modules)")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for all RUNTIME products (executables, dlls)")

# backward compatibility (TODO: remove these deprecated variables)
set(LIBRARY_OUTPUT_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(OTB_LIBRARY_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(OTB_EXECUTABLE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CXX_TEST_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

#-----------------------------------------------------------------------------
# Installation vars.
# OTB_INSTALL_BIN_DIR          - binary dir (executables)
# OTB_INSTALL_LIB_DIR          - library dir (libs)
# OTB_INSTALL_DATA_DIR         - share dir (say, examples, data, etc)
# OTB_INSTALL_INCLUDE_DIR      - include dir (headers)
# OTB_INSTALL_PACKAGE_DIR      - package/export configuration files
# OTB_INSTALL_NO_DEVELOPMENT   - do not install development files
# OTB_INSTALL_NO_RUNTIME       - do not install runtime files
# OTB_INSTALL_NO_DOCUMENTATION - do not install documentation files

if(NOT OTB_INSTALL_BIN_DIR)
  set(OTB_INSTALL_BIN_DIR "bin")
endif()

if(NOT OTB_INSTALL_LIB_DIR)
  set(OTB_INSTALL_LIB_DIR "lib/otb")
endif()

if(NOT OTB_INSTALL_APP_DIR)
  set(OTB_INSTALL_APP_DIR "${OTB_INSTALL_LIB_DIR}/applications")
endif()

if(NOT OTB_INSTALL_PYTHON_DIR)
  set(OTB_INSTALL_PYTHON_DIR "${OTB_INSTALL_LIB_DIR}/python")
endif()

if(NOT OTB_INSTALL_JAVA_DIR)
  set(OTB_INSTALL_JAVA_DIR "${OTB_INSTALL_LIB_DIR}/java")
endif()

if(NOT OTB_INSTALL_DATA_DIR)
  set(OTB_INSTALL_DATA_DIR "share/otb")
endif()

if(NOT OTB_INSTALL_INCLUDE_DIR)
  set(OTB_INSTALL_INCLUDE_DIR "include/otb")
endif()

if(NOT OTB_INSTALL_PACKAGE_DIR)
  set(OTB_INSTALL_PACKAGE_DIR ${OTB_INSTALL_LIB_DIR})
endif()

set(OTB_INSTALL_CMAKEMODULE_DIR ${OTB_INSTALL_PACKAGE_DIR}/cmakemodules)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  set(OTB_INSTALL_NO_DEVELOPMENT 0)
endif()

if(NOT OTB_INSTALL_NO_RUNTIME)
  set(OTB_INSTALL_NO_RUNTIME 0)
endif()

if(NOT OTB_INSTALL_NO_DOCUMENTATION)
  set(OTB_INSTALL_NO_DOCUMENTATION 0)
endif()

set(OTB_INSTALL_NO_LIBRARIES)
if(OTB_BUILD_SHARED_LIBS)
  if(OTB_INSTALL_NO_RUNTIME AND OTB_INSTALL_NO_DEVELOPMENT)
    set(OTB_INSTALL_NO_LIBRARIES 1)
  endif()
else()
  if(OTB_INSTALL_NO_DEVELOPMENT)
    set(OTB_INSTALL_NO_LIBRARIES 1)
  endif()
endif()

include(PreventInstallOnBuildTree)
include(CheckCXXSourceRuns)

#-----------------------------------------------------------------------------
# External libraries

set(CMAKE_TRY_COMPILE_CONFIGURATION Release)

find_package(OSGeo4W)
# Put GDAL first, since on Windows, include dirs for other libs (curl, expat, mapnik)  contain gdal 1.5 includes
include(ImportGdal)
include(ImportExpat)
include(ImportITK)
include(ImportJPEG2000)
include(ImportCurl)
include(ImportBoost)
include(ImportMapnik) # depends on ImportBoost
include(ImportOpenThreads)
include(ImportOssim)
include(ImportQt4)
include(ImportSiftFast)
include(ImportTinyXML)
include(ImportMuParser)
include(ImportLibKML)
include(ImportOpenCV)
#-----------------------------------------------------------------------------
# Option for display all developpment message

option(OTB_CPP_CHECK_SOURCE_FILE "OTB developers : Checking cpp source file (use only for otb developers)." OFF)
mark_as_advanced(OTB_CPP_CHECK_SOURCE_FILE)
if(OTB_CPP_CHECK_SOURCE_FILE)
  find_program(OTB_CPP_CHECK_PROG "")
endif()

macro(OTB_ADD_EXECUTABLE name sources libraries)
#  message(STATUS "name: ${name}")
#  message(STATUS "sources: ${sources}")
#  message(STATUS "libraries: ${libraries}")
  add_executable(${name} ${sources})
  target_link_libraries(${name}  ${libraries})
  if(OTB_CPP_CHECK_SOURCE_FILE)
    foreach( file ${sources})
      add_custom_command(   TARGET ${name}
                            PRE_BUILD
                            COMMAND ${OTB_CPP_CHECK_PROG}
                            ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
                            DEPENDS ${libraries}
                            COMMENT "============== C++ checking ${file} source file...  =================="
                            VERBATIM )
    endforeach()
  endif()
endmacro()



#-----------------------------------------------------------------------------
# The entire OTB tree should use the same include path

# Create the list of include directories needed for OTB header files.
include(${OTB_SOURCE_DIR}/CMake/otbIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
include_directories(
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
)

#-----------------------------------------------------------------------------
# Dispatch the build into the proper subdirectories.
add_subdirectory(Utilities)

# Before building OTB specific code, add include dirs from ITK
# which are deduced after having configured ITK
include_directories(${ITK_INCLUDE_DIRS})
# Add these paths to OTB_INCLUDE_DIRS_BUILD_TREE since it is used to
# generate OTBConfig.cmake
list(APPEND OTB_INCLUDE_DIRS_BUILD_TREE ${ITK_INCLUDE_DIRS})

add_subdirectory(Code)

if(BUILD_APPLICATIONS)
  add_subdirectory(Applications)
endif()

if(BUILD_TESTING)
  add_subdirectory(Testing)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(Examples)
endif()

if(MINGW)
  link_libraries(gdi32)
endif()

include(CheckTypeSize)

CHECK_TYPE_SIZE("long long" OTB_SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("__int64"   OTB_SIZEOF___INT64)
CHECK_TYPE_SIZE("long int" OTB_SIZEOF_LONG_INT)
CHECK_TYPE_SIZE("short int" OTB_SIZEOF_SHORT_INT)

#-----------------------------------------------------------------------------
# Configure the default OTB_DATA_ROOT for the location of OTB Data.
find_path(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB_SOURCE_DIR}/../OTB-Data)
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

option(OTB_DATA_USE_CNESPHRINPUT "Use PHR data provided by CNES." OFF)
mark_as_advanced(OTB_DATA_USE_CNESPHRINPUT)
if(OTB_DATA_USE_CNESPHRINPUT)
  find_path(OTB_DATA_CNESPHRINPUT_ROOT OTB_PHR_Data.readme $ENV{OTB_DATA_CNESPHRINPUT_ROOT})
  mark_as_advanced(OTB_DATA_CNESPHRINPUT_ROOT)
endif()

#-----------------------------------------------------------------------------
# Configure files with settings for use by the build
# This must be done after the definition of all options
configure_file(${OTB_SOURCE_DIR}/CMake/otbConfigure.h.in
               ${OTB_BINARY_DIR}/otbConfigure.h)

#-----------------------------------------------------------------------------
# Uninstall cmake use to uninstall OTB.
#    configure_file(
#      "${CMAKE_CURRENT_SOURCE_DIR}/CMake/cmake_uninstall.cmake.in"
#      "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#      IMMEDIATE @ONLY)

#    add_custom_target(uninstall
#      "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#-----------------------------------------------------------------------------
# Help other projects use OTB.

# Copy the UseOTB.cmake file to the binary tree for backward compatability.
configure_file(${OTB_SOURCE_DIR}/CMake/UseOTB.cmake.in
               ${OTB_BINARY_DIR}/UseOTB.cmake COPYONLY IMMEDIATE)

# Copy the helper macros to the build tree for use from external projects
configure_file(${OTB_SOURCE_DIR}/CMake/OTBWrapperMacros.cmake
               ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/OTBWrapperMacros.cmake COPYONLY IMMEDIATE)

# Save the compiler settings so another project can import them.
include(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${OTB_BINARY_DIR}/OTBBuildSettings.cmake)

# Save library dependencies.
export_library_dependencies(${OTB_BINARY_DIR}/OTBLibraryDepends.cmake)

# Create the OTBConfig.cmake file containing the OTB configuration.
include(${OTB_SOURCE_DIR}/CMake/otbGenerateOTBConfig.cmake)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  install(FILES
    ${OTB_BINARY_DIR}/OTBBuildSettings.cmake
    ${OTB_BINARY_DIR}/OTBLibraryDepends.cmake
    ${OTB_BINARY_DIR}/UseOTB.cmake
    ${OTB_SOURCE_DIR}/CMake/OTBWrapperMacros.cmake
    DESTINATION ${OTB_INSTALL_PACKAGE_DIR}
    COMPONENT Development)

  install(FILES ${OTB_BINARY_DIR}/otbConfigure.h
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}
    COMPONENT Development)

  install(FILES
    ${OTB_SOURCE_DIR}/CMake/FindFFTW.cmake
    ${OTB_SOURCE_DIR}/CMake/FindEXPAT.cmake
    ${OTB_SOURCE_DIR}/CMake/FindICUUC.cmake
    ${OTB_SOURCE_DIR}/CMake/FindLibKML.cmake
    ${OTB_SOURCE_DIR}/CMake/FindLTDL.cmake
    ${OTB_SOURCE_DIR}/CMake/FindMercurial.cmake
    ${OTB_SOURCE_DIR}/CMake/FindMuParser.cmake
    ${OTB_SOURCE_DIR}/CMake/FindOpenThreads.cmake
    ${OTB_SOURCE_DIR}/CMake/FindTinyXML.cmake
    DESTINATION ${OTB_INSTALL_CMAKEMODULE_DIR}
    COMPONENT Development)

endif()

#-----------------------------------------------------------------------------
# => Comment for OTB 2.6.0: simplification : copy ITK values
# OTB requires special compiler flags on some platforms.
set(OTB_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS}")
set(OTB_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS}")
set(OTB_REQUIRED_LINK_FLAGS "${ITK_REQUIRED_LINK_FLAGS}")

#-----------------------------------------------------------------------------
# Add compiler flags OTB needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OTB_REQUIRED_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OTB_REQUIRED_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")

include(OTBCPack)
include(GenerateConfigProperties)

