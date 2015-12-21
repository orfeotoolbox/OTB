set(proj MUPARSER)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup muParser ...")

if(USE_SYSTEM_MUPARSER)
  find_package ( MuParser )
  message(STATUS "  Using muParser system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using muParser SuperBuild version")
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "https://github.com/beltoforion/muparser/archive/v2.2.5.tar.gz"
    URL_MD5 02dae671aa5ad955fdcbcd3fee313fb7
    BINARY_DIR ${MUPARSER_SB_BUILD_DIR}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
        ${MUPARSER_SB_SRC}
    )
  
  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/muparser.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libmuparser${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
