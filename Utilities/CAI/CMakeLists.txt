# This is the root CAI CMakeLists file.
# $Id$
#

PROJECT(CAI)

INCLUDE_REGULAR_EXPRESSION("^(cai).*$")
SOURCE_GROUP("XML Files" REGULAR_EXPRESSION "[.]xml$")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(CAI_VERSION_STRING "${CAI_VERSION_MAJOR}.${CAI_VERSION_MINOR}")

#-----------------------------------------------------------------------------
# RECHERCHE DES LIBRAIRIES EXTERNES UTILISEES


INCLUDE_DIRECTORIES(${CAI_INCLUDES})
LINK_DIRECTORIES( $ENV{CAI_DIR_LIB} )


INCLUDE_DIRECTORIES (
  ${CAI_BINARY_DIR} 
  ${CAI_SOURCE_DIR}
)

INCLUDE_DIRECTORIES( ${CAI_INCLUDE_DIRS} )


#-----------------------------------------------------------------------------
# Output directories.
SET (LIBRARY_OUTPUT_PATH ${CAI_BINARY_DIR}/bin CACHE INTERNAL "Les librairies sont generees dans ce repertoire.")
SET (EXECUTABLE_OUTPUT_PATH ${CAI_BINARY_DIR}/bin CACHE INTERNAL "Les executables sont generes dans ce repertoire.")
MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH EXECUTABLE_OUTPUT_PATH)
SET(CAI_LIBRARY_PATH "${LIBRARY_OUTPUT_PATH}")
SET(CAI_EXECUTABLE_PATH "${EXECUTABLE_OUTPUT_PATH}")
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

INCLUDE_DIRECTORIES( ${CAI_LIBRARY_PATH} )

# Put "." in the rpath of all CAI shared libraries.
LINK_DIRECTORIES(.)

#-----------------------------------------------------------------------------
# CAI build configuration options.
OPTION(BUILD_SHARED_LIBS "Generer les libraires de CAI en dynamique." OFF)
SET(CAI_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

IF(WIN32)
   IF(BUILD_SHARED_LIBS)
     SET(CAI_COMMON_BUILD_TYPE "SHARED")
   ELSE(BUILD_SHARED_LIBS)
     SET(CAI_COMMON_BUILD_TYPE "STATIC")
   ENDIF(BUILD_SHARED_LIBS)
   SET(BUILD_SHARED_LIBS OFF)
ENDIF(WIN32)


IF(WIN32)
   SUBDIRS(cai_dll)
ELSE(WIN32)
   ADD_DEFINITIONS(-D_NOHDF16)
   SUBDIRS(src inc)
ENDIF(WIN32)


#-----------------------------------------------------------------------------
# CAI requires special compiler flags on some platforms.
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} -ftemplate-depth-50")
  IF(APPLE)
    SET(CAI_REQUIRED_C_FLAGS "${CAI_REQUIRED_C_FLAGS} -no-cpp-precomp")
    SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} -no-cpp-precomp")
    SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} -Wno-long-double")
  ENDIF(APPLE)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

IF(UNIX)
  IF(NOT CMAKE_COMPILER_IS_GNUCXX)
    INCLUDE(${CAI_SOURCE_DIR}/CMake/TestNO_ICC_IDYNAMIC_NEEDED.cmake)
    TESTNO_ICC_IDYNAMIC_NEEDED(NO_ICC_IDYNAMIC_NEEDED ${CAI_SOURCE_DIR}/CMake )
    IF(NO_ICC_IDYNAMIC_NEEDED)
      SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS}")
    ELSE(NO_ICC_IDYNAMIC_NEEDED)
      SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} -i_dynamic")
    ENDIF(NO_ICC_IDYNAMIC_NEEDED)
  ENDIF(NOT CMAKE_COMPILER_IS_GNUCXX)
ENDIF(UNIX)

# Force the use of ansi cxx flags (i.e. -LANG:std on sgi )
IF("x${CMAKE_ANSI_CXXFLAGS}" MATCHES "^x.*[^ ]")
  SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} ${CMAKE_ANSI_CXXFLAGS}")
ENDIF("x${CMAKE_ANSI_CXXFLAGS}" MATCHES "^x.*[^ ]")
IF("x${CMAKE_ANSI_CFLAGS}" MATCHES "^x.*[^ ]")
  SET(CAI_REQUIRED_C_FLAGS "${CAI_REQUIRED_C_FLAGS} ${CMAKE_ANSI_CFLAGS}")
ENDIF("x${CMAKE_ANSI_CFLAGS}" MATCHES "^x.*[^ ]")

IF(CMAKE_SYSTEM MATCHES "IRIX.*")
  IF(NOT CMAKE_COMPILER_IS_GNUCXX)
    SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} -woff 15,84,1110,1209,1424,1468,3102,3210,3439  -no_auto_include -ptused -no_prelink")
    SET(CAI_REQUIRED_LINK_FLAGS "${CAI_REQUIRED_LINK_FLAGS}  -Wl,-woff15")
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
      SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} -D_PTHREADS")
    ELSE(CMAKE_COMPILER_IS_GNUCXX)
      SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} -mt")
      SET(CAI_REQUIRED_C_FLAGS "${CAI_REQUIRED_C_FLAGS} -mt")
    ENDIF(CMAKE_COMPILER_IS_GNUCXX)
  ENDIF(CMAKE_SYSTEM MATCHES "SunOS.*")
ENDIF(CMAKE_USE_PTHREADS)

IF(WIN32)
  SET(CAI_REQUIRED_CXX_FLAGS "${CAI_REQUIRED_CXX_FLAGS} -DNOMINMAX")
ENDIF(WIN32)


#-----------------------------------------------------------------------------
# The entire CAI tree should use the same include path, except for the
# Wrapping directory.

# Create the list of include directories needed for CAI header files.
INCLUDE(${CAI_SOURCE_DIR}/caiIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
INCLUDE_DIRECTORIES(
  ${CAI_INCLUDE_DIRS_BUILD_TREE}
  ${CAI_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${CAI_INCLUDE_DIRS_SYSTEM}
)

#-----------------------------------------------------------------------------
# Help other projects use CAI.

# Copy the UseCAI.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${CAI_SOURCE_DIR}/UseCAI.cmake.in
               ${CAI_BINARY_DIR}/UseCAI.cmake COPYONLY IMMEDIATE)

# Save the compiler settings so another project can import them.
INCLUDE(${CMAKE_ROOT}/Modules/CMakeExportBuildSettings.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${CAI_BINARY_DIR}/CAIBuildSettings.cmake)
INSTALL_FILES(/lib/cai .cmake CAIBuildSettings)

# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${CAI_BINARY_DIR}/CAILibraryDepends.cmake)

# Create the CAIConfig.cmake file containing the CAI configuration.
INCLUDE (${CAI_SOURCE_DIR}/caiGenerateCAIConfig.cmake)

# Install some files.
INSTALL_FILES(/include/cai .h )
INSTALL_FILES(/lib/cai .cmake UseCAI CAILibraryDepends CAIConfig )

#-----------------------------------------------------------------------------
# Add compiler flags CAI needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CAI_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CAI_REQUIRED_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CAI_REQUIRED_LINK_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CAI_REQUIRED_LINK_FLAGS}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${CAI_REQUIRED_LINK_FLAGS}")

