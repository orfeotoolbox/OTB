set(proj LIBSVM)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup LibSVM...")

if(USE_SYSTEM_LIBSVM)
  find_package ( LibSVM )
  add_custom_target(${proj})
  message(STATUS "  Using LibSVM system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using LibSVM SuperBuild version")
    
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    URL "http://www.csie.ntu.edu.tw/~cjlin/libsvm/libsvm-3.20.tar.gz"
    URL_MD5 5f088e5f89da1c65b642300c9c5ea772
    BINARY_DIR ${LIBSVM_SB_BUILD_DIR}
    DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
    INSTALL_DIR ${SB_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      ${LIBSVM_FLAGS}
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt  
      ${LIBSVM_SB_SRC}
    )
  
 set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libsvm.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/liblibsvm${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
  
endif()
endif()
