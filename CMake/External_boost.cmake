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
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://sourceforge.net/projects/boost/files/boost/1.57.0/boost_1_57_0.tar.gz/download"
    URL_MD5 25f9a8ac28beeb5ab84aa98510305299
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
endif()
