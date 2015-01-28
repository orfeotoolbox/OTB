message(STATUS "Setup cURL ...")

set(proj CURL)

set(DEFAULT_USE_SYSTEM_CURL  ON)

option(USE_SYSTEM_CURL "  Use a system build of cURL." ${DEFAULT_USE_SYSTEM_CURL})
mark_as_advanced(USE_SYSTEM_CURL)

if(USE_SYSTEM_CURL)
  message(STATUS "  Using cURL system version")
else()
  set(${proj}_DEPENDENCIES)
  set(CURL_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  set(CURL_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
    
  if(USE_SYSTEM_ZLIB)
    set(CURL_SB_ZLIB_CONFIG)
  else()
    set(CURL_SB_ZLIB_CONFIG 
      -DZLIB_ROOT:STRING=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  #TODO: add openssl and other dependencies
  if(MSVC)
    ExternalProject_Add(${proj}
        PREFIX ${proj}
        URL "http://curl.haxx.se/download/curl-7.40.0.tar.gz"
        URL_MD5 58943642ea0ed050ab0431ea1caf3a6f
        SOURCE_DIR ${CURL_SB_SRC}
        BINARY_DIR ${CURL_SB_BUILD_DIR}/winbuild
        INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CURL_SB_SRC} ${CURL_SB_BUILD_DIR}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${SB_MAKE_CMD} /f ${CURL_SB_BUILD_DIR}/winbuild/Makefile.vc mode=dll WITH_ZLIB=dll WITH_DEVEL=${CMAKE_INSTALL_PREFIX}
        INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir ${CURL_SB_BUILD_DIR}/builds/ ${CMAKE_COMMAND} -E copy_directory libcurl-vc-x86-release-dll-zlib-dll-ipv6-sspi-winssl ${CMAKE_INSTALL_PREFIX} 
    )
    
  else(UNIX)
    ExternalProject_Add(${proj}
        PREFIX ${proj}
        URL "http://curl.haxx.se/download/curl-7.40.0.tar.gz"
        URL_MD5 58943642ea0ed050ab0431ea1caf3a6f
        BINARY_DIR ${CURL_SB_BUILD_DIR}
        INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
        CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        -DBUILD_CURL_EXE:BOOL=ON
        -DBUILD_CURL_TESTS:BOOL=OFF
        ${CURL_SB_ZLIB_CONFIG}
        DEPENDS ${${proj}_DEPENDENCIES}
    )
  endif()
  message(STATUS "  Using cURL SuperBuild version")

endif()