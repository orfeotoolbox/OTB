#Adapted from ITKModuleExternal.cmake
# This file ensures the appropriate variables are set up for a project extending
# OTB before including OTBModuleMacros. This is the preferred way to build an
# OTB module outside of the OTB source tree.

if(NOT OTB_FOUND)
  message(FATAL_ERROR "OTB must be found before module macros can be used.")
endif()
if(NOT OTB_VERSION VERSION_GREATER "5.2")
  message(FATAL_ERROR "Requires OTB 5.4 or later to work.")
endif()
if(NOT EXISTS ${OTB_CMAKE_DIR}/OTBModuleMacros.cmake)
  message(FATAL_ERROR "Modules can only be built against an OTB build tree; they cannot be built against an OTB install tree.")
endif()

# Setup build locations.
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OTB_DIR}/bin)
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OTB_DIR}/lib)
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OTB_DIR}/lib)
endif()

# OTB installation structure
if(NOT OTB_INSTALL_RUNTIME_DIR)
  set(OTB_INSTALL_RUNTIME_DIR bin)
endif()
if(NOT OTB_INSTALL_LIBRARY_DIR)
  set(OTB_INSTALL_LIBRARY_DIR lib)
endif()
if(NOT OTB_INSTALL_ARCHIVE_DIR)
  set(OTB_INSTALL_ARCHIVE_DIR lib)
endif()
if(NOT OTB_INSTALL_INCLUDE_DIR)
  set(OTB_INSTALL_INCLUDE_DIR include/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR})
endif()
if(NOT OTB_INSTALL_DATA_DIR)
  set(OTB_INSTALL_DATA_DIR share/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR})
endif()
if(NOT OTB_INSTALL_DOC_DIR)
  set(OTB_INSTALL_DOC_DIR share/doc/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR})
endif()
if(NOT OTB_INSTALL_PACKAGE_DIR)
  set(OTB_INSTALL_PACKAGE_DIR "lib/cmake/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
endif()

# Use OTB's flags.
set(CMAKE_C_FLAGS "${OTB_REQUIRED_C_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} ${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}")
option(BUILD_SHARED_LIBS "Build OTB with shared libraries." ${OTB_BUILD_SHARED})

# Add the OTB_MODULES_DIR to the CMAKE_MODULE_PATH and then use the binary
# directory for the project to write out new ones to.
if(OTB_MODULES_DIR)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${OTB_MODULES_DIR})
endif()
set(OTB_MODULES_DIR "${OTB_DIR}/${OTB_INSTALL_PACKAGE_DIR}/Modules")

#include(OTBExternalData)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
  include(CTest)
  include(OTBModuleTest)
endif()

include(OTBModuleMacros)
include(otb-module.cmake)
set(${otb-module}-targets ${otb-module}Targets)
set(${otb-module}-targets-install "\${OTB_INSTALL_PREFIX}/${OTB_INSTALL_PACKAGE_DIR}/${otb-module}Targets.cmake")
set(${otb-module}_TARGETS_FILE_INSTALL "${${otb-module}-targets-install}")
set(${otb-module}-targets-build "${OTB_DIR}/${OTB_INSTALL_PACKAGE_DIR}/Modules/${otb-module}Targets.cmake")
set(${otb-module}_TARGETS_FILE_BUILD "${${otb-module}-targets-build}")
otb_module_impl()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/CMakeLists.txt AND NOT ${otb-module}_NO_SRC AND "${${otb-module}-targets}")
  install(EXPORT ${${otb-module}-targets} DESTINATION "${OTB_INSTALL_PACKAGE_DIR}/Modules"
          COMPONENT Development)
endif()

set(OTB_TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Testing/Temporary")
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt")
  add_subdirectory(test)
endif()

