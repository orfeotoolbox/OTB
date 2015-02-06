message(STATUS "Setup Boost ...")

set(proj BOOST)

set(DEFAULT_USE_SYSTEM_BOOST  ON)

option(USE_SYSTEM_BOOST "  Use a system build of Boost." ${DEFAULT_USE_SYSTEM_BOOST})
mark_as_advanced(USE_SYSTEM_BOOST)

if(USE_SYSTEM_BOOST)
  message(STATUS "  Using Boost system version")
else()
  
  set(BOOST_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(BOOST_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(UNIX)
    ExternalProject_Add(${proj}
        PREFIX ${proj}
        URL "http://sourceforge.net/projects/boost/files/boost/1.54.0/boost_1_54_0.tar.gz/download"
        URL_MD5 efbfbff5a85a9330951f243d0a46e4b9
        BINARY_DIR ${BOOST_SB_BUILD_DIR}
        INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
        CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
        ${BOOST_SB_SRC}
    )
    
    message(STATUS "  Using Boost SuperBuild version")
    
  else(MSVC)
    #TODO: find hash for archives properly
    set(BOOST_HEADERS_URL_HASH 1605dc6085cb2dc778ef5ab6c0e59083)
    set(BOOST_GRAPH_URL_HASH c246516ca84a3c79ae8a0b22fceb0344)
    set(BOOST_REGEX_URL_HASH a3bfc2dc9a3ebe3c050518ecb29d6f8b)
    set(BOOST_UTF_URL_HASH 75971963a74c76d32e01974a8e48be11)
    
    if(MSVC10)
        set(BOOST_HEADERS_URL_HASH 1605dc6085cb2dc778ef5ab6c0e59083)
        set(BOOST_GRAPH_URL_HASH c246516ca84a3c79ae8a0b22fceb0344)
        set(BOOST_REGEX_URL_HASH a3bfc2dc9a3ebe3c050518ecb29d6f8b)
        set(BOOST_UTF_URL_HASH 75971963a74c76d32e01974a8e48be11)
    else(MSVC80)
 
    else(CMAKE_COMPILER_2005)
   
    endif()

    set(BOOST_HEADERS_URL "http://sourceforge.net/projects/boost/files/boost-binaries/1.50.0/boost_1_50_headers.zip/download")
    set(BOOST_GRAPH_URL "http://sourceforge.net/projects/boost/files/boost-binaries/1.50.0/libboost_graph-vc100-mt-1_50.zip/download")
    set(BOOST_UTF_URL "http://sourceforge.net/projects/boost/files/boost-binaries/1.50.0/boost_unit_test_framework-vc100-mt-1_50.zip/download")
    set(BOOST_REGEX_URL "http://sourceforge.net/projects/boost/files/boost-binaries/1.50.0/libboost_regex-vc100-mt-1_50.zip/download")

    ExternalProject_Add(${proj}_HEADERS
        PREFIX ${proj}/_HEADERS
        URL ${BOOST_HEADERS_URL}
        URL_MD5 ${BOOST_HEADERS_URL_HASH}
        SOURCE_DIR ${BOOST_SB_SRC}/include/boost
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${BOOST_SB_SRC}/include/ ${CMAKE_INSTALL_PREFIX}/include/
     )
    set(BOOSTLIBS)
    foreach(BOOSTLIB "REGEX" "GRAPH" "UTF")
        ExternalProject_Add(${proj}_${BOOSTLIB}
            PREFIX ${proj}/_${BOOSTLIB}
            URL ${BOOST_${BOOSTLIB}_URL}
            URL_MD5 ${BOOST_${BOOSTLIB}_URL_HASH} 
            SOURCE_DIR ${BOOST_SB_SRC}/lib
            DEPENDS ${proj}_HEADERS
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory ${BOOST_SB_SRC}/lib ${CMAKE_INSTALL_PREFIX}/lib
        )
    list(APPEND BOOSTLIBS ${proj}_${BOOSTLIB})
    endforeach()
    
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      DOWNLOAD_COMMAND ""
      SOURCE_DIR ${BOOST_SB_SRC}
      DEPENDS ${BOOSTLIBS}
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )

    message(STATUS "  Using Boost SuperBuild version (prebuilt binaries)")
    
  endif() #end else MSVC
  
endif()
