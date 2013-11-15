# Makefile for the main JPWL OpenJPEG codecs: JPWL_ j2k_to_image and JPWL_image_to_j2k

add_definitions(-DUSE_JPWL)

set(JPWL_SRCS crc.c jpwl.c jpwl_lib.c rs.c)

include_directories(
  ${OPENJPEG_SOURCE_DIR}/libopenjpeg
  )

# Build the library
if(WIN32)
  if(BUILD_SHARED_LIBS)
    add_definitions(-DOPJ_EXPORTS)
  else()
    add_definitions(-DOPJ_STATIC)
  endif()
endif()
add_library(${OPENJPEG_LIBRARY_NAME}_JPWL ${JPWL_SRCS} ${OPENJPEG_SRCS})
set_target_properties(${OPENJPEG_LIBRARY_NAME}_JPWL
  PROPERTIES ${OPENJPEG_LIBRARY_PROPERTIES})

# Install library
install(TARGETS ${OPENJPEG_LIBRARY_NAME}_JPWL
  DESTINATION ${OPENJPEG_INSTALL_LIB_DIR} COMPONENT Libraries
)
