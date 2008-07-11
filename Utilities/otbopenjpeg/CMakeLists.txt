# Main CMakeLists.txt to build the OpenJPEG project using CMake (www.cmake.org)
# Written by Mathieu Malaterre

# This CMake project will by default create a library called openjpeg
# But if you want to use this project within your own (CMake) project
# you will eventually like to prefix the library to avoid linking confusion
# For this purpose you can define a CMake var: OPENJPEG_NAMESPACE to whatever you like
# e.g.:
SET(OPENJPEG_NAMESPACE "OTBOPENJPEG")
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# Pas d'openjpeg en standalone
#IF(NOT OPENJPEG_NAMESPACE)
#  SET(OPENJPEG_NAMESPACE "OPENJPEG")
#  SET(OPENJPEG_STANDALONE 1)
#ENDIF(NOT OPENJPEG_NAMESPACE)
# In all cases:
STRING(TOLOWER ${OPENJPEG_NAMESPACE} OPENJPEG_LIBRARY_NAME)

PROJECT(${OPENJPEG_NAMESPACE} C)

# Do full dependency headers.
INCLUDE_REGULAR_EXPRESSION("^.*$")

#-----------------------------------------------------------------------------
# OPENJPEG version number, useful for packaging and doxygen doc:
SET(OPENJPEG_VERSION_MAJOR 1)
SET(OPENJPEG_VERSION_MINOR 2)
SET(OPENJPEG_VERSION_BUILD 0)
SET(OPENJPEG_VERSION
  "${OPENJPEG_VERSION_MAJOR}.${OPENJPEG_VERSION_MINOR}.${OPENJPEG_VERSION_BUILD}")

# This setting of SOVERSION assumes that any API change
# will increment either the minor or major version number of openjpeg
SET(OPENJPEG_LIBRARY_PROPERTIES
  VERSION   "${OPENJPEG_VERSION_MAJOR}.${OPENJPEG_VERSION_MINOR}.${OPENJPEG_VERSION_BUILD}"
  SOVERSION "${OPENJPEG_VERSION_MAJOR}.${OPENJPEG_VERSION_MINOR}"
)

#-----------------------------------------------------------------------------
# Test for some required system information.
INCLUDE (${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityC.cmake)

#-----------------------------------------------------------------------------
# OpenJPEG build configuration options.
#OPTION(ENABLE_PROFILING "Enable profiling for the library" OFF)

#-----------------------------------------------------------------------------
SET (EXECUTABLE_OUTPUT_PATH ${OTB_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
SET (LIBRARY_OUTPUT_PATH ${OTB_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)


#-----------------------------------------------------------------------------
# For the codec...
#OPTION(BUILD_EXAMPLES "Build the Examples (codec...)." OFF)


# configure name mangling to allow multiple libraries to coexist
# peacefully
IF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/openjpeg_mangle.h.in)
SET(MANGLE_PREFIX ${OPENJPEG_LIBRARY_NAME})
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/openjpeg_mangle.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/openjpeg_mangle.h
               @ONLY IMMEDIATE)
ENDIF(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/openjpeg_mangle.h.in)

#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/opj_configure.h.in 
               ${CMAKE_CURRENT_BINARY_DIR}/opj_configure.h)

#-----------------------------------------------------------------------------
#     IF(OTB_BUILD_SHARED_LIBS)
#         ADD_DEFINITIONS(-DOPJ_EXPORTS)
#     ELSE(OTB_BUILD_SHARED_LIBS)
#         ADD_DEFINITIONS(-DOPJ_STATIC)
#     ENDIF(OTB_BUILD_SHARED_LIBS)

#     IF(CYGWIN OR MINGW)
#       ADD_DEFINITIONS(-DWIN32)  
#     ENDIF(CYGWIN OR MINGW)

#-----------------------------------------------------------------------------
# Always build the library
INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
SUBDIRS(libopenjpeg)

#-----------------------------------------------------------------------------
# For the documentation
OPTION(BUILD_DOCUMENTATION "Build the doxygen documentation" OFF)
IF(BUILD_DOCUMENTATION)
  SUBDIRS(doc)
ENDIF(BUILD_DOCUMENTATION)

#-----------------------------------------------------------------------------
# For openjpeg team if they ever want Dart+CMake
#IF(OPENJPEG_STANDALONE)
#  INCLUDE(Dart)
#  MARK_AS_ADVANCED(BUILD_TESTING DART_ROOT TCL_TCLSH)
#  IF(BUILD_TESTING)
#    ENABLE_TESTING()
#    SET(BUILDNAME "OpenJPEG-${CMAKE_SYSTEM}-${CMAKE_C_COMPILER}" CACHE STRING "Name of build on the dashboard")
#    MARK_AS_ADVANCED(BUILDNAME)
#  ENDIF(BUILD_TESTING)
#ENDIF(OPENJPEG_STANDALONE)

# Adding test with dataset from:
# http://www.crc.ricoh.com/~gormish/jpeg2000conformance/
# http://www.jpeg.org/jpeg2000guide/testimages/testimages.html

#-----------------------------------------------------------------------------
# Compiler specific flags:
IF(CMAKE_COMPILER_IS_GNUCC)
  # For all builds, make sure openjpeg is std99 compliant:
  SET(CMAKE_C_FLAGS "-Wall -std=c99 ${CMAKE_C_FLAGS}")
  # Do not use ffast-math for all build, it would produce incorrect results, only set for release:
  SET(CMAKE_C_FLAGS_RELEASE "-ffast-math ${CMAKE_C_FLAGS_RELEASE}")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

IF(_MSC_VER)
  STRING_APPEND( CMAKE_EXE_LINKER_FLAGS_DEBUG      "/NODEFAULTLIB:\"libcmtd.lib\"")
  STRING_APPEND( CMAKE_EXE_LINKER_FLAGS_RELEASE    "/NODEFAULTLIB:\"libcmt.lib\"")
  STRING_APPEND( CMAKE_SHARED_LINKER_FLAGS_DEBUG   "/NODEFAULTLIB:\"libcmtd.lib\"")
  STRING_APPEND( CMAKE_SHARED_LINKER_FLAGS_RELEASE "/NODEFAULTLIB:\"libcmt.lib\"")
  # make changes persistent
  SET(CMAKE_EXE_LINKER_FLAGS_DEBUG   ${CMAKE_EXE_LINKER_FLAGS_DEBUG}
    CACHE STRING "Debug builds linker flags"   FORCE )
  SET(CMAKE_EXE_LINKER_FLAGS_RELEASE ${CMAKE_EXE_LINKER_FLAGS_RELEASE}
    CACHE STRING "Release builds linker flags" FORCE )
  SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG ${CMAKE_SHARED_LINKER_FLAGS_DEBUG}
	CACHE STRING "flags for linking a DLL in Debug build" FORCE)
  SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE ${CMAKE_SHARED_LINKER_FLAGS_RELEASE}
	CACHE STRING "flags for linking a DLL in Debug build" FORCE)  
ENDIF(_MSC_VER)

#========================================
# OTB added
IF(CYGWIN OR MINGW)
  ADD_DEFINITIONS(-DWIN32)
ENDIF(CYGWIN OR MINGW)
