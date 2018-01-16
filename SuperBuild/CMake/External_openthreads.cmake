set(proj OPENTHREADS)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup OpenThreads ...")

if(MSVC)
set(USE_SYSTEM_OPENTHREADS OFF)
endif()

if(USE_SYSTEM_OPENTHREADS)
  find_package ( OpenThreads )
  add_custom_target(${proj})
  message(STATUS "  Using OpenThread system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using OpenThreads SuperBuild version")

  #TODO: try to use cmake variable for DOWNLOAD_COMMAND for different platforms
  #if(MSVC)
  if(0)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
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
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt ${OPENTHREADS_SB_SRC}
        CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
  #else(UNIX or APPLE)
  else()
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "https://github.com/openscenegraph/OpenSceneGraph/archive/OpenSceneGraph-3.2.0.zip"
      URL_MD5 c35e8a912a9a47625dcfb2c62a5961cb
      SOURCE_DIR ${OPENTHREADS_SB_SRC}
      BINARY_DIR ${OPENTHREADS_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
      CMAKE_COMMAND ${SB_CMAKE_COMMAND}
      DEPENDS ${${proj}_DEPENDENCIES}
      PATCH_COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt
        ${OPENTHREADS_SB_SRC}
    )
  endif()

  set(_SB_${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/OpenThreads.lib)
  elseif(UNIX)
    set(_SB_${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libOpenThreads${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
