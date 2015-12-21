set(proj CURL)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup cURL ...")

if(USE_SYSTEM_CURL)
  find_package ( CURL )
  message(STATUS "  Using cURL system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using cURL SuperBuild version")
  
  # declare dependencies
  ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(${proj} ZLIB OPENSSL)

  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj CURL)
  
  if(USE_SYSTEM_GEOS) #why geos here?. discuss with GP.
    ADD_SUPERBUILD_CMAKE_VAR(ZLIB_ROOT)
  endif()
  
  #TODO: add openssl and other dependencies
  if(MSVC)
    ExternalProject_Add(${proj}
        PREFIX ${proj}
        URL "http://curl.haxx.se/download/curl-7.40.0.tar.gz"
        URL_MD5 58943642ea0ed050ab0431ea1caf3a6f
        SOURCE_DIR ${CURL_SB_SRC}
        BINARY_DIR ${CURL_SB_BUILD_DIR}/winbuild
        INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
        DEPENDS ${${proj}_DEPENDENCIES}        
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CURL_SB_SRC} ${CURL_SB_BUILD_DIR}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND nmake /f ${CURL_SB_BUILD_DIR}/winbuild/Makefile.vc mode=dll WITH_ZLIB=dll WITH_DEVEL=${SB_INSTALL_PREFIX}
        INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${CURL_SB_BUILD_DIR}/builds/ ${CMAKE_COMMAND} -E copy_directory libcurl-vc-x86-release-dll-zlib-dll-ipv6-sspi-winssl ${SB_INSTALL_PREFIX} 
    )
    
  else(UNIX)
    ExternalProject_Add(${proj}
        PREFIX ${proj}
        URL "http://curl.haxx.se/download/curl-7.40.0.tar.gz"
        URL_MD5 58943642ea0ed050ab0431ea1caf3a6f
        BINARY_DIR ${CURL_SB_BUILD_DIR}
        INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
        CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_PREFIX_PATH:STRING=${SB_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        -DBUILD_CURL_EXE:BOOL=ON
        -DBUILD_CURL_TESTS:BOOL=OFF
        ${CURL_SB_CONFIG}
        DEPENDS ${${proj}_DEPENDENCIES}
    )
  endif()
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libcurl.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libcurl${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
