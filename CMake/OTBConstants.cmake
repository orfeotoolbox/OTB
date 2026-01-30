#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# This file provides two function that init cmake vars:
# - get_test_const that init all EPSILON_X vars needed for unit tests
# - get_install_const that init all OTB_INSTALL vars

macro(get_test_const)
  set(NOTOL      0.0)
  set(EPSILON_1  0.1)
  set(EPSILON_2  0.01)
  set(EPSILON_3  0.001)
  set(EPSILON_4  0.0001)
  set(EPSILON_5  0.00001)
  set(EPSILON_6  0.000001)
  set(EPSILON_7  0.0000001)
  set(EPSILON_8  0.00000001)
  set(EPSILON_9  0.000000001)
  set(EPSILON_10 0.0000000001)
  set(EPSILON_11 0.00000000001)
  set(EPSILON_12 0.000000000001)
  set(EPSILON_13 0.0000000000001)
  set(EPSILON_14 0.00000000000001)
  set(EPSILON_15 0.000000000000001)
endmacro()

macro(get_install_const)
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
    set(OTB_INSTALL_INCLUDE_DIR "include/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
  endif()

  if(NOT OTB_INSTALL_APP_DIR)
    set(OTB_INSTALL_APP_DIR "${OTB_INSTALL_LIBRARY_DIR}/otb/applications")
  endif()

  if(NOT OTB_INSTALL_DATA_DIR)
    set(OTB_INSTALL_DATA_DIR "share/otb")
  endif()

  if(NOT OTB_INSTALL_DOC_DIR)
    set(OTB_INSTALL_DOC_DIR "share/doc/otb")
  endif()

  if(NOT OTB_INSTALL_PACKAGE_DIR)
    set(OTB_INSTALL_PACKAGE_DIR "${OTB_INSTALL_LIBRARY_DIR}/cmake/OTB-${OTB_VERSION_MAJOR}.${OTB_VERSION_MINOR}")
  endif()

  if(NOT OTB_INSTALL_PYTHON_DIR)
    set(OTB_INSTALL_PYTHON_DIR "${OTB_INSTALL_LIBRARY_DIR}/otb/python")
  endif()
endmacro()

function(get_modules_const)
  get_property(_is_module_activation_opt_list_defined GLOBAL PROPERTY
               OTB_MODULE_ACTIVATION_OPTION_LIST DEFINED)
  if(NOT _is_module_activation_opt_list_defined)
    define_property(GLOBAL PROPERTY OTB_MODULE_ACTIVATION_OPTION_LIST
                           BRIEF_DOCS "List of some optional module that are"
                                      " activated (see thirdparty modules)"
                           FULL_DOCS "List of some optional modules that are"
                                      " activated (see thirdparty modules)."
                                      "Modules can be added to this list with"
                                      " the macro otb_module_activation_option"
                                      " defined in OTBModuleAPI")
  endif()
endfunction()

macro(get_package_const)
  # these are cache variables, so they could be overwritten with -D,
  set(CPACK_PACKAGE_NAME "OTB"
      CACHE STRING "The OTB full package"
  )
  # which is useful in case of packing only selected components instead of the whole thing
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OTB Package"
      CACHE STRING "This package contains the entire OTB project by modules OR in one full package"
  )
  set(CPACK_PACKAGE_VENDOR "CS Group")

  set(CPACK_VERBATIM_VARIABLES YES)

  set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
  set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/build_packages")

  set(CPACK_PACKAGE_VERSION_MAJOR ${OTB_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${OTB_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${OTB_VERSION_PATCH})

  set(CPACK_PACKAGE_CONTACT "thibaut.romain@cs-soprasteria.com")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Thibaut ROMAIN")

  set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
  set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

  # do not include top directory for zip and tar packages
  # If top level is include and the package "Core.tar.gz" when uncompressing
  # it, it creates a folder named "Core"
  set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
  # package name for deb. If set, then instead of some-application-0.9.2-Linux.deb
  # you'll get some-application_0.9.2_amd64.deb (note the underscores too)
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
  # that is if you want every group to have its own package,
  # although the same will happen if this is not set (so it defaults to ONE_PER_GROUP)
  # and CPACK_DEB_COMPONENT_INSTALL is set to YES
  set(CPACK_COMPONENTS_GROUPING ONE_PER_GROUP)
  # without this you won't be able to pack only specified component



  # NOTE TLA: isolate this in a function called before packaging
  # NOTE TLA: we could set the list depending of enabled component.
  # Actually, even if we enable only Core and FeaturesExtraction (for instance)
  # we also generate empty packages for other components
  get_property(CPACK_COMPONENTS_ALL GLOBAL PROPERTY ENABLED_GROUPS)

  list(APPEND CPACK_COMPONENTS_ALL "Core" "Dependencies")

  message(WARNING "CPACK_COMPONENTS_ALL = ${CPACK_COMPONENTS_ALL}")


  if(WIN32)
      set(CPACK_GENERATOR "ZIP")
  else()
      set(CPACK_GENERATOR "TGZ")
  endif()
endmacro()
