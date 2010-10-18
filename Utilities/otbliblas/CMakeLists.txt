PROJECT(otbliblas)

SET(liblas_VERSION_MAJOR "1")
SET(liblas_VERSION_MINOR "2")
SET(liblas_VERSION_PATCH "1")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(liblas_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
SET(liblas_SRCS
    "${liblas_SOURCES_DIR}/las_c_api.cpp"
    "${liblas_SOURCES_DIR}/lascolor.cpp"
    "${liblas_SOURCES_DIR}/laserror.cpp"
    "${liblas_SOURCES_DIR}/lasfile.cpp"
    "${liblas_SOURCES_DIR}/lasheader.cpp"
    "${liblas_SOURCES_DIR}/laspoint.cpp"
    "${liblas_SOURCES_DIR}/lasreader.cpp"
    "${liblas_SOURCES_DIR}/lasspatialreference.cpp"
    "${liblas_SOURCES_DIR}/lasvariablerecord.cpp"
    "${liblas_SOURCES_DIR}/laswriter.cpp"
    "${liblas_SOURCES_DIR}/tifvsi.cpp"
    "${liblas_SOURCES_DIR}/detail/file.cpp"
    "${liblas_SOURCES_DIR}/detail/reader11.cpp"
    "${liblas_SOURCES_DIR}/detail/reader.cpp"
    "${liblas_SOURCES_DIR}/detail/writer11.cpp"
    "${liblas_SOURCES_DIR}/detail/writer.cpp"
    "${liblas_SOURCES_DIR}/detail/reader10.cpp"
    "${liblas_SOURCES_DIR}/detail/reader12.cpp"
    "${liblas_SOURCES_DIR}/detail/writer10.cpp"
    "${liblas_SOURCES_DIR}/detail/writer12.cpp")

#include Gdal and GeoTiff support only if version >= 1.6
IF(GDAL_VERSION VERSION_LESS 1.6)
  MESSAGE(STATUS "Disabling GDAL/GeoTIFF support for LibLAS (requires GDAL version >= 1.6)")
ELSE()
  MESSAGE(STATUS "Enabling GDAL/GeoTIFF support for LibLAS")
  INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR} ${GEOTIFF_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})

  LIST(APPEND liblas_SRCS
       "${liblas_SOURCES_DIR}/gt_citation.cpp"
       "${liblas_SOURCES_DIR}/gt_wkt_srs.cpp")

  ADD_DEFINITIONS(-DHAVE_GDAL -DHAVE_LIBGEOTIFF)
ENDIF()

IF(NOT OTB_DISABLE_UTILITIES_COMPILATION)


ADD_LIBRARY(otbliblas ${liblas_SRCS} )

IF(NOT GDAL_VERSION VERSION_LESS 1.6)
  TARGET_LINK_LIBRARIES(otbliblas ${GDAL_LIBRARY} ${GEOTIFF_LIBRARY} ${TIFF_LIBRARY})
ENDIF(NOT GDAL_VERSION VERSION_LESS 1.6)

IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(otbliblas PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbliblas
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR_CM24} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR_CM24} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/*.hpp")
  INSTALL(FILES ${__files1}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbliblas/include/liblas
    COMPONENT Development)
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/detail/*.hpp")
  INSTALL(FILES ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbliblas/include/liblas/detail
    COMPONENT Development)
  FILE(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/capi/*.h")
  INSTALL(FILES ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR_CM24}/Utilities/otbliblas/include/liblas/capi
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

ENDIF(NOT OTB_DISABLE_UTILITIES_COMPILATION)
