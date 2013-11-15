project(otbliblas)

set(liblas_VERSION_MAJOR "1")
set(liblas_VERSION_MINOR "6")
set(liblas_VERSION_PATCH "0")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(liblas_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(liblas_SRCS
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
#if(result LESS 0)
##if(GDAL_VERSION VERSION_LESS 1.6)
#  message(STATUS "Disabling GDAL/GeoTIFF support for LibLAS (requires GDAL version >= 1.6)")
#else()
#  message(STATUS "Enabling GDAL/GeoTIFF support for LibLAS")
#  include_directories(${GDAL_INCLUDE_DIR} ${GEOTIFF_INCLUDE_DIRS} ${TIFF_INCLUDE_DIRS})
#
#  list(APPEND liblas_SRCS
#       "${liblas_SOURCES_DIR}/gt_citation.cpp"
#       "${liblas_SOURCES_DIR}/gt_wkt_srs.cpp"
#       "${liblas_SOURCES_DIR}/tifvsi.cpp")
#
#  add_definitions(-DHAVE_GDAL -DHAVE_LIBGEOTIFF)
#endif()

add_definitions(-DLAS_DISABLE_DLL)
add_library(otbliblas ${liblas_SRCS} )

#if(NOT GDAL_VERSION VERSION_LESS 1.6)
#  target_link_libraries(otbliblas ${GDAL_LIBRARY} ${GEOTIFF_LIBRARY} ${TIFF_LIBRARY})
#endif(NOT GDAL_VERSION VERSION_LESS 1.6)

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbliblas PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS otbliblas
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/*.hpp")
  install(FILES ${__files1}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas
    COMPONENT Development)
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/detail/*.hpp")
  install(FILES ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas/detail
    COMPONENT Development)
  file(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/capi/*.h")
  install(FILES ${__files3}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas/capi
    COMPONENT Development)
  file(GLOB __files4 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/external/property_tree/*.hpp")
  install(FILES ${__files4}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas/external/property_tree
    COMPONENT Development)
  file(GLOB __files5 "${CMAKE_CURRENT_SOURCE_DIR}/include/liblas/external/property_tree/detail/*.hpp")
  install(FILES ${__files5}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbliblas/include/liblas/external/property_tree/detail
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
