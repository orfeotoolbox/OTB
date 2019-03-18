
#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

set( OTBApplicationEngine_SRC
  otbWrapperApplicationHtmlDocGenerator.cxx
  otbWrapperComplexOutputImageParameter.cxx
  otbWrapperInputVectorDataListParameter.cxx
  otbWrapperListViewParameter.cxx
  otbWrapperComplexInputImageParameter.cxx
  otbWrapperParameterGroup.cxx
  otbWrapperOutputProcessXMLParameter.cxx
  otbWrapperInputImageListParameter.cxx
  otbWrapperElevationParametersHandler.cxx
  otbWrapperInputFilenameListParameter.cxx
  otbWrapperOutputImageParameter.cxx
  otbWrapperInputImageParameter.cxx
  otbWrapperInputImageParameterUInt8.cxx
  otbWrapperInputImageParameterUInt16.cxx
  otbWrapperInputImageParameterUInt32.cxx
  otbWrapperInputImageParameterInt16.cxx
  otbWrapperInputImageParameterInt32.cxx
  otbWrapperInputImageParameterCInt16.cxx
  otbWrapperInputImageParameterCInt32.cxx
  otbWrapperInputImageParameterCFloat.cxx
  otbWrapperInputImageParameterCDouble.cxx
  otbWrapperInputImageParameterFloat.cxx
  otbWrapperInputImageParameterDouble.cxx
  otbWrapperParameterKey.cxx
  otbWrapperDocExampleStructure.cxx
  otbWrapperInputVectorDataParameter.cxx
  otbWrapperOutputVectorDataParameter.cxx
  otbWrapperInputProcessXMLParameter.cxx
  otbWrapperMapProjectionParametersHandler.cxx
  otbWrapperApplication.cxx
  otbWrapperChoiceParameter.cxx
  otbWrapperApplicationRegistry.cxx
  otbWrapperApplicationFactoryBase.cxx
  otbWrapperCompositeApplication.cxx
  otbWrapperStringListInterface.cxx
  otbWrapperStringListParameter.cxx
  otbWrapperAbstractParameterList.cxx
  otbWrapperParameterList.cxx
  otbWrapperBoolParameter.cxx
  otbWrapperMetaDataHelper.cxx
  otbWrapperParameter.cxx
  )

add_library(OTBApplicationEngine ${OTBApplicationEngine_SRC})
target_link_libraries(OTBApplicationEngine
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBImageManipulation_LIBRARIES}
  ${OTBImageIO_LIBRARIES}
  ${OTBProjection_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBVectorDataIO_LIBRARIES}
  ${OTBTransform_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  ${OTBMPI_LIBRARIES}
  ${OTBMPIConfig_LIBRARIES}
  ${OTBMPITiffWriter_LIBRARIES}
  ${OTBMPIVrtWriter_LIBRARIES}
  )

# target_compile_options(
#   OTBApplicationEngine
#   # PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Q>
#   PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-ftime-report>
#   # PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-E>
#   )

otb_module_target(OTBApplicationEngine)
