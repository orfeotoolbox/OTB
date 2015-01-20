message(STATUS "Setup Ice...")

set(proj ICE)

set(ICE_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)

set(${proj}_DEPENDENCIES OTB)

if(USE_SYSTEM_ITK)
  set(ICE_SB_ITK_CONFIG)
else()
  set(ICE_SB_ITK_CONFIG
    -DITK_DIR:PATH=${CMAKE_INSTALL_PREFIX}/lib/cmake/ITK-4.6
    )
  list(APPEND ${proj}_DEPENDENCIES ITK)
endif()

ExternalProject_Add(${proj}
    DEPENDS ${${proj}_DEPENDENCIES}
    PREFIX ${proj}
    HG_REPOSITORY "http://hg.orfeo-toolbox.org/Ice"
    HG_TAG tip
    BINARY_DIR ${ICE_SB_BUILD_DIR}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
    CMAKE_CACHE_ARGS
      -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DBUILD_SHARED_LIBS:BOOL=ON
      -DBUILD_TESTING:BOOL=${BUILD_TESTING}
      -DBUILD_ICE_APPLICATION:BOOL=${BUILD_ICE_APPLICATION}
      ${ICE_SB_ITK_CONFIG}
      -DOTB_DIR:PATH=${CMAKE_INSTALL_PREFIX}/lib/otb
    #CMAKE_COMMAND
    #  # use 'env' because CTest launcher doesn't perform shell interpretation
    #  env LD_LIBRARY_PATH=${CMAKE_INSTALL_PREFIX}/lib
    #  ${CMAKE_COMMAND}
    )

message(STATUS "  Using Ice SuperBuild version")
