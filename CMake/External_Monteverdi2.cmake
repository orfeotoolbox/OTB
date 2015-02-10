include(External_qwt)

set(proj MONTEVERDI2)

message(STATUS "Setup Monteverdi2...")

set(MVD2_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
set(MVD2_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)

set(${proj}_DEPENDENCIES ICE)

if(USE_SYSTEM_QT4)
  message(STATUS "  Using Qt4 system version")
else()
    set(MVD2_SB_QT4_CONFIG)
    if(MSVC)
        list(APPEND ${proj}_DEPENDENCIES QT4)
        set(MVD2_SB_QT4_CONFIG)
    else(UNIX)
      message(STATUS "  This platform does not provide Qt4 via SuperBuild")
    endif()
endif()

if(USE_SYSTEM_QWT)
  message(STATUS "  Using Qwt system version")
else()
    set(MVD2_SB_QWT_CONFIG)
    if(MSVC)
        list(APPEND ${proj}_DEPENDENCIES QWT)
        set(MVD2_SB_QWT_CONFIG)
    else(UNIX)
      message(STATUS "  This platform does not provide Qwt via SuperBuild")
    endif()
endif()

#TODO: control build testing via cmake variable properly
ExternalProject_Add(${proj}
   PREFIX ${proj}
   HG_REPOSITORY "http://hg.orfeo-toolbox.org/Monteverdi2"
   HG_TAG tip
   BINARY_DIR ${MVD2_SB_BUILD_DIR}
   INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
   DEPENDS ${${proj}_DEPENDENCIES}
   CMAKE_CACHE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=Release
    -DBUILD_SHARED_LIBS:BOOL=${SB_BUILD_SHARED_LIBS}
    -DBUILD_TESTING:BOOL=ON
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
    -DOTB_DATA_ROOT:STRING=${OTB_DATA_ROOT}    
    ${MVD2_SB_QT4_CONFIG}
    ${MVD2_SB_QWT_CONFIG}
)

message(STATUS "  Using Monteverdi2 SuperBuild version")

