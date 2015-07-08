set(proj JPEG)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup libjpeg...")

if(USE_SYSTEM_JPEG)
  find_package ( JPEG )
  add_custom_target(${proj})
  message(STATUS "  Using libjpeg system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using libjpeg SuperBuild version")

  if(WIN32)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://sourceforge.net/projects/libjpeg-turbo/files/1.4.1/libjpeg-turbo-1.4.1.tar.gz"
      URL_MD5 b1f6b84859a16b8ebdcda951fa07c3f2
      SOURCE_DIR ${JPEG_SB_SRC}
      BINARY_DIR ${JPEG_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        -DWITH_SIMD:BOOL=OFF
        -DWITH_TURBOJPEG:BOOL=OFF        
        -DCMAKE_VERBOSE_MAKEFILE:BOOL=OFF
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/patches/${proj}/jconfigint.h.in ${JPEG_SB_SRC}/win/  )

    else()
      
      ExternalProject_Add(${proj}
        PREFIX ${proj}
        URL "http://sourceforge.net/projects/libjpeg-turbo/files/1.4.1/libjpeg-turbo-1.4.1.tar.gz"
        URL_MD5 b1f6b84859a16b8ebdcda951fa07c3f2
        BINARY_DIR ${JPEG_SB_BUILD_DIR}
        INSTALL_DIR ${SB_INSTALL_PREFIX}
        DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
        CONFIGURE_COMMAND
        ${JPEG_SB_BUILD_DIR}/configure
        --prefix=${SB_INSTALL_PREFIX}
        --enable-shared=yes
        --with-jpeg8
        --without-simd
        --without-java
        BUILD_COMMAND $(MAKE) bin_PROGRAMS=
        INSTALL_COMMAND $(MAKE) install bin_PROGRAMS=
        DEPENDS ${${proj}_DEPENDENCIES} )

      ExternalProject_Add_Step(${proj} copy_source
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${JPEG_SB_SRC} ${JPEG_SB_BUILD_DIR}
        DEPENDEES patch update
        DEPENDERS configure )

      ExternalProject_Add_Step(${proj} remove_static
        COMMAND ${CMAKE_COMMAND} -E remove -f ${SB_INSTALL_PREFIX}/lib/libturbojpeg.a
        DEPENDEES install  )
      
    endif()

  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
    if(WIN32)
      set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libjpeg.lib)
    elseif(UNIX)
      set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libjpeg${CMAKE_SHARED_LIBRARY_SUFFIX})
    endif()

endif()
endif()
