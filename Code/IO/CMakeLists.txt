# Sources of non-templated classes.

file(GLOB OTBIO_SRCS "*.cxx" )

# Remove the otbTestDriver cause only an executable is nedded
list(REMOVE_ITEM OTBIO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbTestDriver.cxx" )

if(NOT OTB_USE_JPEG2000)
    list(REMOVE_ITEM OTBIO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbJPEG2000ImageIO.cxx" )
    list(REMOVE_ITEM OTBIO_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/otbJPEG2000ImageIOFactory.cxx" )
    add_definitions(-DOTB_JPEG2000_DISABLED)
endif(NOT OTB_USE_JPEG2000)

# otbopenjpeg
if(OTB_USE_JPEG2000)

  if(WIN32)
    if(OTB_BUILD_SHARED_LIBS)
      add_definitions(-DOPJ_EXPORTS)
    else(OTB_BUILD_SHARED_LIBS)
      add_definitions(-DOPJ_STATIC)
    endif(OTB_BUILD_SHARED_LIBS)
  endif(WIN32)

  # This is the only place where we need to include openjpeg headers
  # Use BEFORE since we can have a conflict with system-installed openjpeg
  # and we want to ensure the internal version is used for now
  include_directories(BEFORE
                      ${OTB_SOURCE_DIR}/Utilities/otbopenjpeg/libopenjpeg
                      ${OTB_BINARY_DIR}/Utilities/otbopenjpeg )

endif(OTB_USE_JPEG2000)


add_library(OTBIO ${OTBIO_SRCS})
#Apparently not supported by opensolaris at least
#  set_target_properties(OTBIO
#    PROPERTIES
#    LINK_INTERFACE_LIBRARIES ""
#  )
target_link_libraries(OTBIO  ${TIFF_LIBRARY} ${GEOTIFF_LIBRARY}  ${GDAL_LIBRARY} ${OGR_LIBRARY} ${JPEG_LIBRARY} ${OPENTHREADS_LIBRARY})
target_link_libraries(OTBIO ${LIBKML_LIBRARIES} ${TINYXML_LIBRARIES} ${LIBLAS_LIBRARIES})
target_link_libraries(OTBIO  OTBCommon OTBBasicFilters OTBCurlAdapters OTBOGRAdapters)
target_link_libraries(OTBIO ${ITK_LIBRARIES})

if( OTB_USE_JPEG2000 )
    target_link_libraries(OTBIO otbopenjpeg)
endif( OTB_USE_JPEG2000 )

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(OTBIO PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)


if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS OTBIO
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)

  file(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
  file(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")

  if(NOT OTB_USE_JPEG2000)
     list(REMOVE_ITEM __files1 "${CMAKE_CURRENT_SOURCE_DIR}/otbJpeg2000ImageIOFactory.h" )
     list(REMOVE_ITEM __files1 "${CMAKE_CURRENT_SOURCE_DIR}/otbJpeg2000ImageIO.h" )
  endif(NOT OTB_USE_JPEG2000)


  install(FILES ${__files1} ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/IO
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
