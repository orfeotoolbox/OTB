# This is the root OTB CMakeLists file.
# $Id$
#

PROJECT(OTB)

INCLUDE_REGULAR_EXPRESSION("^(otb|itk|vtk|vnl|vcl|vxl|f2c|netlib|ce|itpack|DICOM|meta|png|dbh|tif|jpeg|zlib).*$")
SOURCE_GROUP("XML Files" REGULAR_EXPRESSION "[.]xml$")

# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
IF(WIN32)
  IF(NOT BORLAND)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
        IF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
          ADD_DEFINITIONS(
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
        ENDIF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
  ENDIF(NOT BORLAND)
ENDIF(WIN32)

# Don't build anything unless the version of CMake is high enough.
# The matching ELSE/ENDIF should be the last lines in the file.
IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.7)

# If CMake is the version we're expecting, don't show the
# CMAKE_BACKWARDS_COMPATIBILITY option.
IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^1\\.8$")
  MARK_AS_ADVANCED(FORCE CMAKE_BACKWARDS_COMPATIBILITY)
ELSE("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^1\\.8$")
  MARK_AS_ADVANCED(CLEAR CMAKE_BACKWARDS_COMPATIBILITY)
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" MATCHES "^1\\.8$")


#-----------------------------------------------------------------------------
# OTB version number.  An even minor number corresponds to releases.
SET(OTB_VERSION_MAJOR "1")
SET(OTB_VERSION_MINOR "0")
SET(OTB_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(OTB_VERSION_STRING "${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")

#-----------------------------------------------------------------------------
# RECHERCHE DES LIBRAIRIES EXTERNES UTILISEES

#-------------------------------
# Librairie VTK
#-------------------------------
#       FIND_PACKAGE(VTK)
#       IF(VTK_FOUND)
#         INCLUDE(${VTK_USE_FILE})
#       ELSE(VTK_FOUND)
#         MESSAGE(FATAL_ERROR
#                 "Cannot build OTB project without VTK.  Please set VTK_DIR.")
#       ENDIF(VTK_FOUND)

#-------------------------------
# Librairie ITK
#-------------------------------
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build OTB project without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

#-------------------------------
# Librairie OpenGL
#-------------------------------
FIND_PACKAGE(OpenGL)
IF(OPENGL_INCLUDE_PATH)
  INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_PATH})
ENDIF(OPENGL_INCLUDE_PATH)

#-------------------------------
# Librairie CAI
#-------------------------------
#       FIND_LIBRARY(CAI_LIBRARY NAMES cai PATHS $ENV{CAI_DIR_LIB} )
#       FIND_LIBRARY(TIFF_LIBRARY NAMES tiff PATHS $ENV{CAI_DIR_LIB} )
#       FIND_PATH(CAI_INCLUDES cai_image.h $ENV{CAI_DIR_INCLUDES} )
#       IF (NOT CAI_LIBRARY)
#         MESSAGE(FATAL_ERROR
#                 "Cannot find CAI library. Please set CAI_DIR_LIB.")
#       ENDIF (NOT CAI_LIBRARY)

#INCLUDE_DIRECTORIES(${CAI_INCLUDES})
#LINK_DIRECTORIES( $ENV{CAI_DIR_LIB} )

#       SET( $ENV{CAI_DIR} ${OTB_SOURCE_DIR}/Utilities/CAI)


#       FIND_PACKAGE(CAI)
#       IF(CAI_FOUND)
#         INCLUDE(${CAI_USE_FILE})
#       ELSE(CAI_FOUND)
#         MESSAGE(FATAL_ERROR
#                 "Cannot build OTB project without CAI.  Please set CAI_DIR.")
#       ENDIF(CAI_FOUND)


#-------------------------------
# Librairie GDAL
#-------------------------------
FIND_LIBRARY(GDAL_LIBRARY NAMES gdal PATHS $ENV{GDAL_DIR_LIB} )
FIND_PATH(GDAL_INCLUDES gdal.h $ENV{GDAL_DIR_INCLUDES} )
IF (NOT GDAL_LIBRARY)
  MESSAGE(FATAL_ERROR
          "Cannot find GDAL library. Please set GDAL_DIR_LIB.")
ENDIF (NOT GDAL_LIBRARY)

INCLUDE_DIRECTORIES(${GDAL_INCLUDES})
LINK_DIRECTORIES( $ENV{GDAL_DIR_LIB} )


#-------------------------------
# Librairie FLTK
#-------------------------------
# Add an option to use or not use FLTK (http://www.fltk.org)
FIND_PACKAGE(FLTK)
IF(FLTK_FOUND)
#  INCLUDE(${FLTK_USE_FILE})
  INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})
ELSE(FLTK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build OTB project without FLTK.  Please set FLTK_DIR.")
ENDIF(FLTK_FOUND)

# The fluid-generated fltk sources have many warnings.  This macro
# will disable warnings for the generated files on some compilers.
MACRO(OTB_DISABLE_FLTK_GENERATED_WARNINGS files)
  IF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 1.6)
    IF(CMAKE_COMPILER_IS_GNUCXX)
      FOREACH(f ${files})
        STRING(REGEX REPLACE "\\.fl$" ".cxx" SRC "${f}")
        STRING(REGEX REPLACE ".*/([^/]*)$" "\\1" SRC "${SRC}")
        SET_SOURCE_FILES_PROPERTIES(${SRC} PROPERTIES COMPILE_FLAGS -w)
      ENDFOREACH(f)
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  ENDIF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 1.6)
ENDMACRO(OTB_DISABLE_FLTK_GENERATED_WARNINGS)

# Macro for adding resources to GUI application on the Mac
#
IF(APPLE AND NOT FLTK_USE_X)
    FIND_PROGRAM(ITK_APPLE_RESOURCE Rez /Developer/Tools)
    FIND_FILE(ITK_FLTK_RESOURCE mac.r /usr/local/include/FL)
    IF(NOT ITK_FLTK_RESOURCE)
      MESSAGE("Fltk resources not found, GUI application will not respond to mouse events")
    ENDIF(NOT ITK_FLTK_RESOURCE)

    MACRO(ADD_GUI_EXECUTABLE name sources)
      ADD_EXECUTABLE(${name} ${sources})
      INSTALL_TARGETS(/bin ${name})
      SET(EXEC_PATH ${EXECUTABLE_OUTPUT_PATH})
      IF(NOT EXEC_PATH)
        SET(EXEC_PATH ${CMAKE_CURRENT_BINARY_DIR})
      ENDIF(NOT EXEC_PATH)
        IF(ITK_APPLE_RESOURCE)
          ADD_CUSTOM_COMMAND(SOURCE ${name}
                             COMMAND ${ITK_APPLE_RESOURCE}
                             ARGS -t APPL ${ITK_FLTK_RESOURCE} -o
                             ${EXEC_PATH}/${name}
                             TARGET ${name})
        ENDIF(ITK_APPLE_RESOURCE)
    ENDMACRO(ADD_GUI_EXECUTABLE)
ELSE(APPLE AND NOT FLTK_USE_X)
  MACRO(ADD_GUI_EXECUTABLE name sources)
    ADD_EXECUTABLE(${name} ${sources})
    INSTALL_TARGETS(/bin ${name})
  ENDMACRO(ADD_GUI_EXECUTABLE)
ENDIF(APPLE AND NOT FLTK_USE_X)


#FIND_LIBRARY(FLTK_LIBRARY NAMES fltk PATHS /ORFEO/outils/libs-externes-install/lib/fltk )
#FIND_PATH(FLTK_INCLUDES Fl.H /ORFEO/outils/libs-externes-install/include/FL )
#FIND_LIBRARY(CAI_LIBRARY NAMES cai PATHS ${CAI_DIR_LIB} )
#FIND_PATH(CAI_INCLUDES cai_image.h ${CAI_DIR_INCLUDES} )
#IF (NOT FLTK_LIBRARY)
#  MESSAGE(FATAL_ERROR
#          "Cannot find FLTK library. Please set FLTK_DIR_LIB.")
#ENDIF (NOT FLTK_LIBRARY)

#INCLUDE_DIRECTORIES(${FLTK_INCLUDES})
#LINK_DIRECTORIES( $ENV{FLTK_DIR_LIB} )


INCLUDE_DIRECTORIES (
  ${OTB_BINARY_DIR} 
  ${OTB_SOURCE_DIR}
)

INCLUDE_DIRECTORIES( ${OTB_INCLUDE_DIRS} )
#INCLUDE_DIRECTORIES( ${OTB_SOURCE_DIR}/library )
#INCLUDE_DIRECTORIES( ${OTB_SOURCE_DIR}/tools )
#LINK_DIRECTORIES( ${OTB_LIBRARY_DIRS} )

#Includes pour les Utilities (/Utilities)
INCLUDE_DIRECTORIES (
#  ${OTB_SOURCE_DIR}/Utils/FltkImageViewer
)


#-----------------------------------------------------------------------------
# Test for some required system information.
INCLUDE (${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityC.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/CMakeBackwardCompatibilityCXX.cmake)


#-----------------------------------------------------------------------------
# Repertoire des images de télédetection.
SET( OTB_DATA_DIR "$ENV{OTB_DATA_DIR}" CACHE STRING "Image directory")

#-----------------------------------------------------------------------------
# Configure Dart testing support.  This should be done before any
# MESSAGE(FATAL_ERROR ...) commands are invoked.
INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
IF(BUILD_TESTING)
  ENABLE_TESTING()
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)



#-----------------------------------------------------------------------------
# Output directories.
SET (LIBRARY_OUTPUT_PATH ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Les librairies sont generees dans le mï¿½e repertoire.")
SET (EXECUTABLE_OUTPUT_PATH ${OTB_BINARY_DIR}/bin CACHE INTERNAL "Les executables sont generes dans le mï¿½e repertoire.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
SET(OTB_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(OTB_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

INCLUDE_DIRECTORIES( ${OTB_LIBRARY_PATH} )

# Put "." in the rpath of all OTB shared libraries.
LINK_DIRECTORIES(.)

#-----------------------------------------------------------------------------
# OTB build configuration options.
OPTION(BUILD_SHARED_LIBS "Generer les libraires de l'OTB en dynamique." OFF)
SET(OTB_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

# Options for building some of the most common core templates explicitly only
# once.  This has only been tested on gcc compilers.
OPTION(OTB_EXPLICIT_INSTANTIATION "BETA-Try to explicitly build some of the core templates explictitly rather than over and over again." OFF)
MARK_AS_ADVANCED(OTB_EXPLICIT_INSTANTIATION)
IF( OTB_EXPLICIT_INSTANTIATION )
 ADD_DEFINITIONS(-DOTB_EXPLICIT_INSTANTIATION)
ENDIF( OTB_EXPLICIT_INSTANTIATION )


IF(WIN32)
   IF(BUILD_SHARED_LIBS)
     SET(OTB_COMMON_BUILD_TYPE "SHARED")
   ELSE(BUILD_SHARED_LIBS)
     SET(OTB_COMMON_BUILD_TYPE "STATIC")
   ENDIF(BUILD_SHARED_LIBS)
   SET(BUILD_SHARED_LIBS OFF)
ENDIF(WIN32)

OPTION(BUILD_EXAMPLES "Generer les exemples." ON)
OPTION(BUILD_TESTING "Generer les tests." ON)


#-----------------------------------------------------------------------------
# Dispatch the build into the proper subdirectories.
SUBDIRS(Utilities Code)

IF (BUILD_EXAMPLES)
  SUBDIRS(Examples)
ENDIF (BUILD_EXAMPLES)

IF (BUILD_TESTING)
  SUBDIRS(Testing)
ENDIF (BUILD_TESTING)

#-----------------------------------------------------------------------------
# OTB requires special compiler flags on some platforms.
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -ftemplate-depth-50")
  IF(APPLE)
    SET(OTB_REQUIRED_C_FLAGS "${OTB_REQUIRED_C_FLAGS} -no-cpp-precomp")
    SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -no-cpp-precomp")
    SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -Wno-long-double")
  ENDIF(APPLE)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(UNIX)
  IF(NOT CMAKE_COMPILER_IS_GNUCXX)
    INCLUDE(${OTB_SOURCE_DIR}/CMake/TestNO_ICC_IDYNAMIC_NEEDED.cmake)
    TESTNO_ICC_IDYNAMIC_NEEDED(NO_ICC_IDYNAMIC_NEEDED ${OTB_SOURCE_DIR}/CMake )
    IF(NO_ICC_IDYNAMIC_NEEDED)
      SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS}")
    ELSE(NO_ICC_IDYNAMIC_NEEDED)
      SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -i_dynamic")
    ENDIF(NO_ICC_IDYNAMIC_NEEDED)
  ENDIF(NOT CMAKE_COMPILER_IS_GNUCXX)
ENDIF(UNIX)

# Force the use of ansi cxx flags (i.e. -LANG:std on sgi )
IF("x${CMAKE_ANSI_CXXFLAGS}" MATCHES "^x.*[^ ]")
  SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} ${CMAKE_ANSI_CXXFLAGS}")
ENDIF("x${CMAKE_ANSI_CXXFLAGS}" MATCHES "^x.*[^ ]")
IF("x${CMAKE_ANSI_CFLAGS}" MATCHES "^x.*[^ ]")
  SET(OTB_REQUIRED_C_FLAGS "${OTB_REQUIRED_C_FLAGS} ${CMAKE_ANSI_CFLAGS}")
ENDIF("x${CMAKE_ANSI_CFLAGS}" MATCHES "^x.*[^ ]")

IF(CMAKE_SYSTEM MATCHES "IRIX.*")
  IF(NOT CMAKE_COMPILER_IS_GNUCXX)
    SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -woff 15,84,1110,1209,1424,1468,3102,3210,3439  -no_auto_include -ptused -no_prelink")
    SET(OTB_REQUIRED_LINK_FLAGS "${OTB_REQUIRED_LINK_FLAGS}  -Wl,-woff15")
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
      SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -D_PTHREADS")
    ELSE(CMAKE_COMPILER_IS_GNUCXX)
      SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -mt")
      SET(OTB_REQUIRED_C_FLAGS "${OTB_REQUIRED_C_FLAGS} -mt")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  ENDIF(CMAKE_SYSTEM MATCHES "SunOS.*")
ENDIF(CMAKE_USE_PTHREADS)

IF(WIN32)
  SET(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -DNOMINMAX")
ENDIF(WIN32)

#-----------------------------------------------------------------------------
# Find platform-specific differences in the handling of IEEE floating point 
# special values.
INCLUDE(${OTB_SOURCE_DIR}/CMake/CheckBigBitfield.cmake)
CHECK_BIG_BITFIELD(BIGBITFIELD_VALUE ${OTB_SOURCE_DIR}/CMake)
IF(BIGBITFIELD_VALUE)
   SET(BIGBITFIELD 1 CACHE INTERNAL "System handles bit-fields larger than 32 bits.")
ELSE(BIGBITFIELD_VALUE)
   SET(BIGBITFIELD 0 CACHE INTERNAL "System handles bit-fields larger than 32 bits.")
ENDIF(BIGBITFIELD_VALUE)

INCLUDE(${OTB_SOURCE_DIR}/CMake/TestQnanhibit.cmake)
TEST_QNANHIBIT(QNANHIBIT_VALUE ${OTB_SOURCE_DIR}/CMake)
IF(QNANHIBIT_VALUE)
   SET(QNANHIBIT 1 CACHE INTERNAL "The 22nd bit of 32-bit floating-point quiet NaN.")
ELSE(QNANHIBIT_VALUE)
   SET(QNANHIBIT 0 CACHE INTERNAL "The 22nd bit of 32-bit floating-point quiet NaN.")
ENDIF(QNANHIBIT_VALUE)

IF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.0)
ELSE("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.0)
INCLUDE(CheckTypeSize)
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.0)

CHECK_TYPE_SIZE("long long" OTB_SIZEOF_LONG_LONG)
CHECK_TYPE_SIZE("__int64"   OTB_SIZEOF___INT64)

IF(OTB_SIZEOF___INT64)
  IF("OTB_TYPE_SAME_LONG_AND___INT64" MATCHES "^OTB_TYPE_SAME_LONG_AND___INT64$")
    MESSAGE(STATUS "Checking whether long and __int64 are the same type")
    TRY_COMPILE(OTB_TYPE_SAME_LONG_AND___INT64
      ${OTB_BINARY_DIR}/CMakeTmp
      ${OTB_SOURCE_DIR}/CMake/otbTestCompareTypes.cxx
      COMPILE_DEFINITIONS
      -DOTB_TEST_COMPARE_TYPE_1=long
      -DOTB_TEST_COMPARE_TYPE_2=__int64
      OUTPUT_VARIABLE OUTPUT)
    IF(OTB_TYPE_SAME_LONG_AND___INT64)
      MESSAGE(STATUS "Checking whether long and __int64 are the same type -- yes")
      SET(OTB_TYPE_SAME_LONG_AND___INT64 1 CACHE INTERNAL "Whether long and __int64 are the same type")
    ELSE(OTB_TYPE_SAME_LONG_AND___INT64)
      MESSAGE(STATUS "Checking whether long and __int64 are the same type -- no")
      SET(OTB_TYPE_SAME_LONG_AND___INT64 0 CACHE INTERNAL "Whether long and __int64 are the same type")
      WRITE_FILE(${CMAKE_BINARY_DIR}/CMakeError.log
        "Determining whether long and __int64 are the same type "
        "failed with the following output:\n"
        "${OUTPUT}\n" APPEND)
    ENDIF(OTB_TYPE_SAME_LONG_AND___INT64)
  ENDIF("OTB_TYPE_SAME_LONG_AND___INT64" MATCHES "^OTB_TYPE_SAME_LONG_AND___INT64$")
ENDIF(OTB_SIZEOF___INT64)

IF(OTB_SIZEOF_LONG_LONG)
  CONFIGURE_FILE(${OTB_SOURCE_DIR}/CMake/otbTestStreamLongLong.cxx.in
    ${OTB_BINARY_DIR}/otbTestStreamLongLong.cxx @ONLY IMMEDIATE)
  IF("OTB_OSTREAM_SUPPORTS_LONG_LONG" MATCHES "^OTB_OSTREAM_SUPPORTS_LONG_LONG$")
    MESSAGE(STATUS "Checking if ostream supports long long")
    TRY_COMPILE(OTB_OSTREAM_SUPPORTS_LONG_LONG
      ${OTB_BINARY_DIR}
      ${OTB_BINARY_DIR}/otbTestStreamLongLong.cxx
      COMPILE_DEFINITIONS -DOTB_TEST_OSTREAM_LONG_LONG
      OUTPUT_VARIABLE OUTPUT)
    IF(OTB_OSTREAM_SUPPORTS_LONG_LONG)
      MESSAGE(STATUS "Checking if ostream supports long long -- yes")
      SET(OTB_OSTREAM_SUPPORTS_LONG_LONG 1 CACHE INTERNAL "Whether ostream supports long long")
    ELSE(OTB_OSTREAM_SUPPORTS_LONG_LONG)
      MESSAGE(STATUS "Checking if ostream supports long long -- no")
      SET(OTB_OSTREAM_SUPPORTS_LONG_LONG 0 CACHE INTERNAL "Whether ostream supports long long")
      WRITE_FILE(${CMAKE_BINARY_DIR}/CMakeError.log
        "Determining if ostream supports long long "
        "failed with the following output:\n"
        "${OUTPUT}\n" APPEND)
    ENDIF(OTB_OSTREAM_SUPPORTS_LONG_LONG)
  ENDIF("OTB_OSTREAM_SUPPORTS_LONG_LONG" MATCHES "^OTB_OSTREAM_SUPPORTS_LONG_LONG$")
  IF("OTB_ISTREAM_SUPPORTS_LONG_LONG" MATCHES "^OTB_ISTREAM_SUPPORTS_LONG_LONG$")
    MESSAGE(STATUS "Checking if istream supports long long")
    TRY_COMPILE(OTB_ISTREAM_SUPPORTS_LONG_LONG
      ${OTB_BINARY_DIR}
      ${OTB_BINARY_DIR}/otbTestStreamLongLong.cxx
      COMPILE_DEFINITIONS -DOTB_TEST_ISTREAM_LONG_LONG
      OUTPUT_VARIABLE OUTPUT)
    IF(OTB_ISTREAM_SUPPORTS_LONG_LONG)
      MESSAGE(STATUS "Checking if istream supports long long -- yes")
      SET(OTB_ISTREAM_SUPPORTS_LONG_LONG 1 CACHE INTERNAL "Whether istream supports long long")
    ELSE(OTB_ISTREAM_SUPPORTS_LONG_LONG)
      MESSAGE(STATUS "Checking if istream supports long long -- no")
      SET(OTB_ISTREAM_SUPPORTS_LONG_LONG 0 CACHE INTERNAL "Whether istream supports long long")
      WRITE_FILE(${CMAKE_BINARY_DIR}/CMakeError.log
        "Determining if istream supports long long "
        "failed with the following output:\n"
        "${OUTPUT}\n" APPEND)
    ENDIF(OTB_ISTREAM_SUPPORTS_LONG_LONG)
  ENDIF("OTB_ISTREAM_SUPPORTS_LONG_LONG" MATCHES "^OTB_ISTREAM_SUPPORTS_LONG_LONG$")
ENDIF(OTB_SIZEOF_LONG_LONG)


#-----------------------------------------------------------------------------
# Configure the default OTB_DATA_ROOT for the location of OTB Data.
FIND_PATH(OTB_DATA_ROOT OTBData.readme ${OTB_SOURCE_DIR}/Testing/Data $ENV{OTB_DATA_ROOT})
MARK_AS_ADVANCED(OTB_DATA_ROOT)


#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/otbConfigure.h.in 
               ${OTB_BINARY_DIR}/otbConfigure.h)

#-----------------------------------------------------------------------------
# The entire OTB tree should use the same include path, except for the
# Wrapping directory.

# Create the list of include directories needed for OTB header files.
INCLUDE(${OTB_SOURCE_DIR}/otbIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
INCLUDE_DIRECTORIES(
  ${OTB_INCLUDE_DIRS_BUILD_TREE}
  ${OTB_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${OTB_INCLUDE_DIRS_SYSTEM}
)

#-----------------------------------------------------------------------------
# Uninstall cmake use to uninstall OTB.
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/CMake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#-----------------------------------------------------------------------------
# Help other projects use OTB.

# Copy the UseOTB.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${OTB_SOURCE_DIR}/UseOTB.cmake.in
               ${OTB_BINARY_DIR}/UseOTB.cmake COPYONLY IMMEDIATE)

# Save the compiler settings so another project can import them.
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${OTB_BINARY_DIR}/OTBBuildSettings.cmake)
INSTALL_FILES(/lib/otb .cmake OTBBuildSettings)

# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${OTB_BINARY_DIR}/OTBLibraryDepends.cmake)

# Create the OTBConfig.cmake file containing the OTB configuration.
INCLUDE (${OTB_SOURCE_DIR}/otbGenerateOTBConfig.cmake)


# Install some files.
INSTALL_FILES(/include/otb .h otbConfigure)
INSTALL_FILES(/lib/otb .cmake UseOTB OTBLibraryDepends OTBConfig )

#-----------------------------------------------------------------------------
# Add compiler flags OTB needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OTB_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OTB_REQUIRED_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OTB_REQUIRED_LINK_FLAGS}")

#-----------------------------------------------------------------------------
# The frename-registers option does not work due to a bug in the gnu compiler.
# It must be removed or data errors will be produced and incorrect results
# will be produced.  This is first documented in the gcc4 man page.
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(ALL_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}" )
  SEPARATE_ARGUMENTS(ALL_FLAGS)
  FOREACH(COMP_OPTION ${ALL_FLAGS})
    #  MESSAGE("${COMP_OPTION} being checked.")
    IF("${COMP_OPTION}" STREQUAL "-frename-registers")
      MESSAGE(FATAL_ERROR "-frename-registers causes runtime bugs.  It must be removed from your compilation options.")
    ENDIF("${COMP_OPTION}" STREQUAL "-frename-registers")
    IF("${COMP_OPTION}" STREQUAL "-ffloat-store")
      MESSAGE(FATAL_ERROR "-ffloat-store causes runtime bugs on gcc 3.2.3 (appearently not on gcc 3.4.3, but the exact criteria is not known).  It must be removed from your compilation options.")
    ENDIF("${COMP_OPTION}" STREQUAL "-ffloat-store")
  ENDFOREACH(COMP_OPTION)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# If the version of CMake was too old, complain and build nothing.
# These should be the last lines in this file.
ELSE("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.7)
  MESSAGE(SEND_ERROR
          "This version of CMake is too old to build this version of OTB.  "
          "Please upgrade to CMake 1.8.")
ENDIF("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 1.7)

