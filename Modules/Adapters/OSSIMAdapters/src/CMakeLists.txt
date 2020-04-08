#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

set(OTBOSSIMAdapters_SRC
  otbDEMHandler.cxx
  otbImageKeywordlist.cxx
  otbSensorModelAdapter.cxx
  otbRPCSolverAdapter.cxx
  otbDateTimeAdapter.cxx
  otbFilterFunctionValues.cxx
  otbMetaDataKey.cxx
  otbEllipsoidAdapter.cxx
  otbSarSensorModelAdapter.cxx
  )

add_library(OTBOSSIMAdapters ${OTBOSSIMAdapters_SRC})
target_link_libraries(OTBOSSIMAdapters
  ${OTBGDAL_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBOssim_LIBRARIES}
  ${OTBOssimPlugins_LIBRARIES}
  ${OTBOpenThreads_LIBRARIES}
  )

otb_module_target(OTBOSSIMAdapters)

# add the OTB_OSSIM_VERSION definition
get_target_property(_OTBOSSIMAdapters_COMP_DEF OTBOSSIMAdapters COMPILE_DEFINITIONS)
if(_OTBOSSIMAdapters_COMP_DEF)
  set_target_properties(OTBOSSIMAdapters
    PROPERTIES COMPILE_DEFINITIONS "${_OTBOSSIMAdapters_COMP_DEF};OTB_OSSIM_VERSION=${OTB_OSSIM_VERSION}")
else()
  set_target_properties(OTBOSSIMAdapters
    PROPERTIES COMPILE_DEFINITIONS "OTB_OSSIM_VERSION=${OTB_OSSIM_VERSION}")
endif()
