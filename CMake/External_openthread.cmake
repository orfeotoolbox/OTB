message(STATUS "Setup OpenThread ...")

set(proj OPENTHREAD)

set(DEFAULT_USE_SYSTEM_OPENTHREAD  OFF)

option(USE_SYSTEM_OPENTHREAD "  Use a system build of OpenThread." ${DEFAULT_USE_SYSTEM_OPENTHREAD})
mark_as_advanced(USE_SYSTEM_OPENTHREAD)

if(USE_SYSTEM_OPENTHREAD)
  message(STATUS "  Using OpenThread system version")
else()
  set(${proj}_DEPENDENCIES)
  set(OPENTHREAD_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)
  set(OPENTHREAD_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
  
  ExternalProject_Add(${proj}
    PREFIX ${proj}
    SVN_REPOSITORY "http://svn.openscenegraph.org/osg/OpenThreads/tags/OpenThreads-2.3.0/"
    SVN_REVISION -r 14667
    BINARY_DIR ${OPENTHREAD_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
    CMAKE_COMMAND
      # use 'env' because CTest launcher doesn't perform shell interpretation
      env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
      ${CMAKE_COMMAND}
    DEPENDS ${${proj}_DEPENDENCIES}
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy 
      ${CMAKE_SOURCE_DIR}/patches/${proj}/CMakeLists.txt 
      ${OPENTHREAD_SB_SRC}
    )
  
  message(STATUS "  Using OpenThread SuperBuild version")

endif()