if( __EXTERNAL_OPENTHREADS__)
  return()
else()
  set(__EXTERNAL_OPENTHREADS__ 1)
endif()

if(USE_SYSTEM_OPENTHREADS)
  message(STATUS "  Using OpenThreads system version")
  return()
endif()


  SETUP_SUPERBUILD(PROJECT OPENTHREADS)
  message(STATUS "  Using OpenThreads SuperBuild version")

  #TODO: try to use cmake variable for DOWNLOAD_COMMAND for different platforms
  #if(MSVC)
  if(0)
    ExternalProject_Add(OPENTHREADS
      PREFIX OPENTHREADS
      SVN_REPOSITORY "http://svn.openscenegraph.org/osg/OpenThreads/tags/OpenThreads-2.3.0/"
      SVN_REVISION -r 14667
      SOURCE_DIR ${OPENTHREADS_SB_SRC}
      BINARY_DIR ${OPENTHREADS_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/patches/OPENTHREADS/CMakeLists.txt ${OPENTHREADS_SB_SRC}
        CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
  #else(UNIX or APPLE)
  else()
    ExternalProject_Add(OPENTHREADS
      PREFIX OPENTHREADS
      URL "http://www.openscenegraph.org/downloads/stable_releases/OpenSceneGraph-3.4.0/source/OpenSceneGraph-3.4.0.zip"
      URL_MD5 a5e762c64373a46932e444f6f7332496
      SOURCE_DIR ${OPENTHREADS_SB_SRC}
      BINARY_DIR ${OPENTHREADS_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
      CMAKE_COMMAND ${SB_CMAKE_COMMAND}
      DEPENDS ${OPENTHREADS_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/patches/OPENTHREADS/CMakeLists.txt
        ${OPENTHREADS_SB_SRC}
    )
  endif()

  set(_SB_OPENTHREADS_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_OPENTHREADS_LIBRARY ${SB_INSTALL_PREFIX}/lib/OpenThreads.lib)
  elseif(UNIX)
    set(_SB_OPENTHREADS_LIBRARY ${SB_INSTALL_PREFIX}/lib/libOpenThreads${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
