set(proj OPENJPEG)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup OpenJpeg...")

if(USE_SYSTEM_OPENJPEG)
  # TODO : FindOpenJPEG.cmake
  # find_package ( OpenJPEG )
  add_custom_target(${proj})
  message(STATUS "  Using OpenJpeg system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using OpenJPEG SuperBuild version")
  
  # declare dependencies
  set(${proj}_DEPENDENCIES TIFF ZLIB)
  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj OPENJPEG)
  
  ADD_SUPERBUILD_CMAKE_VAR(TIFF_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(TIFF_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(ZLIB_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(ZLIB_LIBRARY)

  if(MSVC)
  #TODO: add LCMS dependency
  endif()
  
  ExternalProject_Add(${proj}
        PREFIX ${proj}
        URL "http://sourceforge.net/projects/openjpeg.mirror/files/2.1.0/openjpeg-2.1.0.tar.gz/download"
        URL_MD5 f6419fcc233df84f9a81eb36633c6db6
        BINARY_DIR ${OPENJPEG_SB_BUILD_DIR}
        INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
        CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_CODEC:BOOL=ON
        -DBUILD_DOC:BOOL=OFF
        -DBUILD_JPIP:BOOL=OFF
        -DBUILD_JPWL:BOOL=OFF
        -DBUILD_MJ2:BOOL=OFF
        -DBUILD_PKGCONFIG_FILES:BOOL=ON
        -DBUILD_SHARED_LIBS:BOOL=ON
        -DBUILD_TESTING:BOOL=OFF
        -DBUILD_THIRDPARTY:BOOL=OFF
        -DCMAKE_PREFIX_PATH:STRING=${SB_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
        ${OPENJPEG_SB_CONFIG}
        DEPENDS ${${proj}_DEPENDENCIES}
        CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )

  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/openjp2.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libopenjp2${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
 
endif()
endif()
