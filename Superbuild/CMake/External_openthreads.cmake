set(proj OPENTHREADS)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup OpenThreads ...")

if(MSVC)
set(USE_SYSTEM_OPENTHREADS OFF)
endif()

if(USE_SYSTEM_OPENTHREADS)
  find_package ( OpenThreads REQUIRED )
  add_custom_target(${proj})
  message(STATUS "  Using OpenThread system version")
else()
  SETUP_SUPERBUILD(PROJECT ${proj})
  message(STATUS "  Using OpenThreads SuperBuild version")
  
  #TODO: try to use cmake variable for DOWNLOAD_COMMAND for different platforms
  if(MSVC)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      SVN_REPOSITORY "http://svn.openscenegraph.org/osg/OpenThreads/tags/OpenThreads-2.3.0/"
      SVN_REVISION -r 14667
      SOURCE_DIR ${OPENTHREADS_SB_SRC}
      BINARY_DIR ${OPENTHREADS_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
      CMAKE_CACHE_ARGS
        -DCMAKE_INSTALL_PREFIX:STRING=${SB_INSTALL_PREFIX}
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DBUILD_SHARED_LIBS:BOOL=ON
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt ${OPENTHREADS_SB_SRC}
        CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    )
  else(UNIX or APPLE)
    ExternalProject_Add(${proj}
      PREFIX ${proj}
      URL "http://www.openscenegraph.org/downloads/developer_releases/OpenSceneGraph-3.2.0.zip"
      URL_MD5 4980f8692712a24d4c99f363f80c6814
      SOURCE_DIR ${OPENTHREADS_SB_SRC}
      BINARY_DIR ${OPENTHREADS_SB_BUILD_DIR}
      INSTALL_DIR ${SB_INSTALL_PREFIX}
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
  
  set(${proj}_INCLUDE_DIR ${SB_INSTALL_PREFIX}/include)
  if(WIN32)
    set(${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/OpenThreads.lib)
  elseif(UNIX)
    set(${proj}_LIBRARY ${SB_INSTALL_PREFIX}/lib/libOpenThreads${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()

endif()
endif()
