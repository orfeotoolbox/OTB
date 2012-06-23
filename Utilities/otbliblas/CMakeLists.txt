PROJECT(otbliblas)

SET(liblas_VERSION_MAJOR "1")
SET(liblas_VERSION_MINOR "6")
SET(liblas_VERSION_PATCH "0")

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

SET(liblas_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

SET(liblas_SRCS
  #las src
  "${liblas_SOURCES_DIR}/las_c_api.cpp"
  "${liblas_SOURCES_DIR}/lasclassification.cpp"
  "${liblas_SOURCES_DIR}/lascolor.cpp"
  "${liblas_SOURCES_DIR}/lasdimension.cpp"
  "${liblas_SOURCES_DIR}/laserror.cpp"
  "${liblas_SOURCES_DIR}/lasfilter.cpp"
  "${liblas_SOURCES_DIR}/lasheader.cpp"
  "${liblas_SOURCES_DIR}/lasindex.cpp"
  "${liblas_SOURCES_DIR}/laspoint.cpp"
  "${liblas_SOURCES_DIR}/lasreader.cpp"
  "${liblas_SOURCES_DIR}/lasspatialreference.cpp"
  "${liblas_SOURCES_DIR}/lasschema.cpp"
  "${liblas_SOURCES_DIR}/lastransform.cpp"
  "${liblas_SOURCES_DIR}/utility.cpp"
  "${liblas_SOURCES_DIR}/lasvariablerecord.cpp"
  "${liblas_SOURCES_DIR}/laswriter.cpp"
  #index
  "${liblas_SOURCES_DIR}/detail/index/indexcell.cpp"
  "${liblas_SOURCES_DIR}/detail/index/indexoutput.cpp"
  #reader
  "${liblas_SOURCES_DIR}/detail/reader/reader.cpp"
  "${liblas_SOURCES_DIR}/detail/reader/point.cpp"
  "${liblas_SOURCES_DIR}/detail/reader/header.cpp"
  "${liblas_SOURCES_DIR}/detail/reader/cachedreader.cpp"
  #writer
  "${liblas_SOURCES_DIR}/detail/writer/writer.cpp"
  "${liblas_SOURCES_DIR}/detail/writer/point.cpp"
  "${liblas_SOURCES_DIR}/detail/writer/base.cpp"
  "${liblas_SOURCES_DIR}/detail/writer/header.cpp"
  #detail 
  "${liblas_SOURCES_DIR}/detail/utility.cpp"
  )

#include Gdal and GeoTiff support only if version >= 1.6
#COMPARE_VERSION_STRINGS(GDAL_VERSION VERSION_LESS 1.6 result)
#IF(result LESS 0)
##IF(GDAL_VERSION VERSION_LESS 1.6)
#  MESSAGE(STATUS "Disabling GDAL/GeoTIFF support for LibLAS (requires GDAL version >= 1.6)")
#ELSE()
#  MESSAGE(STATUS "Enabling GDAL/GeoTIFF support for LibLAS")
#  INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR} ${GEOTIFF_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})
#
#  LIST(APPEND liblas_SRCS
#       "${liblas_SOURCES_DIR}/gt_citation.cpp"
#       "${liblas_SOURCES_DIR}/gt_wkt_srs.cpp"
#       "${liblas_SOURCES_DIR}/tifvsi.cpp")
#
#  ADD_DEFINITIONS(-DHAVE_GDAL -DHAVE_LIBGEOTIFF)
#ENDIF()

IF(NOT OTB_DISABLE_UTILITIES_COMPILATION)
ADD_DEFINITIONS(-DLAS_DISABLE_DLL)
ADD_LIBRARY(otbliblas ${liblas_SRCS} )

#IF(NOT GDAL_VERSION VERSION_LESS 1.6)
#  TARGET_LINK_LIBRARIES(otbliblas ${GDAL_LIBRARY} ${GEOTIFF_LIBRARY} ${TIFF_LIBRARY})
#ENDIF(NOT GDAL_VERSION VERSION_LESS 1.6)

IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(otbliblas PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbliblas
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/*.hpp")
  INSTALL(FILES ${__files1}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas
    COMPONENT Development)
  FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/detail/*.hpp")
  INSTALL(FILES ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas/detail
    COMPONENT Development)
  FILE(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/capi/*.h")
  INSTALL(FILES ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas/capi
    COMPONENT Development)
  FILE(GLOB __files4 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/external/property_tree/*.hpp")
  INSTALL(FILES ${__files4}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas/external/property_tree
    COMPONENT Development)
  FILE(GLOB __files5 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/external/property_tree/detail/*.hpp")
  INSTALL(FILES ${__files5}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas/external/property_tree/detail
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)

ENDIF(NOT OTB_DISABLE_UTILITIES_COMPILATION)
