###############################################################################
#
# test/unit/CMakeLists.txt controls building of libLAS unit tests suite
#
# Copyright (c) 2009 Mateusz Loskot <mateusz@loskot.net>
#
###############################################################################
set(LIBLAS_UNIT_TEST liblas_test)

set(LIBLAS_UNIT_TEST_SRC
    common.cpp
    guid_test.cpp
    lasbounds_test.cpp
    laserror_test.cpp
    lasheader_test.cpp
    laspoint_test.cpp
    lasreader_iterator_test.cpp
    lasreader_test.cpp
    lasspatialreference_test.cpp
    lasvariablerecord_test.cpp
    laswriter_test.cpp
    liblas_test_suite.cpp)

include_directories(
    .
    ../../include
    ${GDAL_INCLUDE_DIR}
    ${GEOTIFF_INCLUDE_DIR})

add_executable(${LIBLAS_UNIT_TEST} ${LIBLAS_UNIT_TEST_SRC} )

set_target_properties(${LIBLAS_UNIT_TEST} PROPERTIES COMPILE_DEFINITIONS LAS_DLL_IMPORT)

target_link_libraries(${LIBLAS_UNIT_TEST}
    ${LIBLAS_LIB_NAME}
    ${ZLIB_LIBRARIES}
    ${TIFF_LIBRARY}
    ${GEOTIFF_LIBRARY}
    ${GDAL_LIBRARY}
    ${SPATIALINDEX_LIBRARY})

add_test(liblas_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/liblas_test ${CMAKE_SOURCE_DIR}/test/data)
