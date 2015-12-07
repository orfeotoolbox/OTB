set(proj JPEG)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup libjpeg...")

if(USE_SYSTEM_JPEG)
  find_package ( JPEG )
  message(STATUS "  Using libjpeg system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using libjpeg SuperBuild version")

  include(CheckTypeSize)
  check_type_size("size_t" JPEG_SIZEOF_SIZE_T)

  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/libjpeg-turbo/files/1.4.1/libjpeg-turbo-1.4.1.tar.gz"
    URL_MD5 b1f6b84859a16b8ebdcda951fa07c3f2
    SOURCE_DIR ${JPEG_SB_SRC}
    BINARY_DIR ${JPEG_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    DEPENDS ${${proj}_DEPENDENCIES}
    CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_C_FLAGS=-DSIZEOF_SIZE_T=${JPEG_SIZEOF_SIZE_T}
    -DCMAKE_VERBOSE_MAKEFILE=TRUE
    -DENABLE_SHARED=TRUE
    -DENABLE_STATIC=FALSE
    -DWITH_SIMD=FALSE
    -DWITH_TURBOJPEG=FALSE
    CMAKE_COMMAND ${SB_CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX=${SB_INSTALL_PREFIX}
    )

  if(MSVC)
    ExternalProject_Add_Step(${proj} patch_jconfigint
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/${proj}/jconfigint.h.in ${JPEG_SB_SRC}/win/
      DEPENDERS configure)
  endif()
  if(UNIX)
    ExternalProject_Add_Step(${proj} patch_setmode
      COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/patches/${proj}/patch.cmake
      DEPENDERS configure)
  endif()


  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
    if(WIN32)
      set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/jpeg.lib)
    elseif(UNIX)
      set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libjpeg${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()

endif()
endif()
