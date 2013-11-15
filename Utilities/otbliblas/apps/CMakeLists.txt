###############################################################################
#
# apps/CMakeLists.txt controls building of libLAS utilities
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
###############################################################################

include_directories(
    .
    ../include
    ../include/liblas/capi)

###############################################################################
# Collect programs to build

set(LASINFO_OLD lasinfo-old)
set(LASINFO lasinfo)
set(LASMERGE lasmerge)
set(LAS2LAS las2las)
set(LAS2LAS_OLD las2las-old)
set(LAS2TXT las2txt)
set(TXT2LAS txt2las)
set(TS2LAS ts2las)
set(LASBLOCK lasblock )

set(BIGFILE_TEST bigfile_test)
set(LASINDEX_TEST lasindex_test)

if(Boost_IOSTREAMS_FOUND)
  set(BIGFILE_BIO_TEST bigfile_boost_iostreams_test)
endif()

# Set the build type to release if it is not explicitly set by the user and
# isn't in the cache yet
if (NOT CMAKE_BUILD_TYPE )
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Utilities depending on 3rd-pary libraries
if(GDAL_FOUND)
    set(LAS2OGR las2ogr)
endif()

if(ORACLE_FOUND AND GDAL_FOUND)
    set(LAS2OCI las2oci)
endif()

set(LIBLAS_UTILITIES
    ${LASINFO_OLD} ${LASINFO} ${LASMERGE} ${LAS2LAS} ${LAS2TXT} ${TXT2LAS}
    ${LAS2OGR}  ${LAS2OCI} ${LAS2LAS} ${LAS2LAS_OLD} ${LASBLOCK} ${TS2LAS})

# TODO: Experimental and requires testing --mloskot
# Generate user-specific settings for Visual Studio project
set(VCPROJ_USER_REMOTE_MACHINE_DEBUG ${MACHINE_NAME})
set(VCPROJ_USER_ENVIRONMENT_DEBUG "${ENVIRONMENT_PATH}")

if(MSVC)
    foreach(utility ${LIBLAS_UTILITIES})
        set(USER_FILE ${utility}.vcproj.$ENV{USERDOMAIN}.$ENV{USERNAME}.user)
        set(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${USER_FILE})
        message(STATUS "Generating ${CMAKE_GENERATOR} user-specific settings in ${USER_FILE}")
        configure_file(${CMAKE_SOURCE_DIR}/cmake/libLAS.vcproj.user.template ${OUTPUT_PATH} @ONLY)
    endforeach()
endif()

###############################################################################
# Configure build targets

if(WIN32)
    add_definitions("-DLAS_DLL_EXPORT=1")
endif()


set(APPS_CPP_DEPENDENCIES
    ${LIBLAS_LIB_NAME}
    ${TIFF_LIBRARY}
    ${GEOTIFF_LIBRARY}
    ${GDAL_LIBRARY}
    ${SPATIALINDEX_LIBRARY}
    ${Boost_LIBRARIES})

link_directories(${Boost_LIBRARY_DIRS})

# Build lasinfo
if(LASINFO_OLD)
    set(LASINFO_OLD_SRC lascommon.c ${LASINFO_OLD}.c)
    add_executable(${LASINFO_OLD} ${LASINFO_OLD_SRC})
    target_link_libraries(${LASINFO_OLD} ${LIBLAS_C_LIB_NAME})
endif()

# Build las2las
if(LAS2LAS_OLD)
    set(LAS2LAS_OLD_SRC lascommon.c ${LAS2LAS_OLD}.c)
    add_executable(${LAS2LAS_OLD} ${LAS2LAS_OLD_SRC})
    target_link_libraries(${LAS2LAS_OLD} ${LIBLAS_C_LIB_NAME})
endif()

if(LAS2LAS)
    add_executable(${LAS2LAS} las2las.cpp laskernel.cpp)
    target_link_libraries(${LAS2LAS} ${APPS_CPP_DEPENDENCIES} )
endif()

if(LASINFO)
    add_executable(${LASINFO} lasinfo.cpp laskernel.cpp)
    target_link_libraries(${LASINFO} ${APPS_CPP_DEPENDENCIES} )
endif()

# Build las2txt
if(LAS2TXT)
    set(LAS2TXT_SRC lascommon.c ${LAS2TXT}.c)
    add_executable(${LAS2TXT} ${LAS2TXT_SRC})
    target_link_libraries(${LAS2TXT} ${LIBLAS_C_LIB_NAME})
endif()

# Build txt2las
if(TXT2LAS)
    set(TXT2LAS_SRC lascommon.c ${TXT2LAS}.c)
    add_executable(${TXT2LAS} ${TXT2LAS_SRC})
    target_link_libraries(${TXT2LAS} ${LIBLAS_C_LIB_NAME})
endif()

if(TS2LAS)
    add_executable(${TS2LAS} ts2las.cpp )
    target_link_libraries(${TS2LAS} ${APPS_CPP_DEPENDENCIES} )
endif()

# Build lasmerge
if(LASMERGE)
    set(LASMERGE_SRC lascommon.c ${LASMERGE}.c)
    add_executable(${LASMERGE} ${LASMERGE_SRC})
    target_link_libraries(${LASMERGE} ${LIBLAS_C_LIB_NAME})
endif()

# Build lasblock
if(LASBLOCK)
    set(LASBLOCK_SRC lasblock.cpp chipper.cpp chipper.hpp)
    add_executable(${LASBLOCK} ${LASBLOCK_SRC})
    target_link_libraries(${LASBLOCK} ${APPS_CPP_DEPENDENCIES} )
endif()

# Build las2ogr
if(LAS2OGR)
    add_executable(${LAS2OGR} las2ogr.cpp)
    target_link_libraries(${LAS2OGR} ${APPS_CPP_DEPENDENCIES})
endif()

# Build las2oci
if(LAS2OCI)
    add_executable(${LAS2OCI} las2oci.cpp oci_wrapper.cpp kdx_util.cpp oci_util.cpp laskernel.cpp chipper.cpp)
    target_link_libraries(${LAS2OCI} ${APPS_CPP_DEPENDENCIES} ${ORACLE_LIBRARY})
endif()

if(BIGFILE_TEST)
    add_executable(${BIGFILE_TEST} bigtest.c)
    target_link_libraries(${BIGFILE_TEST} ${LIBLAS_C_LIB_NAME})
endif()

if (LASINDEX_TEST)
    add_executable(${LASINDEX_TEST} lasindex_test.cpp)
    target_link_libraries(${LASINDEX_TEST} ${APPS_CPP_DEPENDENCIES})
endif()

if(BIGFILE_BIO_TEST)
    add_executable(${BIGFILE_BIO_TEST} bigfile_boost_iostreams_test.cpp)
    target_link_libraries(${BIGFILE_BIO_TEST} ${APPS_CPP_DEPENDENCIES} )
endif()

###############################################################################
# Targets installation

install(TARGETS ${LIBLAS_UTILITIES}
    RUNTIME DESTINATION ${LIBLAS_BIN_DIR}
    LIBRARY DESTINATION ${LIBLAS_LIB_DIR}
    ARCHIVE DESTINATION ${LIBLAS_LIB_DIR})
