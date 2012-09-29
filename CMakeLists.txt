# This is the root OTB CMakeLists file.
# $Id$
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

#-----------------------------------------------------------------------------
# Sanity check -- prevent in-source builds
#
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/CMake/PreventInSourceBuilds.cmake)

#For the support of cmake 2.6 which does not have VERSION_LESS
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/CMake/CompareVersionStrings.cmake)

PROJECT(OTB)

# Path to additional CMake modules
SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

INCLUDE(CMakeDependentOption)

INCLUDE(SourceStatus)

#-----------------------------------------------------------------------------
# OTB version number.  An even minor number corresponds to releases.
SET(OTB_VERSION_MAJOR "3")
SET(OTB_VERSION_MINOR "15")
SET(OTB_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(OTB_VERSION_STRING "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}")

IF(NOT OTB_NO_LIBRARY_VERSION)
  # This setting of SOVERSION assumes that any API change
  # will increment either the minor or major version number of OTB.
  SET(OTB_LIBRARY_PROPERTIES
      VERSION   "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}.${OTB_VERSION_PATCH}"
      SOVERSION "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
ENDIF(NOT OTB_NO_LIBRARY_VERSION)

#-----------------------------------------------------------------------------
# Test for some required system information.
INCLUDE (${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityC.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityCXX.cmake)

#-----------------------------------------------------------------------------
# Configure Dart testing support.  This should be done before any
# MESSAGE(FATAL_ERROR ...) commands are invoked.

OPTION(BUILD_TESTING  "Build testing." OFF)
OPTION(BUILD_EXAMPLES "Build the Examples directory." OFF)
OPTION(BUILD_APPLICATIONS "Build applications" OFF)
OPTION(BUILD_BUG_TRACKER_TESTS "Build failing tests showing bugs" OFF)
MARK_AS_ADVANCED(BUILD_BUG_TRACKER_TESTS)
INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
CONFIGURE_FILE(CMake/CTestCustom.cmake.in CTestCustom.cmake)
MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
ENABLE_TESTING()
IF(BUILD_TESTING)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

INCLUDE(CommonDefinitions)


#-----------------------------------------------------------------------------
# Option to activate deprecated classes
OPTION(OTB_USE_DEPRECATED "Turn on the use and test of deprecated classes" OFF)
MARK_AS_ADVANCED(OTB_USE_DEPRECATED)

#-----------------------------------------------------------------------------
# SHOW_ALL_MSG_DEBUG option
OPTION(OTB_SHOW_ALL_MSG_DEBUG "OTB developers : Show all messages (debug, dev, etc...) use only for otb developers." OFF)
MARK_AS_ADVANCED(OTB_SHOW_ALL_MSG_DEBUG)

#-----------------------------------------------------------------------------
# Option to define streaming activation in applications
# Use by otbConfigure.h.in
# Note: 8*4000*4000 = 128000000 (double 4000x4000 image)
SET(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING 128000000 CACHE STRING "Image size to activate using streaming for applications.")
MARK_AS_ADVANCED(OTB_STREAM_IMAGE_SIZE_TO_ACTIVATE_STREAMING)
SET(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING 128000000 CACHE STRING "Max size buffer for streaming.")
MARK_AS_ADVANCED(OTB_STREAM_MAX_SIZE_BUFFER_FOR_STREAMING)

#-----------------------------------------------------------------------------
# Option for generate Patented examples
OPTION(OTB_USE_PATENTED "Build patented examples. ITK must be generated whith patented option (ITK_USE_PATENTED = ON)." OFF)
MARK_AS_ADVANCED(OTB_USE_PATENTED)

#
# Setup the option for each laguage
#
OPTION ( OTB_WRAP_PYTHON "Wrap Python" OFF )
OPTION ( OTB_WRAP_JAVA   "Wrap Java"   OFF )
#OPTION ( OTB_WRAP_LUA    "Wrap Lua"    OFF )
#OPTION ( OTB_WRAP_RUBY   "Wrap Ruby"   OFF )
#OPTION ( OTB_WRAP_TCL    "Wrap Tcl"    OFF )
#OPTION ( OTB_WRAP_CSHARP "Wrap CSharp" OFF )
#OPTION ( OTB_WRAP_R      "Wrap R"      OFF )

# Generate Qt based GUI
OPTION ( OTB_WRAP_QT "Enable Qt wrappers" OFF )

# Do not release this yet
#OPTION ( OTB_WRAP_PYQT "Enable PyQt wrappers" OFF )


#-----------------------------------------------------------------------------
# Advanced option to avoid recompiling utilities at each cmake reconfiguration
OPTION(OTB_DISABLE_UTILITIES_COMPILATION "Disable utilities compilation (developpers only - use at you own risk)" OFF)
MARK_AS_ADVANCED(OTB_DISABLE_UTILITIES_COMPILATION)
IF(OTB_DISABLE_UTILITIES_COMPILATION)
  MESSAGE("Warning: utilities compilation is disabled.")
  LINK_DIRECTORIES("${OTB_BINARY_DIR}/bin")
ENDIF(OTB_DISABLE_UTILITIES_COMPILATION)

#-----------------------------------------------------------------------------
# Output directories
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for all ARCHIVE products (static libs, import libs)")
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for all LIBRARY products (so, modules)")
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for all RUNTIME products (executables, dlls)")

# backward compatibility (TODO: remove these deprecated variables)
SET(LIBRARY_OUTPUT_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
SET(OTB_LIBRARY_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
SET(OTB_EXECUTABLE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
SET(CXX_TEST_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

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

IF(NOT OTB_INSTALL_BIN_DIR)
  SET(OTB_INSTALL_BIN_DIR "bin")
ENDIF(NOT OTB_INSTALL_BIN_DIR)

IF(NOT OTB_INSTALL_LIB_DIR)
  SET(OTB_INSTALL_LIB_DIR "lib/otb")
ENDIF(NOT OTB_INSTALL_LIB_DIR)

IF(NOT OTB_INSTALL_APP_DIR)
  SET(OTB_INSTALL_APP_DIR "lib/otb/applications")
ENDIF(NOT OTB_INSTALL_APP_DIR)

IF(NOT OTB_INSTALL_PYTHON_DIR)
  SET(OTB_INSTALL_PYTHON_DIR "lib/otb/python")
ENDIF(NOT OTB_INSTALL_PYTHON_DIR)

IF(NOT OTB_INSTALL_JAVA_DIR)
  SET(OTB_INSTALL_JAVA_DIR "lib/otb/java")
ENDIF(NOT OTB_INSTALL_JAVA_DIR)

IF(NOT OTB_INSTALL_DATA_DIR)
  SET(OTB_INSTALL_DATA_DIR "share/otb")
ENDIF(NOT OTB_INSTALL_DATA_DIR)

IF(NOT OTB_INSTALL_INCLUDE_DIR)
  SET(OTB_INSTALL_INCLUDE_DIR "include/otb")
ENDIF(NOT OTB_INSTALL_INCLUDE_DIR)

IF(NOT OTB_INSTALL_PACKAGE_DIR)
  SET(OTB_INSTALL_PACKAGE_DIR ${OTB_INSTALL_LIB_DIR})
ENDIF(NOT OTB_INSTALL_PACKAGE_DIR)

SET(OTB_INSTALL_CMAKEMODULE_DIR ${OTB_INSTALL_PACKAGE_DIR}/cmakemodules)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  SET(OTB_INSTALL_NO_DEVELOPMENT 0)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

IF(NOT OTB_INSTALL_NO_RUNTIME)
  SET(OTB_INSTALL_NO_RUNTIME 0)
ENDIF(NOT OTB_INSTALL_NO_RUNTIME)

IF(NOT OTB_INSTALL_NO_DOCUMENTATION)
  SET(OTB_INSTALL_NO_DOCUMENTATION 0)
ENDIF(NOT OTB_INSTALL_NO_DOCUMENTATION)

SET(OTB_INSTALL_NO_LIBRARIES)
IF(OTB_BUILD_SHARED_LIBS)
  IF(OTB_INSTALL_NO_RUNTIME AND OTB_INSTALL_NO_DEVELOPMENT)
    SET(OTB_INSTALL_NO_LIBRARIES 1)
  ENDIF(OTB_INSTALL_NO_RUNTIME AND OTB_INSTALL_NO_DEVELOPMENT)
ELSE(OTB_BUILD_SHARED_LIBS)
  IF(OTB_INSTALL_NO_DEVELOPMENT)
    SET(OTB_INSTALL_NO_LIBRARIES 1)
  ENDIF(OTB_INSTALL_NO_DEVELOPMENT)
ENDIF(OTB_BUILD_SHARED_LIBS)

INCLUDE(PreventInstallOnBuildTree)
INCLUDE(CheckCXXSourceRuns)

#-----------------------------------------------------------------------------
# External libraries

FIND_PACKAGE(OSGeo4W)
# Put GDAL first, since on Windows, include dirs for other libs (curl, expat, mapnik)  contain gdal 1.5 includes
INCLUDE(ImportGdal)
INCLUDE(ImportLibLAS) # depends on ImportGdal
INCLUDE(ImportExpat)
INCLUDE(ImportITK)
INCLUDE(ImportJPEG2000)
INCLUDE(ImportFLTK) # depends on ImportGdal
INCLUDE(ImportCurl)
INCLUDE(ImportPqxx)
INCLUDE(ImportBoost)
INCLUDE(ImportMapnik) # depends on ImportBoost
INCLUDE(ImportOpenThreads)
INCLUDE(ImportOssim)
INCLUDE(ImportQt4)
INCLUDE(ImportGetText)
INCLUDE(ImportSiftFast)
INCLUDE(ImportTinyXML)
INCLUDE(ImportMuParser)
INCLUDE(ImportLibKML)


INCLUDE_DIRECTORIES (
  ${OTB_BINARY_DIR}
  ${OTB_INCLUDE_DIRS} )

#-----------------------------------------------------------------------------
# Option for display all developpment message

OPTION(OTB_CPP_CHECK_SOURCE_FILE "OTB developers : Checking cpp source file (use only for otb developers)." OFF)
MARK_AS_ADVANCED(OTB_CPP_CHECK_SOURCE_FILE)
IF(OTB_CPP_CHECK_SOURCE_FILE)
  FIND_PROGRAM(OTB_CPP_CHECK_PROG "")
ENDIF(OTB_CPP_CHECK_SOURCE_FILE)

MACRO(OTB_ADD_EXECUTABLE name sources libraries)
#  MESSAGE(STATUS "name: ${name}")
#  MESSAGE(STATUS "sources: ${sources}")
#  MESSAGE(STATUS "libraries: ${libraries}")
  ADD_EXECUTABLE(${name} ${sources})
  TARGET_LINK_LIBRARIES(${name}  ${libraries})
  IF(OTB_CPP_CHECK_SOURCE_FILE)
    FOREACH( file ${sources})
      ADD_CUSTOM_COMMAND(   TARGET ${name}
                            PRE_BUILD
                            COMMAND ${OTB_CPP_CHECK_PROG}
                            ARGS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
                            DEPENDS ${libraries}
                            COMMENT "============== C++ checking ${file} source file...  =================="
                            VERBATIM )
    ENDFOREACH(file)
  ENDIF(OTB_CPP_CHECK_SOURCE_FILE)
ENDMACRO(OTB_ADD_EXECUTABLE)



#-----------------------------------------------------------------------------
# The entire OTB tree should use the same include path

# Create the list of include directories needed for OTB header files.
INCLUDE(${OTB_SOURCE_DIR}/CMake/otbIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
INCLUDE_DIRECTORIES(
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
)

#-----------------------------------------------------------------------------
# Dispatch the build into the proper subdirectories.
ADD_SUBDIRECTORY(Utilities)
ADD_SUBDIRECTORY(Code)

IF (BUILD_APPLICATIONS)
  ADD_SUBDIRECTORY(Applications)
ENDIF (BUILD_APPLICATIONS)

IF (BUILD_TESTING)
  ADD_SUBDIRECTORY(Testing)
ENDIF (BUILD_TESTING)

IF (BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(Examples)
ENDIF (BUILD_EXAMPLES)

IF(MINGW)
  LINK_LIBRARIES(gdi32)
ENDIF(MINGW)

INCLUDE(CheckTypeSize)

CHECK_TYPE_SIZE("long long" OTB_SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("__int64"   OTB_SIZEOF___INT64)
CHECK_TYPE_SIZE("long int" OTB_SIZEOF_LONG_INT)
CHECK_TYPE_SIZE("short int" OTB_SIZEOF_SHORT_INT)

#-----------------------------------------------------------------------------
# Configure the default OTB_DATA_ROOT for the location of OTB Data.
FIND_PATH(OTB_DATA_ROOT README-OTB-Data PATHS $ENV{OTB_DATA_ROOT} ${OTB_SOURCE_DIR}/../OTB-Data)
MARK_AS_ADVANCED(OTB_DATA_ROOT)

OPTION(OTB_DATA_USE_LARGEINPUT "Use Large inputs images test." OFF)
MARK_AS_ADVANCED(OTB_DATA_USE_LARGEINPUT)
IF(OTB_DATA_USE_LARGEINPUT)
  FIND_PATH(OTB_DATA_LARGEINPUT_ROOT OTBData.readme $ENV{OTB_DATA_LARGEINPUT_ROOT})
  MARK_AS_ADVANCED(OTB_DATA_LARGEINPUT_ROOT)
ENDIF(OTB_DATA_USE_LARGEINPUT)

OPTION(OTB_DATA_USE_SPOTPHRINPUT "Use PHR data provided by SPOT." OFF)
MARK_AS_ADVANCED(OTB_DATA_USE_SPOTPHRINPUT)
IF(OTB_DATA_USE_SPOTPHRINPUT)
  FIND_PATH(OTB_DATA_SPOTPHRINPUT_ROOT OTB_PHR_Data.readme $ENV{OTB_DATA_SPOTPHRINPUT_ROOT})
  MARK_AS_ADVANCED(OTB_DATA_SPOTPHRINPUT_ROOT)
ENDIF(OTB_DATA_USE_SPOTPHRINPUT)

OPTION(OTB_DATA_USE_CNESPHRINPUT "Use PHR data provided by CNES." OFF)
MARK_AS_ADVANCED(OTB_DATA_USE_CNESPHRINPUT)
IF(OTB_DATA_USE_CNESPHRINPUT)
  FIND_PATH(OTB_DATA_CNESPHRINPUT_ROOT OTB_PHR_Data.readme $ENV{OTB_DATA_CNESPHRINPUT_ROOT})
  MARK_AS_ADVANCED(OTB_DATA_CNESPHRINPUT_ROOT)
ENDIF(OTB_DATA_USE_CNESPHRINPUT)

#-----------------------------------------------------------------------------
# Configure files with settings for use by the build
# This must be done after the definition of all options
CONFIGURE_FILE(${OTB_SOURCE_DIR}/CMake/otbConfigure.h.in
               ${OTB_BINARY_DIR}/otbConfigure.h)

#-----------------------------------------------------------------------------
# Uninstall cmake use to uninstall OTB.
#    CONFIGURE_FILE(
#      "${CMAKE_CURRENT_SOURCE_DIR}/CMake/cmake_uninstall.cmake.in"
#      "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#      IMMEDIATE @ONLY)

#    ADD_CUSTOM_TARGET(uninstall
#      "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#-----------------------------------------------------------------------------
# Help other projects use OTB.

# Copy the UseOTB.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/CMake/UseOTB.cmake.in
               ${OTB_BINARY_DIR}/UseOTB.cmake COPYONLY IMMEDIATE)

# Copy the helper macros to the build tree for use from external projects
CONFIGURE_FILE(${OTB_SOURCE_DIR}/CMake/OTBParseArguments.cmake
               ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/OTBParseArguments.cmake COPYONLY IMMEDIATE)
CONFIGURE_FILE(${OTB_SOURCE_DIR}/CMake/OTBWrapperMacros.cmake
               ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/OTBWrapperMacros.cmake COPYONLY IMMEDIATE)

# Save the compiler settings so another project can import them.
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${OTB_BINARY_DIR}/OTBBuildSettings.cmake)

# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${OTB_BINARY_DIR}/OTBLibraryDepends.cmake)

# Create the OTBConfig.cmake file containing the OTB configuration.
INCLUDE (${OTB_SOURCE_DIR}/CMake/otbGenerateOTBConfig.cmake)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES
    ${OTB_BINARY_DIR}/OTBBuildSettings.cmake
    ${OTB_BINARY_DIR}/OTBLibraryDepends.cmake
    ${OTB_BINARY_DIR}/UseOTB.cmake
    ${OTB_SOURCE_DIR}/CMake/OTBParseArguments.cmake
    ${OTB_SOURCE_DIR}/CMake/OTBWrapperMacros.cmake
    DESTINATION ${OTB_INSTALL_PACKAGE_DIR}
    COMPONENT Development)
    
  INSTALL(FILES ${OTB_BINARY_DIR}/otbConfigure.h
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}
    COMPONENT Development)
  
  INSTALL(FILES
    ${OTB_SOURCE_DIR}/CMake/FindFFTW.cmake
    ${OTB_SOURCE_DIR}/CMake/FindEXPAT.cmake
    ${OTB_SOURCE_DIR}/CMake/FindFLTK.cmake
    ${OTB_SOURCE_DIR}/CMake/FindGettext.cmake
    ${OTB_SOURCE_DIR}/CMake/FindGettextLib.cmake
    ${OTB_SOURCE_DIR}/CMake/FindICUUC.cmake
    ${OTB_SOURCE_DIR}/CMake/FindLibKML.cmake
    ${OTB_SOURCE_DIR}/CMake/FindLibLAS.cmake
    ${OTB_SOURCE_DIR}/CMake/FindLTDL.cmake
    ${OTB_SOURCE_DIR}/CMake/FindMercurial.cmake
    ${OTB_SOURCE_DIR}/CMake/FindMuParser.cmake
    ${OTB_SOURCE_DIR}/CMake/FindOpenThreads.cmake
    ${OTB_SOURCE_DIR}/CMake/FindPqxx.cmake
    ${OTB_SOURCE_DIR}/CMake/FindTinyXML.cmake
    DESTINATION ${OTB_INSTALL_CMAKEMODULE_DIR}
    COMPONENT Development)

ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

#-----------------------------------------------------------------------------
# => Comment for OTB 2.6.0: simplification : copy ITK values
# OTB requires special compiler flags on some platforms.
SET(OTB_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS}")
SET(OTB_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS}")
SET(OTB_REQUIRED_LINK_FLAGS "${ITK_REQUIRED_LINK_FLAGS}")

#-----------------------------------------------------------------------------
# Add compiler flags OTB needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OTB_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OTB_REQUIRED_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")

INCLUDE(OTBCPack)
INCLUDE(GenerateConfigProperties)

