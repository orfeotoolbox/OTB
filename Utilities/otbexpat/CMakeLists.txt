# Sources of non-templated classes.

project(libexpat)

set(libexpat_VERSION_MAJOR "2")
set(libexpat_VERSION_MINOR "0")
set(libexpat_VERSION_PATCH "1")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the library.
set(libexpat_VERSION_STRING "${libexpat_VERSION_MAJOR}.${libexpat_VERSION_MINOR}.${libexpat_VERSION_PATCH}")


# Allow sources in subdirectories to see the include files.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Build libexpat library
add_definitions(-DHAVE_EXPAT_CONFIG_H)
file(GLOB EXPAT_SRCS "lib/*.c" )

# Adjust the compiler flags to avoid problems with ossim code.
#if(CMAKE_COMPILER_IS_GNUCC)
#  foreach(f ${EXPAT_SRCS} )
#        set_source_files_properties( ${f} PROPERTIES COMPILE_FLAGS -w )
#  endforeach(f)
#endif(CMAKE_COMPILER_IS_GNUCC)


include(CheckIncludeFiles)
CHECK_INCLUDE_FILES( "dlfcn.h" OTB_EXPAT_HAVE_DLFCN_H )
CHECK_INCLUDE_FILES( "fcntl.h" OTB_EXPAT_HAVE_FCNTL_H )
CHECK_INCLUDE_FILES( "inttypes.h" OTB_EXPAT_HAVE_INTTYPES_H )
CHECK_INCLUDE_FILES( "memory.h" OTB_EXPAT_HAVE_MEMORY_H )
CHECK_INCLUDE_FILES( "stdint.h" OTB_EXPAT_HAVE_STDINT_H )
CHECK_INCLUDE_FILES( "stdlib.h" OTB_EXPAT_HAVE_STDLIB_H )
CHECK_INCLUDE_FILES( "strings.h" OTB_EXPAT_HAVE_STRINGS_H )
CHECK_INCLUDE_FILES( "string.h" OTB_EXPAT_HAVE_STRING_H )
CHECK_INCLUDE_FILES( "sys/stat.h" OTB_EXPAT_HAVE_SYS_STAT_H )
CHECK_INCLUDE_FILES( "sys/types.h" OTB_EXPAT_HAVE_SYS_TYPES_H )
CHECK_INCLUDE_FILES( "unistd.h" OTB_EXPAT_HAVE_UNISTD_H )


include(CheckFunctionExists)
CHECK_FUNCTION_EXISTS( "bcopy" OTB_EXPAT_HAVE_MEMMOVE )
CHECK_FUNCTION_EXISTS( "getpagesize" OTB_EXPAT_HAVE_GETPAGESIZE )
CHECK_FUNCTION_EXISTS( "memmove" OTB_EXPAT_HAVE_MEMMOVE )

CHECK_FUNCTION_EXISTS( "mmap" OTB_EXPAT_HAVE_MMAP )

if(WIN32)
  if(NOT BORLAND)
    if(NOT CYGWIN)
      if(NOT MINGW)
          add_definitions(-DCOMPILED_FROM_DSP)
      endif(NOT MINGW)
    endif(NOT CYGWIN)
  endif(NOT BORLAND)
endif(WIN32)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake_expat_config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/expat_config.h)


add_library(otbexpat ${EXPAT_SRCS} )
#target_link_libraries(otbkml)
if(OTB_LIBRARY_PROPERTIES)
  set_target_properties(otbexpat PROPERTIES ${OTB_LIBRARY_PROPERTIES})
endif(OTB_LIBRARY_PROPERTIES)

if(NOT OTB_INSTALL_NO_LIBRARIES)
  install(TARGETS otbexpat
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
endif(NOT OTB_INSTALL_NO_LIBRARIES)

if(NOT OTB_INSTALL_NO_DEVELOPMENT)
  set(__files     ${CMAKE_CURRENT_SOURCE_DIR}/otb_expat.h
                ${CMAKE_CURRENT_SOURCE_DIR}/otb_expat_mangle.h)
  install(FILES ${__files}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbexpat
    COMPONENT Development)
  set(__files2    ${CMAKE_CURRENT_SOURCE_DIR}/lib/expat_external.h
                ${CMAKE_CURRENT_SOURCE_DIR}/lib/expat.h)
  install(FILES ${__files2}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbexpat/lib
    COMPONENT Development)
endif(NOT OTB_INSTALL_NO_DEVELOPMENT)
