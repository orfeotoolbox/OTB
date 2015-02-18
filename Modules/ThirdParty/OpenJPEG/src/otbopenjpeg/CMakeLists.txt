project(OTB3P_OpenJPEG C)

set(OTB3P_INSTALL_EXPORT_NAME "${OTBOpenJPEG-targets}")
set(OTB3P_INSTALL_INCLUDE_DIR "${OTBOpenJPEG_INSTALL_INCLUDE_DIR}")
set(OTB3P_INSTALL_RUNTIME_DIR "${OTBOpenJPEG_INSTALL_RUNTIME_DIR}")
set(OTB3P_INSTALL_LIBRARY_DIR "${OTBOpenJPEG_INSTALL_LIBRARY_DIR}")
set(OTB3P_INSTALL_ARCHIVE_DIR "${OTBOpenJPEG_INSTALL_ARCHIVE_DIR}")

set(OPENJPEG_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/bio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/cio.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/dwt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/event.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/image.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/j2k.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/j2k_lib.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/jp2.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/jpt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/mct.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/mqc.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/openjpeg.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/pi.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/raw.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/t1.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/t2.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/tcd.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/tgt.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/cidx_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/phix_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/ppix_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/thix_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/tpix_manager.c
  ${CMAKE_CURRENT_SOURCE_DIR}/libopenjpeg/function_list.c
)

set(CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
    ${CMAKE_MODULE_PATH})
    
include( CMakeBackwardCompatibilityC )

include( TestBigEndian )
TEST_BIG_ENDIAN(OPJ_BIG_ENDIAN)

if(CMAKE_COMPILER_IS_GNUCC)
  # For all builds, make sure openjpeg is std99 compliant:
  # set(CMAKE_C_FLAGS "-Wall -std=c99 ${CMAKE_C_FLAGS}") # FIXME: this setting prevented us from setting a coverage build.
  # Do not use ffast-math for all build, it would produce incorrect results, only set for release:
  set(CMAKE_C_FLAGS_RELEASE "-ffast-math ${CMAKE_C_FLAGS_RELEASE}")
endif()

include( CheckIncludeFile )
CHECK_INCLUDE_FILE("strings.h"      HAVE_STRINGS_H)
CHECK_INCLUDE_FILE("inttypes.h"     HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE("memory.h"       HAVE_MEMORY_H)
CHECK_INCLUDE_FILE("stdint.h"       HAVE_STDINT_H)
CHECK_INCLUDE_FILE("stdlib.h"       HAVE_STDLIB_H)
CHECK_INCLUDE_FILE("string.h"       HAVE_STRING_H)
CHECK_INCLUDE_FILE("sys/stat.h"     HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE("sys/types.h"    HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE("unistd.h"       HAVE_UNISTD_H)

# Enable Large file support
include( TestLargeFiles )
OPJ_TEST_LARGE_FILES(OPJ_HAVE_LARGEFILES)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/opj_config.h.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/opj_config.h
  @ONLY
  )

# Rename all symbols with a "otb_" prefix to avoid conflicts
# with other projects building openjpeg internally
set(OPJ_USE_MANGLE_PREFIX ON)
set(OPJ_MANGLE_PREFIX otbopenjpeg)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/openjpeg_mangle.h.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/openjpeg_mangle.h
  @ONLY IMMEDIATE
  )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_library(otbopenjpeg ${OPENJPEG_SRCS})

if(UNIX)
  target_link_libraries(otbopenjpeg m)
endif()

if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbopenjpeg PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

install(TARGETS otbopenjpeg
  EXPORT ${OTB3P_INSTALL_EXPORT_NAME}
  RUNTIME DESTINATION ${OTB3P_INSTALL_RUNTIME_DIR} COMPONENT RuntimeLibraries
  LIBRARY DESTINATION ${OTB3P_INSTALL_LIBRARY_DIR} COMPONENT RuntimeLibraries
  ARCHIVE DESTINATION ${OTB3P_INSTALL_ARCHIVE_DIR} COMPONENT Development
  )

# Purposely NOT installing headers

otb_module_target( otbopenjpeg NO_INSTALL )