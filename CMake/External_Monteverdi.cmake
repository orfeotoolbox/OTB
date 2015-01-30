include(External_FLTK)

set(proj MONTEVERDI)

message(STATUS "Setup Monteverdi...")

set(MVD_SB_SRC ${CMAKE_BINARY_DIR}/${proj}/src/${proj})
set(MVD_SB_BUILD_DIR ${CMAKE_BINARY_DIR}/${proj}/build)

set(${proj}_DEPENDENCIES OTB)

if(USE_SYSTEM_FLTK)
  message(STATUS "  Using FLTK system version")
else()
    set(MVD_SB_FLTK_CONFIG)
    list(APPEND ${proj}_DEPENDENCIES FLTK)
endif()


ExternalProject_Add(${proj}
   PREFIX ${proj}
   HG_REPOSITORY "http://hg.orfeo-toolbox.org/Monteverdi"
   HG_TAG tip
   BINARY_DIR ${MVD_SB_BUILD_DIR}
   INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
   DEPENDS ${${proj}_DEPENDENCIES}
   CMAKE_CACHE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=Release
    -DBUILD_SHARED_LIBS:BOOL=ON
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    ${MVD_SB_FLTK_CONFIG}
)

message(STATUS "  Using Monteverdi SuperBuild version")

