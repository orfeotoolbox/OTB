set(proj MVD)

if(NOT __EXTERNAL_${proj}__)
set(__EXTERNAL_${proj}__ 1)

message(STATUS "Setup Monteverdi...")
SETUP_SUPERBUILD(PROJECT ${proj})
message(STATUS "  Using Monteverdi SuperBuild version")

# declare dependencies
ADDTO_DEPENDENCIES_IF_NOT_SYSTEM(${proj} OTB QWT QT4)

INCLUDE_SUPERBUILD_DEPENDENCIES(${${proj}_DEPENDENCIES})
# set proj back to its original value
set(proj MVD)

set(MVD_SB_CONFIG)
ADD_SUPERBUILD_CMAKE_VAR(OTB_DIR)
ADD_SUPERBUILD_CMAKE_VAR(QWT_INCLUDE_DIR)
ADD_SUPERBUILD_CMAKE_VAR(QWT_LIBRARY)
ADD_SUPERBUILD_CMAKE_VAR(QT_QMAKE_EXECUTABLE)

#TODO: control build testing via cmake variable properly
ExternalProject_Add(${proj}
  PREFIX ${proj}
  GIT_REPOSITORY "https://git@git.orfeo-toolbox.org/git/monteverdi2.git"
  GIT_TAG "release-3.2"
  SOURCE_DIR ${MVD_SB_SRC}
  BINARY_DIR ${MVD_SB_BUILD_DIR}
  INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  DEPENDS ${${proj}_DEPENDENCIES}
  CMAKE_CACHE_ARGS
  -DCMAKE_BUILD_TYPE:STRING=Release
  -DBUILD_SHARED_LIBS:BOOL=${SB_BUILD_SHARED_LIBS}
  -DBUILD_TESTING:BOOL=OFF
  -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  -DCMAKE_PREFIX_PATH:STRING=${CMAKE_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
  -DOTB_DATA_ROOT:STRING=${OTB_DATA_ROOT}    
  ${MVD_SB_CONFIG}
)

endif()
