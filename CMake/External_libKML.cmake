message(STATUS "Setup libKML ...")

set(proj LIBKML)

set(DEFAULT_USE_SYSTEM_LIBKML  OFF)

option(USE_SYSTEM_LIBKML "  Use a system build of libKML." ${DEFAULT_USE_SYSTEM_LIBKML})
mark_as_advanced(USE_SYSTEM_LIBKML)

if(USE_SYSTEM_LIBKML)
  message(STATUS "  Using libKML system version")
else()
  set(${proj}_DEPENDENCIES)
  set(LIBKML_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(LIBKML_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  if(USE_SYSTEM_EXPAT)
    set(LIBKML_SB_EXPAT_CONFIG)
  else()
    set(LIBKML_SB_EXPAT_CONFIG 
      -DEXPAT_INCLUDE_DIR:STRING=${CMAKE_INSTALL_PREFIX}/include
      -DEXPAT_LIBRARY:STRING=${CMAKE_INSTALL_PREFIX}/lib/libexpat.so
      )
    list(APPEND ${proj}_DEPENDENCIES EXPAT)
  endif()
  
  if(USE_SYSTEM_BOOST)
    set(LIBKML_SB_BOOST_CONFIG)
  else()
    set(LIBKML_SB_BOOST_CONFIG 
      -DBOOST_ROOT:STRING=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES BOOST)
  endif()
  
  if(USE_SYSTEM_ZLIB)
    set(LIBKML_SB_ZLIB_CONFIG)
  else()
    set(LIBKML_SB_ZLIB_CONFIG 
      -DZLIB_ROOT:STRING=${CMAKE_INSTALL_PREFIX}
      )
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://ftp.de.debian.org/debian/pool/main/libk/libkml/libkml_1.3.0~r863.orig.tar.gz"
    URL_MD5 211ed5fdf2dd45aeb9c0abc8e1fe42be
    BINARY_DIR ${LIBKML_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      ${LIBKML_SB_EXPAT_CONFIG}
      ${LIBKML_SB_BOOST_CONFIG}
      ${LIBKML_SB_ZLIB_CONFIG}
    #CMAKE_COMMAND
    #  # use 'env' because CTest launcher doesn't perform shell interpretation
    #  env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    #  ${CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory 
      ${CMAKE_SOURCE_DIR}/patches/${proj}
      ${LIBKML_SB_SRC}
    )
  
  message(STATUS "  Using libKML SuperBuild version")

endif()