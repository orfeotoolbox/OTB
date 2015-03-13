set(proj LIBKML)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup libKML ...")

if(USE_SYSTEM_LIBKML)
  find_package ( LibKML )
  add_custom_target(${proj})
  message(STATUS "  Using libKML system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using libKML SuperBuild version")
  # declare dependencies
  set(${proj}_DEPENDENCIES EXPAT ZLIB BOOST)
  INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
  # set proj back to its original value
  set(proj LIBKML)
  
  ADD_SUPERBUILD_CMAKE_VAR(EXPAT_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(EXPAT_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(ZLIB_INCLUDE_DIR)
  ADD_SUPERBUILD_CMAKE_VAR(ZLIB_LIBRARY)
  ADD_SUPERBUILD_CMAKE_VAR(Boost_INCLUDE_DIR)

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
      ${LIBKML_SB_CONFIG}
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_BASE_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlbase.lib)
    set(_SB_${proj}_CONVENIENCE_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlconvenience.lib)
    set(_SB_${proj}_DOM_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmldom.lib)
    set(_SB_${proj}_ENGINE_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlengine.lib)
    set(_SB_${proj}_REGIONATOR_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlregionator.lib)
    set(_SB_${proj}_XSD_LIBRARY ${SB_INSTALL_PREFIX}/lib/kmlxsd.lib)
    set(_SB_${proj}_MINIZIP_LIBRARY ${SB_INSTALL_PREFIX}/lib/minizip.lib)
  elseif(UNIX)
    set(_SB_${proj}_BASE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlbase${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(_SB_${proj}_CONVENIENCE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlconvenience${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(_SB_${proj}_DOM_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmldom${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(_SB_${proj}_ENGINE_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlengine${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(_SB_${proj}_REGIONATOR_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlregionator${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(_SB_${proj}_XSD_LIBRARY ${SB_INSTALL_PREFIX}/lib/libkmlxsd${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(_SB_${proj}_MINIZIP_LIBRARY ${SB_INSTALL_PREFIX}/lib/libminizip${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
  
endif()
endif()
