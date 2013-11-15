include_regular_expression("^.*$")
# Defines the source code for the library
set(OPENJPEG_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/bio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/dwt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/event.c
  ${CMAKE_CURRENT_SOURCE_DIR}/image.c
  ${CMAKE_CURRENT_SOURCE_DIR}/j2k.c
  ${CMAKE_CURRENT_SOURCE_DIR}/j2k_lib.c
  ${CMAKE_CURRENT_SOURCE_DIR}/jp2.c
  ${CMAKE_CURRENT_SOURCE_DIR}/jpt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mct.c
  ${CMAKE_CURRENT_SOURCE_DIR}/mqc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/openjpeg.c
  ${CMAKE_CURRENT_SOURCE_DIR}/pi.c
  ${CMAKE_CURRENT_SOURCE_DIR}/raw.c
  ${CMAKE_CURRENT_SOURCE_DIR}/t1.c
  ${CMAKE_CURRENT_SOURCE_DIR}/t2.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tcd.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tgt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/cidx_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/phix_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/ppix_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/thix_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/tpix_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/function_list.c
)

# Build the library
if(WIN32)
  if(BUILD_SHARED_LIBS)
    add_definitions(-DOPJ_EXPORTS)
  else()
    add_definitions(-DOPJ_STATIC)
  endif()
endif()
add_library(${OPENJPEG_LIBRARY_NAME} ${OPENJPEG_SRCS})
if(UNIX)
  target_link_libraries(${OPENJPEG_LIBRARY_NAME} m)
endif()
set_target_properties(${OPENJPEG_LIBRARY_NAME} PROPERTIES ${OPENJPEG_LIBRARY_PROPERTIES})

# Build the JPWL library ?
if(BUILD_JPWL)
 add_subdirectory(jpwl)
endif()

# Install library
install(TARGETS ${OPENJPEG_LIBRARY_NAME}
  EXPORT OpenJPEGTargets
  DESTINATION ${OPENJPEG_INSTALL_LIB_DIR}
  COMPONENT RuntimeLibraries
)

# Install includes files
#install(FILES openjpeg.h
#  DESTINATION ${OPENJPEG_INSTALL_INCLUDE_DIR}/${OPENJPEG_INSTALL_SUBDIR} COMPONENT Headers
#)
#install(CODE
#  "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${OPENJPEG_INSTALL_SUBDIR}/openjpeg.h \$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${OPENJPEG_INSTALL_INCLUDE_DIR}/openjpeg.h)")

# install man page of the library
#install(
#  FILES       ${OPENJPEG_SOURCE_DIR}/doc/man/man3/libopenjpeg.3
#  DESTINATION ${OPENJPEG_INSTALL_MAN_DIR}/man3)
