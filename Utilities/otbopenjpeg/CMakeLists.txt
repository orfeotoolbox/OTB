# Main CMakeLists.txt to build the OpenJPEG project using CMake (www.cmake.org)
# Written by Mathieu Malaterre

# This CMake project will by default create a library called openjpeg
# But if you want to use this project within your own (CMake) project
# you will eventually like to prefix the library to avoid linking confusion
# For this purpose you can define a CMake var: OPENJPEG_NAMESPACE to whatever you like
# e.g.:
# set(OPENJPEG_NAMESPACE "GDCMOPENJPEG")
cmake_minimum_required(VERSION 2.6)

if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)

if(NOT OPENJPEG_NAMESPACE)
  set(OPENJPEG_NAMESPACE "OPENJPEG")
  set(OPENJPEG_STANDALONE 1)
endif(NOT OPENJPEG_NAMESPACE)
# In all cases:
string(TOLOWER ${OPENJPEG_NAMESPACE} OPENJPEG_LIBRARY_NAME)

project(${OPENJPEG_NAMESPACE} C)

# Do full dependency headers.
include_regular_expression("^.*$")

#-----------------------------------------------------------------------------
# OPENJPEG version number, useful for packaging and doxygen doc:
set(OPENJPEG_VERSION_MAJOR 1)
set(OPENJPEG_VERSION_MINOR 99)
set(OPENJPEG_VERSION_BUILD 0)
set(OPENJPEG_VERSION
  "${OPENJPEG_VERSION_MAJOR}.${OPENJPEG_VERSION_MINOR}.${OPENJPEG_VERSION_BUILD}")
set(PACKAGE_VERSION
  "${OPENJPEG_VERSION_MAJOR}.${OPENJPEG_VERSION_MINOR}.${OPENJPEG_VERSION_BUILD}")

# svn revision number of this openjpeg fork (used for .so versioning)
set(OPENJPEG_SVN_REVISION "1111")
# patch number for OTB modifications applied on top of OPENJPEG_SVN_REVISION (used for .so versioning)
set(OPENJPEG_OTB_PATCH "1")

set(OPENJPEG_LIBRARY_PROPERTIES
  VERSION   "${OPENJPEG_SVN_REVISION}.${OPENJPEG_OTB_PATCH}.0"
  SOVERSION "${OPENJPEG_SVN_REVISION}.${OPENJPEG_OTB_PATCH}"
)

# --------------------------------------------------------------------------
# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${${OPENJPEG_NAMESPACE}_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})

# --------------------------------------------------------------------------
# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
if(WIN32)
  if(NOT BORLAND)
    if(NOT CYGWIN)
      if(NOT MINGW)
        if(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
          add_definitions(
            -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
            -D_CRT_IS_WCTYPE_NO_DEPRECATE
            -D_CRT_MANAGED_FP_NO_DEPRECATE
            -D_CRT_NONSTDC_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE
            -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
            -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
            -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
            -D_CRT_VCCLRIT_NO_DEPRECATE
            -D_SCL_SECURE_NO_DEPRECATE
            )
        endif(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
      endif(NOT MINGW)
    endif(NOT CYGWIN)
  endif(NOT BORLAND)
endif(WIN32)


# --------------------------------------------------------------------------
# Install directories

string(TOLOWER ${PROJECT_NAME} projectname)
set(OPENJPEG_INSTALL_SUBDIR "${projectname}-${OPENJPEG_VERSION_MAJOR}.${OPENJPEG_VERSION_MINOR}")

if(NOT OPENJPEG_INSTALL_BIN_DIR)
  set(OPENJPEG_INSTALL_BIN_DIR "bin")
endif(NOT OPENJPEG_INSTALL_BIN_DIR)

if(NOT OPENJPEG_INSTALL_LIB_DIR)
  set(OPENJPEG_INSTALL_LIB_DIR "lib")
endif(NOT OPENJPEG_INSTALL_LIB_DIR)

if(NOT OPENJPEG_INSTALL_DATA_DIR)
  set(OPENJPEG_INSTALL_DATA_DIR "share/${OPENJPEG_INSTALL_SUBDIR}")
endif(NOT OPENJPEG_INSTALL_DATA_DIR)

if(NOT OPENJPEG_INSTALL_INCLUDE_DIR)
  set(OPENJPEG_INSTALL_INCLUDE_DIR "include/")
endif(NOT OPENJPEG_INSTALL_INCLUDE_DIR)

if(NOT OPENJPEG_INSTALL_MAN_DIR)
  set(OPENJPEG_INSTALL_MAN_DIR "share/man/")
endif(NOT OPENJPEG_INSTALL_MAN_DIR)

if(NOT OPENJPEG_INSTALL_DOC_DIR)
  set(OPENJPEG_INSTALL_DOC_DIR "share/doc/${OPENJPEG_INSTALL_SUBDIR}")
endif(NOT OPENJPEG_INSTALL_DOC_DIR)

if(NOT OPENJPEG_INSTALL_PACKAGE_DIR)
  set(OPENJPEG_INSTALL_PACKAGE_DIR ${OPENJPEG_INSTALL_LIB_DIR}/${OPENJPEG_INSTALL_SUBDIR}
    CACHE INTERNAL "")
endif(NOT OPENJPEG_INSTALL_PACKAGE_DIR)

#-----------------------------------------------------------------------------
# Test for some required system information.
include(${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityC.cmake)

# Big endian test:
include(${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
TEST_BIG_ENDIAN(OPJ_BIG_ENDIAN)

#-----------------------------------------------------------------------------
# Setup file for setting custom ctest vars
configure_file(
  ${${OPENJPEG_NAMESPACE}_SOURCE_DIR}/CMake/CTestCustom.cmake.in
  ${${OPENJPEG_NAMESPACE}_BINARY_DIR}/CTestCustom.cmake
  @ONLY
  )

#-----------------------------------------------------------------------------
# OpenJPEG build configuration options.
option(BUILD_SHARED_LIBS "Build OpenJPEG shared library and link executables against it." ON)
set(EXECUTABLE_OUTPUT_PATH ${OPENJPEG_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables.")
set(LIBRARY_OUTPUT_PATH ${OPENJPEG_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries.")
mark_as_advanced(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)

#-----------------------------------------------------------------------------
# configure name mangling to allow multiple libraries to coexist
# peacefully

#set(OPJ_MANGLE_PREFIX "" CACHE STRING "Prefix used to mangle all the symbols")
if(OPJ_MANGLE_PREFIX)
  set(OPJ_USE_MANGLE_PREFIX ON)
else(OPJ_MANGLE_PREFIX)
  set(OPJ_USE_MANGLE_PREFIX OFF)
endif(OPJ_MANGLE_PREFIX)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openjpeg_mangle.h.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/openjpeg_mangle.h
               @ONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# pkgconfig support
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg1.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/libopenjpeg1.pc @ONLY)
#install( FILES  ${CMAKE_CURRENT_BINARY_DIR}/libopenjpeg1.pc DESTINATION ${OPENJPEG_INSTALL_LIB_DIR}/pkgconfig )
#install( CODE
#  "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \"libopenjpeg1.pc\" \"\$ENV{DESTDIR}${OPENJPEG_INSTALL_LIB_DIR}/pkgconfig/libopenjpeg.pc\")")

#-----------------------------------------------------------------------------
# Compiler specific flags:
if(CMAKE_COMPILER_IS_GNUCC)
  # For all builds, make sure openjpeg is std99 compliant:
  # set(CMAKE_C_FLAGS "-Wall -std=c99 ${CMAKE_C_FLAGS}") # FIXME: this setting prevented us from setting a coverage build.
  # Do not use ffast-math for all build, it would produce incorrect results, only set for release:
  set(CMAKE_C_FLAGS_RELEASE "-ffast-math ${CMAKE_C_FLAGS_RELEASE}")
endif(CMAKE_COMPILER_IS_GNUCC)

#-----------------------------------------------------------------------------
# opj_config.h generation (1/2)
CHECK_INCLUDE_FILE("strings.h"       HAVE_STRINGS_H)
CHECK_INCLUDE_FILE("inttypes.h"       HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE("memory.h"       HAVE_MEMORY_H)
CHECK_INCLUDE_FILE("stdint.h"       HAVE_STDINT_H)
CHECK_INCLUDE_FILE("stdlib.h"       HAVE_STDLIB_H)
CHECK_INCLUDE_FILE("string.h"       HAVE_STRING_H)
CHECK_INCLUDE_FILE("sys/stat.h"       HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE("sys/types.h"       HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE("unistd.h"       HAVE_UNISTD_H)

# Enable Large file support
include(TestLargeFiles)
OPJ_TEST_LARGE_FILES(OPJ_HAVE_LARGEFILES)

#-----------------------------------------------------------------------------
# Build Library
include_directories(BEFORE ${OPENJPEG_BINARY_DIR})
add_subdirectory(libopenjpeg)

#-----------------------------------------------------------------------------
# Build Applications
#option(BUILD_CODEC "Build the CODEC executables" ON)
#option(BUILD_MJ2 "Build the MJ2 executables." OFF)
#option(BUILD_JPIP "Build the JPIP library and executables." OFF)

if(BUILD_CODEC OR BUILD_MJ2 OR BUILD_JPIP)
  option(BUILD_THIRDPARTY "Build the thirdparty executables if it is needed" OFF)
  add_subdirectory(thirdparty)
  add_subdirectory(applications)
endif(BUILD_CODEC OR BUILD_MJ2 OR BUILD_JPIP)

#-----------------------------------------------------------------------------
# opj_config.h generation (2/2)
configure_file("${OPENJPEG_SOURCE_DIR}/opj_config.h.cmake.in"
 "${OPENJPEG_BINARY_DIR}/opj_config.h"
 @ONLY
 )

#-----------------------------------------------------------------------------
# Build JPWL-flavoured library and executables
#option(BUILD_JPWL "Build the JPWL library and executables" OFF)

#-----------------------------------------------------------------------------
# Build DOCUMENTATION (not in ALL target and only if Doxygen is found)
#option(BUILD_DOC "Build the HTML documentation (with doxygen if available)." OFF)
if(BUILD_DOC)
    add_subdirectory(doc)
endif(BUILD_DOC)

#-----------------------------------------------------------------------------
# Buld Testing
#option(BUILD_TESTING "Build the tests." OFF)
if(BUILD_TESTING)
  if(BUILD_CODEC)
    enable_testing()
    include(CTest)

    # Search openjpeg data needed for the tests
    # They could be found via svn on the OpenJPEG google code project
    # svn checkout http://openjpeg.googlecode.com/svn/data (about 70 Mo)
    find_path(OPJ_DATA_ROOT README-OPJ-Data
      PATHS
      $ENV{OPJ_DATA_ROOT}
      ${CMAKE_SOURCE_DIR}/../data
      )

    # For encoding tests we need the path to the ref decoder exe/lib (kakadu)
    find_program(REF_DECODER_BIN
                 NAMES kdu_expand
                 DOC "Kakadu kdu_expand program used in encoder non-regression testing")
    if(REF_DECODER_BIN)
      get_filename_component(REF_DECODER_BIN_PATH ${REF_DECODER_BIN} PATH CACHE)
      mark_as_advanced(REF_DECODER_BIN_PATH)
    endif(REF_DECODER_BIN)

    # Add repository where to find tests
    add_subdirectory(tests)

  else(BUILD_CODEC)
    message(FATAL_ERROR "You need build codec to run the tests")
  endif(BUILD_CODEC)
endif(BUILD_TESTING)

#-----------------------------------------------------------------------------
# install all targets referenced as OPENJPEGTargets
#install(EXPORT OpenJPEGTargets DESTINATION ${OPENJPEG_INSTALL_PACKAGE_DIR})
#configure_file( ${OPENJPEG_SOURCE_DIR}/CMake/OpenJPEGConfig.cmake.in
#  ${OPENJPEG_BINARY_DIR}/OpenJPEGConfig.cmake
#  @ONLY
#)
#install( FILES ${OPENJPEG_BINARY_DIR}/OpenJPEGConfig.cmake
#  DESTINATION ${OPENJPEG_INSTALL_PACKAGE_DIR}
#)

#-----------------------------------------------------------------------------
# install CHANGES and LICENSE
#install(
#  FILES       CHANGES
#              LICENSE
#  DESTINATION ${OPENJPEG_INSTALL_DOC_DIR})
#
