set(proj LIBKML)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup libKML ...")

if(USE_SYSTEM_LIBKML)
  find_package ( LibKML REQUIRED )
  message(STATUS "  Using libKML system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  
  if(USE_SYSTEM_EXPAT)
    set(LIBKML_SB_EXPAT_CONFIG)
  else()
    set(LIBKML_SB_EXPAT_CONFIG 
      -DEXPAT_INCLUDE_DIR:STRING=${SB_INSTALL_PREFIX}/include
      -DEXPAT_LIBRARY:STRING=${SB_INSTALL_PREFIX}/lib/libexpat${CMAKE_SHARED_LIBRARY_SUFFIX}
      )
      if(MSVC)
        set(LIBKML_SB_EXPAT_CONFIG 
            -DEXPAT_INCLUDE_DIR:STRING=${SB_INSTALL_PREFIX}/include
            -DEXPAT_LIBRARY:STRING=${SB_INSTALL_PREFIX}/lib/expat.lib
        )  
      endif()
    list(APPEND ${proj}_DEPENDENCIES EXPAT)
  endif()
  
  if(USE_SYSTEM_BOOST)
    # TODO : handle specific prefix
  else()
    list(APPEND ${proj}_DEPENDENCIES BOOST)
  endif()
  
  if(USE_SYSTEM_ZLIB)
    # TODO : handle specific prefix
  else()
    list(APPEND ${proj}_DEPENDENCIES ZLIB)
  endif()

  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://ftp.de.debian.org/debian/pool/main/libk/libkml/libkml_1.3.0~r863.orig.tar.gz"
    URL_MD5 211ed5fdf2dd45aeb9c0abc8e1fe42be
    BINARY_DIR ${LIBKML_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy_directory  ${CMAKE_SOURCE_DIR}/patches/${proj} ${LIBKML_SB_SRC}    
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DCMAKE_PREFIX_PATH:STRING=${SB_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
      ${LIBKML_SB_EXPAT_CONFIG}
    )
  
  message(STATUS "  Using libKML SuperBuild version")
  
endif()
endif()
