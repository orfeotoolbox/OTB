###############################################################################
# CMake macro to find GDAL library.
#
# On success, the macro sets the following variables:
# GDAL_FOUND       = if the library found
# GDAL_LIBRARY     = full path to the library
# GDAL_INCLUDE_DIR = where to find the library headers
#
# On Unix, macro sets also:
# GDAL_VERSION_STRING = human-readable string containing version of the library
#
# Author of original: Magnus Homann (Quantum GIS)
# Modifications: Mateusz Loskot <mateusz@loskot.net>
#
###############################################################################
message(STATUS "Searching for GDAL ${GDAL_FIND_VERSION}+ library")
message(STATUS "   NOTE: Required version is not checked - to be implemented")

set(GDAL_NAMES gdal)

if(WIN32)

    set(OSGEO4W_IMPORT_LIBRARY gdal_i)
    if(DEFINED ENV{OSGEO4W_ROOT})
        set(OSGEO4W_ROOT_DIR $ENV{OSGEO4W_ROOT})
        message(STATUS "Trying OSGeo4W using environment variable OSGEO4W_ROOT=$ENV{OSGEO4W_ROOT}")
    else()
        set(OSGEO4W_ROOT_DIR c:/OSGeo4W)
        message(STATUS "Trying OSGeo4W using default location OSGEO4W_ROOT=${OSGEO4W_ROOT_DIR}")
    endif()

    if(MINGW)
        find_path(GDAL_INCLUDE_DIR
            gdal.h
            PATH_PREFIXES gdal gdal-1.6
            PATHS
            /usr/local/include
            /usr/include
            c:/msys/local/include
            ${OSGEO4W_ROOT_DIR}/include)

        find_library(GDAL_LIBRARY
            NAMES ${GDAL_NAMES}
            PATH_PREFIXES gdal gdal-1.6
            PATHS
            /usr/local/lib
            /usr/lib
            c:/msys/local/lib
            ${OSGEO4W_ROOT_DIR}/lib)
    endif(MINGW)

    if(MSVC)

        find_path(GDAL_INCLUDE_DIR
            NAMES gdal.h
            PATH_PREFIXES gdal gdal-1.6
            PATHS
            "${OSGEO4W_ROOT_DIR}/apps/gdal-dev/include"
            "$ENV{LIB_DIR}/include/gdal"
            ${OSGEO4W_ROOT_DIR}/include)

        set(GDAL_NAMES ${OSGEO4W_IMPORT_LIBRARY} ${GDAL_NAMES})
        find_library(GDAL_LIBRARY
            NAMES ${GDAL_NAMES}
            PATH_PREFIXES gdal gdal-1.6
            PATHS
            "$ENV{LIB_DIR}/lib"
            /usr/lib
            c:/msys/local/lib
            "${OSGEO4W_ROOT_DIR}/apps/gdal-dev/lib"
            ${OSGEO4W_ROOT_DIR}/lib)

        if(GDAL_LIBRARY)
            set(GDAL_LIBRARY;odbc32;odbccp32 CACHE STRING INTERNAL)
        endif()
    endif(MSVC)

elseif(UNIX)

    # Try to use framework on Mac OS X
    if(APPLE)
        set(GDAL_MAC_PATH /Library/Frameworks/GDAL.framework/unix/bin)
    endif()

    # Try to use GDAL_HOME location if specified
    if($ENV{GDAL_HOME})
        set(GDAL_CONFIG_PREFER_PATH
            "$ENV{GDAL_HOME}/bin" CACHE STRING "Search for gdal-config program in preferred location")
    endif()

    # Try to use OSGeo4W installation
    if($ENV{OSGEO4W_HOME})
        set(GDAL_CONFIG_PREFER_OSGEO4W_PATH
            "$ENV{OSGEO4W_HOME}/bin" CACHE STRING "Search for gdal-config program provided by OSGeo4W")
    endif()

    # Try to use FWTools installation
    if($ENV{FWTOOLS_HOME})
        set(GDAL_CONFIG_PREFER_FWTOOLS_PATH
            "$ENV{FWTOOLS_HOME}/bin_safe" CACHE STRING "Search for gdal-config program provided by FWTools")
    endif()

    find_program(GDAL_CONFIG gdal-config
        ${GDAL_CONFIG_PREFER_PATH}
        ${GDAL_CONFIG_PREFER_OSGEO4W_PATH}
        ${GDAL_CONFIG_PREFER_FWTOOLS_PATH}
        ${GDAL_MAC_PATH}
        /usr/local/bin/
        /usr/bin/)

    if(GDAL_CONFIG)

        # TODO: Replace the regex hacks with CMake version comparison feature:
        # if(version1 VERSION_LESS version2)

        # Extract GDAL version
        exec_program(${GDAL_CONFIG} ARGS --version OUTPUT_VARIABLE GDAL_VERSION)
        set(GDAL_VERSION_STRING "${GDAL_VERSION}" CACHE STRING "Version of GDAL package found")

        string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\1" GDAL_VERSION_MAJOR "${GDAL_VERSION}")
        string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)" "\\2" GDAL_VERSION_MINOR "${GDAL_VERSION}")

        # Check for GDAL version
        # TODO: What version is requiredfor libLAS? --mloskot
        if(GDAL_VERSION_MAJOR LESS 1 OR GDAL_VERSION_MINOR LESS 6)
            message(FATAL_ERROR "GDAL version is too old (${GDAL_VERSION}). Use 1.6.0 or higher.")
        endif()

        # Set INCLUDE_DIR to prefix+include
        exec_program(${GDAL_CONFIG} ARGS --prefix OUTPUT_VARIABLE GDAL_PREFIX)

        find_path(GDAL_INCLUDE_DIR
            gdal.h
            PATH_PREFIXES gdal gdal-1.6
            PATHS
            ${GDAL_PREFIX}/include/gdal
            ${GDAL_PREFIX}/include
            /usr/local/include
            /usr/include)

        # Extract link dirs for rpath
        exec_program(${GDAL_CONFIG} ARGS --libs OUTPUT_VARIABLE GDAL_CONFIG_LIBS)

        # Split off the link dirs (for rpath)
        # Use regular expression to match wildcard equivalent "-L*<endchar>"
        # with <endchar> is a space or a semicolon
        string(REGEX MATCHALL "[-][L]([^ ;])+" GDAL_LINK_DIRECTORIES_WITH_PREFIX "${GDAL_CONFIG_LIBS}")
        #message("DBG GDAL_LINK_DIRECTORIES_WITH_PREFIX=${GDAL_LINK_DIRECTORIES_WITH_PREFIX}")

        # Remove prefix -L because we need the pure directory for LINK_DIRECTORIES
        if(GDAL_LINK_DIRECTORIES_WITH_PREFIX)
            string(REGEX REPLACE "[-][L]" "" GDAL_LINK_DIRECTORIES "${GDAL_LINK_DIRECTORIES_WITH_PREFIX}" )
            #message("DBG GDAL_LINK_DIRECTORIES ${GDAL_LINK_DIRECTORIES}")
        endif()

        # Split off the name
        # use regular expression to match wildcard equivalent "-l*<endchar>"
        # with <endchar> is a space or a semicolon
        string(REGEX MATCHALL "[-][l]([^ ;])+" GDAL_LIB_NAME_WITH_PREFIX "${GDAL_CONFIG_LIBS}")

        # Remove prefix -l because we need the pure name
        if(GDAL_LIB_NAME_WITH_PREFIX)
            string(REGEX REPLACE "[-][l]" "" GDAL_LIB_NAME ${GDAL_LIB_NAME_WITH_PREFIX})
        endif()

        if(APPLE)
            set(GDAL_LIBRARY ${GDAL_LINK_DIRECTORIES}/lib${GDAL_LIB_NAME}.dylib CACHE STRING INTERNAL)
        else()
            set(GDAL_LIBRARY ${GDAL_LINK_DIRECTORIES}/lib${GDAL_LIB_NAME}.so CACHE STRING INTERNAL)
        endif()

    else()
        message("FindGDAL.cmake: gdal-config not found. Please set it manually: GDAL_CONFIG=${GDAL_CONFIG}")
    endif(GDAL_CONFIG)

else()
    message("FindGDAL.cmake: unrecognized or unsupported operating system (use Unix or Windows)")
endif()

# Handle the QUIETLY and REQUIRED arguments and set SPATIALINDEX_FOUND to TRUE
# if all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GDAL DEFAULT_MSG GDAL_LIBRARY GDAL_INCLUDE_DIR)

# TODO: Do we want to mark these as advanced? --mloskot
# http://www.cmake.org/cmake/help/cmake2.6docs.html#command:mark_as_advanced
#mark_as_advanced(SPATIALINDEX_LIBRARY SPATIALINDEX_INCLUDE_DIR)
