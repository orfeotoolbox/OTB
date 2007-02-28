# This is the root ITK CMakeLists file.
CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(ITK)

#-----------------------------------------------------------------------------
# ITK version number.  An even minor number corresponds to releases.
SET(ITK_VERSION_MAJOR "3")
SET(ITK_VERSION_MINOR "0")
SET(ITK_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(ITK_VERSION_STRING "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}")

#-----------------------------------------------------------------------------
# Load some macros.
INCLUDE(${ITK_SOURCE_DIR}/Utilities/itkThirdParty.cmake)


SET(CMAKE_MODULE_PATH ${ITK_SOURCE_DIR}/CMake)
# Deal with FFT v3 Options
MARK_AS_ADVANCED(USE_FFTWD)
OPTION(USE_FFTWD "Use double precision FFTW if found" OFF)
MARK_AS_ADVANCED(USE_FFTWF)
OPTION(USE_FFTWF "Use single precision FFTW if found" OFF)
IF( USE_FFTWD OR USE_FFTWF )
    FIND_PACKAGE( FFTW )
ENDIF( USE_FFTWD OR USE_FFTWF )

# Deal with SGI SCSL Options
OPTION(USE_SCSL "Use the SGI SCSL High Performance Computing math library" OFF)
MARK_AS_ADVANCED(USE_SCSL)
IF( USE_SCSL )
    FIND_PACKAGE( SCSL )
ENDIF( USE_SCSL )

# Put "." in the rpath of all ITK shared libraries.
LINK_DIRECTORIES(.)

#-----------------------------------------------------------------------------
# ITK build configuration options.
# OPTION(BUILD_SHARED_LIBS "Build ITK with shared libraries." OFF)
# SET(ITK_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

# IF(NOT ITK_NO_LIBRARY_VERSION)
  # This setting of SOVERSION assumes that any API change
  # will increment either the minor or major version number of ITK.
#   SET(ITK_LIBRARY_PROPERTIES 
#       VERSION   "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}.${ITK_VERSION_PATCH}"
#       SOVERSION "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}"
#      )
# ENDIF(NOT ITK_NO_LIBRARY_VERSION)

#-----------------------------------------------------------------------------
# ITK build patented algorithms
OPTION(ITK_USE_PATENTED "Build algorithms that are covered with Patents. It is YOUR responsibility to obtain a license from the patent holder in order to use these methods." OFF)
MARK_AS_ADVANCED(ITK_USE_PATENTED)

#-----------------------------------------------------------------------------
# ITK build classes that are in the review process
OPTION(ITK_USE_REVIEW "Build algorithms that are candidates to be included in the Toolkit. This classes are not covered by the Backward Compatibility Policy, nor the Copyright Policy of ITK." OFF)
MARK_AS_ADVANCED(ITK_USE_REVIEW)

#-----------------------------------------------------------------------------
# ITK uses KWStyle for checking the coding style
OPTION(ITK_USE_KWSTYLE "Run KWStyle in order to check for violations of the coding standard." OFF)
MARK_AS_ADVANCED(ITK_USE_KWSTYLE)

IF(ITK_USE_KWSTYLE)
  FIND_PROGRAM(KWSTYLE_EXECUTABLE
  NAMES KWStyle 
  PATHS
  /usr/local/bin
  )
  MARK_AS_ADVANCED(KWSTYLE_EXECUTABLE)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${ITK_BINARY_DIR}/KWStyleReport.txt
    COMMAND ${KWSTYLE_EXECUTABLE}
    ARGS    -xml ${ITK_BINARY_DIR}/ITK.kws.xml -o ${ITK_SOURCE_DIR}/Utilities/KWStyle/ITKOverwrite.txt -vim -D ${ITK_BINARY_DIR}/ITKFiles.txt  
    COMMENT "Coding Style Checker"
    )
  ADD_CUSTOM_TARGET(StyleCheck DEPENDS ${ITK_BINARY_DIR}/KWStyleReport.txt)
ENDIF(ITK_USE_KWSTYLE)

#-----------------------------------------------------------------------------
# ITK turn on concept checking
IF( NOT BORLAND )
  OPTION(ITK_USE_CONCEPT_CHECKING "Turn on concept checking to give helpful errors at compile time if a type cannot be used as a template parameter." OFF)
  MARK_AS_ADVANCED(ITK_USE_CONCEPT_CHECKING)
ELSE( NOT BORLAND )
  SET(ITK_USE_CONCEPT_CHECKING OFF)
  MARK_AS_ADVANCED(ITK_USE_CONCEPT_CHECKING)
ENDIF( NOT BORLAND )

#-----------------------------------------------------------------------------
# Installation vars.
# ITK_INSTALL_BIN_DIR          - binary dir (executables)
# ITK_INSTALL_LIB_DIR          - library dir (libs)
# ITK_INSTALL_DATA_DIR         - share dir (say, examples, data, etc)
# ITK_INSTALL_INCLUDE_DIR      - include dir (headers)
# ITK_INSTALL_PACKAGE_DIR      - package/export configuration files
# ITK_INSTALL_NO_DEVELOPMENT   - do not install development files
# ITK_INSTALL_NO_RUNTIME       - do not install runtime files
# ITK_INSTALL_NO_DOCUMENTATION - do not install documentation files

IF(NOT ITK_INSTALL_BIN_DIR)
  SET(ITK_INSTALL_BIN_DIR "/bin")
ENDIF(NOT ITK_INSTALL_BIN_DIR)

IF(NOT ITK_INSTALL_LIB_DIR)
#  SET(ITK_INSTALL_LIB_DIR "/lib/InsightToolkit")
  SET(ITK_INSTALL_LIB_DIR "/lib/otb")
ENDIF(NOT ITK_INSTALL_LIB_DIR)

IF(NOT ITK_INSTALL_DATA_DIR)
  SET(ITK_INSTALL_DATA_DIR "/share/otb/Utilities/ITK")
ENDIF(NOT ITK_INSTALL_DATA_DIR)

IF(NOT ITK_INSTALL_INCLUDE_DIR)
  SET(ITK_INSTALL_INCLUDE_DIR "/include/otb/Utilities/ITK")
ENDIF(NOT ITK_INSTALL_INCLUDE_DIR)

IF(NOT ITK_INSTALL_PACKAGE_DIR)
  SET(ITK_INSTALL_PACKAGE_DIR ${ITK_INSTALL_LIB_DIR}
    CACHE INTERNAL "")
ENDIF(NOT ITK_INSTALL_PACKAGE_DIR)

IF(NOT ITK_INSTALL_NO_DEVELOPMENT)
  SET(ITK_INSTALL_NO_DEVELOPMENT 0)
ENDIF(NOT ITK_INSTALL_NO_DEVELOPMENT)

IF(NOT ITK_INSTALL_NO_RUNTIME)
  SET(ITK_INSTALL_NO_RUNTIME 0)
ENDIF(NOT ITK_INSTALL_NO_RUNTIME)

IF(NOT ITK_INSTALL_NO_DOCUMENTATION)
  SET(ITK_INSTALL_NO_DOCUMENTATION 0)
ENDIF(NOT ITK_INSTALL_NO_DOCUMENTATION)

SET(ITK_INSTALL_NO_LIBRARIES)
IF(ITK_BUILD_SHARED_LIBS)
  IF(ITK_INSTALL_NO_RUNTIME AND ITK_INSTALL_NO_DEVELOPMENT)
    SET(ITK_INSTALL_NO_LIBRARIES 1)
  ENDIF(ITK_INSTALL_NO_RUNTIME AND ITK_INSTALL_NO_DEVELOPMENT)
ELSE(ITK_BUILD_SHARED_LIBS)
  IF(ITK_INSTALL_NO_DEVELOPMENT)
    SET(ITK_INSTALL_NO_LIBRARIES 1)
  ENDIF(ITK_INSTALL_NO_DEVELOPMENT)
ENDIF(ITK_BUILD_SHARED_LIBS)

# Because INSTALL_* commands require a leading / and because INSTALL (cmake 2.4
# and newer) requires no leading / to install under INSTALL_PREFIX, we
# are stripping the leading /. In the future, there should be no leading
# / in any install directory variables
STRING(REGEX REPLACE "^/" "" ITK_INSTALL_LIB_DIR_CM24 "${ITK_INSTALL_LIB_DIR}")
STRING(REGEX REPLACE "^/" "" ITK_INSTALL_BIN_DIR_CM24 "${ITK_INSTALL_BIN_DIR}")
STRING(REGEX REPLACE "^/" "" ITK_INSTALL_INCLUDE_DIR_CM24 "${ITK_INSTALL_INCLUDE_DIR}")
STRING(REGEX REPLACE "^/" "" ITK_INSTALL_PACKAGE_DIR_CM24 "${ITK_INSTALL_PACKAGE_DIR}")
STRING(REGEX REPLACE "^/" "" ITK_INSTALL_DOXYGEN_DIR_CM24 "${ITK_INSTALL_DOXYGEN_DIR}")
STRING(REGEX REPLACE "^/" "" ITK_INSTALL_TCL_DIR_CM24 "${ITK_INSTALL_TCL_DIR}")
STRING(REGEX REPLACE "^/" "" ITK_INSTALL_JAVA_DIR_CM24 "${ITK_INSTALL_JAVA_DIR}")

# set ITK_DIR so it can be used by subprojects
SET(ITK_DIR "${CMAKE_BINARY_DIR}" CACHE INTERNAL "ITK dir to be used by subprojects")

#-----------------------------------------------------------------------------
# Perform a setup of ITK Wrapping. This will provide CMake options for
# individual wrapping as well as determine if CableSwig is required. If it is,
# ITK_NEED_CableSwig will be set. This file also tries to locate CableSwig by
# searching first in the source tree of ITK, and if that fails, it searches
# for a binary built of CableSwig.
#
# INCLUDE(${ITK_SOURCE_DIR}/Wrapping/itkWrapSetup.cmake)


IF(WIN32)
   IF(BUILD_SHARED_LIBS)
     SET(ITK_COMMON_BUILD_TYPE "SHARED")
   ELSE(BUILD_SHARED_LIBS)
     SET(ITK_COMMON_BUILD_TYPE "STATIC")
   ENDIF(BUILD_SHARED_LIBS)
   SET(BUILD_SHARED_LIBS OFF)
ENDIF(WIN32)


OPTION(ITK_USE_SYSTEM_VXL "Use an outside build of VXL." OFF)
MARK_AS_ADVANCED(ITK_USE_SYSTEM_VXL)
IF(ITK_USE_SYSTEM_VXL)
  FIND_PACKAGE(VXL)
  IF(VXL_FOUND)
    INCLUDE(${VXL_CMAKE_DIR}/UseVXL.cmake)
    SET(VXL_NUMERICS_LIBRARIES vnl vnl_algo netlib vcl)
  ELSE(VXL_FOUND)
    MESSAGE(FATAL_ERROR "Must set VXL_DIR for ITK_USE_SYSTEM_VXL.")
  ENDIF(VXL_FOUND)
ELSE(ITK_USE_SYSTEM_VXL)
  SET(VXL_NETLIB_INCLUDE_DIR ${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib)
  SET(VXL_NUMERICS_LIBRARIES itkvnl itkvnl_algo itkvcl)
ENDIF(ITK_USE_SYSTEM_VXL)


# Provide options to use system versions of third-party libraries.
ITK_THIRD_PARTY_OPTION(ZLIB zlib)
ITK_THIRD_PARTY_OPTION(PNG  png)
ITK_THIRD_PARTY_OPTION(TIFF tiff)

#-----------------------------------------------------------------------------
# Dispatch the build into the proper subdirectories.
SUBDIRS(Utilities Code)

#-----------------------------------------------------------------------------
# ITK requires special compiler flags on some platforms.
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -ftemplate-depth-50")
  IF(APPLE)
    SET(ITK_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS} -no-cpp-precomp")
    SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -no-cpp-precomp")
    SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -Wno-long-double")
  ENDIF(APPLE)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(UNIX)
  IF(NOT CMAKE_COMPILER_IS_GNUCXX)
    INCLUDE(${ITK_SOURCE_DIR}/CMake/TestNO_ICC_IDYNAMIC_NEEDED.cmake)
    TESTNO_ICC_IDYNAMIC_NEEDED(NO_ICC_IDYNAMIC_NEEDED ${ITK_SOURCE_DIR}/CMake )
    IF(NO_ICC_IDYNAMIC_NEEDED)
      SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS}")
    ELSE(NO_ICC_IDYNAMIC_NEEDED)
      SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -i_dynamic")
    ENDIF(NO_ICC_IDYNAMIC_NEEDED)
  ENDIF(NOT CMAKE_COMPILER_IS_GNUCXX)
ENDIF(UNIX)

IF(BORLAND)
    SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -w- -whid -waus -wpar")
    SET(ITK_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS} -w- -whid -waus -wpar")
ENDIF(BORLAND)

# Force the use of ansi cxx flags (i.e. -LANG:std on sgi )
IF("x${CMAKE_ANSI_CXXFLAGS}" MATCHES "^x.*[^ ]")
  SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} ${CMAKE_ANSI_CXXFLAGS}")
ENDIF("x${CMAKE_ANSI_CXXFLAGS}" MATCHES "^x.*[^ ]")
IF("x${CMAKE_ANSI_CFLAGS}" MATCHES "^x.*[^ ]")
  SET(ITK_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS} ${CMAKE_ANSI_CFLAGS}")
ENDIF("x${CMAKE_ANSI_CFLAGS}" MATCHES "^x.*[^ ]")

IF(CMAKE_SYSTEM MATCHES "IRIX.*")
  IF(NOT CMAKE_COMPILER_IS_GNUCXX)
    SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -woff 15,84,1110,1209,1424,1468,3102,3210,3439  -no_auto_include -ptused -no_prelink")
    SET(ITK_REQUIRED_LINK_FLAGS "${ITK_REQUIRED_LINK_FLAGS}  -Wl,-woff15")
  ENDIF(NOT CMAKE_COMPILER_IS_GNUCXX)
ENDIF(CMAKE_SYSTEM MATCHES "IRIX.*")

IF(MINGW)
  LINK_LIBRARIES(gdi32)
ENDIF(MINGW)

# for the gnu compiler a -D_PTHREADS is needed on sun
# for the native compiler a -mt flag is needed on the sun
IF(CMAKE_USE_PTHREADS)
  IF(CMAKE_SYSTEM MATCHES "SunOS.*")
    IF(CMAKE_COMPILER_IS_GNUCXX)
      SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -D_PTHREADS")
    ELSE(CMAKE_COMPILER_IS_GNUCXX)
      SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -mt")
      SET(ITK_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS} -mt")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  ENDIF(CMAKE_SYSTEM MATCHES "SunOS.*")
ENDIF(CMAKE_USE_PTHREADS)

IF(WIN32)
  SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -DNOMINMAX")
ENDIF(WIN32)

# mingw thread support
IF(MINGW)
  SET(ITK_REQUIRED_CXX_FLAGS "${ITK_REQUIRED_CXX_FLAGS} -mthreads")
  SET(ITK_REQUIRED_C_FLAGS "${ITK_REQUIRED_C_FLAGS} -mthreads")
  SET(ITK_REQUIRED_LINK_FLAGS "${ITK_REQUIRED_LINK_FLAGS} -mthreads")
ENDIF(MINGW)

#-----------------------------------------------------------------------------
# Find platform-specific differences in the handling of IEEE floating point 
# special values.


#
# See if compiler preprocessor has the __FUNCTION__ directive used by itkExceptionMacro
#
INCLUDE(${ITK_SOURCE_DIR}/CMake/CheckCPPDirective.cmake)
CHECK_CPP_DIRECTIVE_EXISTS(__FUNCTION__ ITK_CPP_FUNCTION)


#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
CONFIGURE_FILE(${ITK_SOURCE_DIR}/itkConfigure.h.in 
  ${ITK_BINARY_DIR}/itkConfigure.h)
IF(NOT ITK_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES ${ITK_BINARY_DIR}/itkConfigure.h
    DESTINATION ${ITK_INSTALL_INCLUDE_DIR_CM24}
    COMPONENT Development)
ENDIF(NOT ITK_INSTALL_NO_DEVELOPMENT)

CONFIGURE_FILE(${ITK_SOURCE_DIR}/Utilities/Doxygen/doxygen.config.in
               ${ITK_BINARY_DIR}/doxygen.config)

CONFIGURE_FILE(${ITK_SOURCE_DIR}/Utilities/Doxygen/itkdoxygen.pl.in
               ${ITK_BINARY_DIR}/itkdoxygen.pl)

CONFIGURE_FILE(${ITK_SOURCE_DIR}/Utilities/KWStyle/ITK.kws.xml.in
               ${ITK_BINARY_DIR}/ITK.kws.xml)

CONFIGURE_FILE(${ITK_SOURCE_DIR}/Utilities/KWStyle/ITKFiles.txt.in
               ${ITK_BINARY_DIR}/ITKFiles.txt)

#-----------------------------------------------------------------------------
# The entire ITK tree should use the same include path, except for the
# Wrapping directory.

# Create the list of include directories needed for ITK header files.
#THOMAS
INCLUDE(${ITK_SOURCE_DIR}/itkIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
INCLUDE_DIRECTORIES(
  ${ITK_INCLUDE_DIRS_BUILD_TREE}
  ${ITK_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${ITK_INCLUDE_DIRS_SYSTEM}
)

#-----------------------------------------------------------------------------
# ITK_USE_SYSTEM_GDCM is provided so that you can use an installed or external
# version of gdcm. If its on, you must specify the variables 
# Important: INCLUDE(${GDCM_USE_FILE}) *has* to be set after the
# INCLUDE_DIRECTORIES that sets all the include paths for ITK, otherwise
# if GDCM was build with VTK support, including it will bring the VTK
# include paths as well, and the wrong DICOMParser will be picked.

OPTION(ITK_USE_SYSTEM_GDCM "Use an outside build of GDCM." OFF)
MARK_AS_ADVANCED(ITK_USE_SYSTEM_GDCM)
IF(ITK_USE_SYSTEM_GDCM)
  FIND_PACKAGE(GDCM)
  IF(GDCM_FOUND)
    INCLUDE(${GDCM_USE_FILE})
    SET(ITK_GDCM_LIBRARIES gdcm)
  ELSE(GDCM_FOUND)
    MESSAGE(FATAL_ERROR "Must set GDCM_DIR for ITK_USE_SYSTEM_GDCM.")
  ENDIF(GDCM_FOUND)
ELSE(ITK_USE_SYSTEM_GDCM)
  SET(ITK_GDCM_LIBRARIES itkgdcm)
  SET(GDCM_INCLUDE_DIR
    ${ITK_SOURCE_DIR}/Utilities/gdcm/src
    )
ENDIF(ITK_USE_SYSTEM_GDCM)

#-----------------------------------------------------------------------------
# Help other projects use ITK.

# Copy the UseITK.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${ITK_SOURCE_DIR}/UseITK.cmake.in
               ${ITK_BINARY_DIR}/UseITK.cmake COPYONLY IMMEDIATE)

# Save the compiler settings so another project can import them.
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${ITK_BINARY_DIR}/ITKBuildSettings.cmake)

# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${ITK_BINARY_DIR}/ITKLibraryDepends.cmake)

# Create the ITKConfig.cmake file containing the ITK configuration.
INCLUDE (${ITK_SOURCE_DIR}/itkGenerateITKConfig.cmake)

IF(NOT ITK_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES
    ${ITK_BINARY_DIR}/ITKBuildSettings.cmake
    ${ITK_BINARY_DIR}/ITKLibraryDepends.cmake
    ${ITK_BINARY_DIR}/UseITK.cmake
    DESTINATION ${ITK_INSTALL_PACKAGE_DIR_CM24}
    COMPONENT Development
    )
ENDIF(NOT ITK_INSTALL_NO_DEVELOPMENT)

