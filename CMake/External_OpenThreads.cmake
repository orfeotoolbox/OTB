message(STATUS "Setup OpenThreads ...")

set(proj OPENTHREADS)

set(DEFAULT_USE_SYSTEM_OPENTHREADS  OFF)

option(USE_SYSTEM_OPENTHREADS "  Use a system build of OpenThread." ${DEFAULT_USE_SYSTEM_OPENTHREADS})
mark_as_advanced(USE_SYSTEM_OPENTHREADS)

if(MSVC)
set(USE_SYSTEM_OPENTHREADS OFF)
endif()

if(USE_SYSTEM_OPENTHREADS)
  message(STATUS "  Using OpenThread system version")
else()
  set(${proj}_DEPENDENCIES)
  set(OPENTHREADS_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(OPENTHREADS_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    #SVN_REPOSITORY "http://svn.openscenegraph.org/osg/OpenThreads/tags/OpenThreads-2.3.0/"
    #SVN_REVISION -r 14667
    URL "http://www.openscenegraph.org/downloads/developer_releases/OpenSceneGraph-3.2.0.zip"
    URL_MD5 4980f8692712a24d4c99f363f80c6814
    SOURCE_DIR ${OPENTHREADS_SB_SRC}
    BINARY_DIR ${OPENTHREADS_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
    CMAKE_COMMAND ${SB_CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
      ${OPENTHREADS_SB_SRC}
    )
  
  message(STATUS "  Using OpenThreads SuperBuild version")

endif()
